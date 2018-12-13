#include "render.h"
#include "actors.h"

input_button_t input_buttons_states;


static struct actor actors[32];
static int actors_cnt = 0;



static void update_player(struct actor* const actor)
{
	if (input_buttons_states&INPUT_BUTTON_UP) {
		actor->scr.pos.y -= 1;
	} else if (input_buttons_states&INPUT_BUTTON_DOWN) {
		actor->scr.pos.y += 1;
	} else if (input_buttons_states&INPUT_BUTTON_LEFT) {
		actor->scr.pos.x -= 1;
	} else if (input_buttons_states&INPUT_BUTTON_RIGHT) {
		actor->scr.pos.x += 1;
	}
}



int actors_add(const struct actor* const actor)
{
	actors[actors_cnt] = *actor;
	return actors_cnt++;
}

void actors_update(void)
{
	for (int i = 0; i < actors_cnt; ++i) {
		struct actor* actor = &actors[i];
		switch (actor->type) {
		case ACTOR_TYPE_PLAYER:
			update_player(actor);
			break;

		}
	}
	render_actors(actors, actors_cnt);
}

