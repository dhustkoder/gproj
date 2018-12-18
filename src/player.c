#include "types.h"
#include "render.h"
#include "log.h"
#include "actors.h"
#include "map.h"
#include "timer.h"
#include "player.h"


input_button_t input_buttons_states;


static const struct actor_frame walk_up[] = {
	{ 144, { .size = { 16, 16 }, .pos = { 511, 3040 } } },
	{ 144, { .size = { 16, 16 }, .pos = { 526, 3040 } } }
};
static const struct actor_frame walk_down[] = {
	{ 144, { .size = { 16, 16 }, .pos = { 512, 3072 } } },
	{ 144, { .size = { 16, 16 }, .pos = { 527, 3072 } } }
};
static const struct actor_frame walk_left[] = {
	{ 144, { .size = { 16, 16 }, .pos = { 510, 3088 } } },
	{ 144, { .size = { 16, 16 }, .pos = { 527, 3088 } } }
};
static const struct actor_frame walk_right[] = {
	{ 144, { .size = { 16, 16 }, .pos = { 512, 3056 } } },
	{ 144, { .size = { 16, 16 }, .pos = { 527, 3056 } } }
};
/*
static const struct actor_frame attack_right[] = {
	{ 1250, { .size = { 16, 16 }, .pos = { 544, 3056 } } },
	{ 125, { .size = { 16, 16 }, .pos = { 559, 3056 } } },
	{ 100, { .size = { 16, 16 }, .pos = { 575, 3056 } } }
};

static const struct actor_frame attack_left[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 544, 3056 } } },
	{ 125, { .size = { 16, 16 }, .pos = { 559, 3056 } } },
	{ 100, { .size = { 16, 16 }, .pos = { 575, 3056 } } }
};
static const struct actor_frame attack_up[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 544, 3056 } } },
	{ 125, { .size = { 16, 16 }, .pos = { 559, 3056 } } },
	{ 100, { .size = { 16, 16 }, .pos = { 575, 3056 } } }
};
static const struct actor_frame attack_down[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 544, 3056 } } },
	{ 125, { .size = { 16, 16 }, .pos = { 559, 3056 } } },
	{ 100, { .size = { 16, 16 }, .pos = { 575, 3056 } } }
};
*/

static struct actor* actor;
static int anim_id = 0;
static float velocity = 5 * GPROJ_TILE_WIDTH;
static input_button_t prev_buttons_states;


void player_init(void)
{

	actor = actors_create(1);
	anim_id = actors_anim_create(actor, walk_down, ARRSZ(walk_down));
	actor->scr = (struct rectf) {
		.size = { 32, 32 }, .pos = { GPROJ_SCR_WIDTH / 2, GPROJ_SCR_HEIGHT / 2 } 
	};

}


void player_update(const uint32_t now, const float dt)
{
	if (input_buttons_states&(INPUT_BUTTON_LEFT |
	                          INPUT_BUTTON_RIGHT|
	                          INPUT_BUTTON_UP   |
	                          INPUT_BUTTON_DOWN)) {
	
		struct rectf mov = actor->scr;

		if (input_buttons_states&INPUT_BUTTON_DOWN) {
			mov.pos.y += velocity * dt;
			if (!(prev_buttons_states&INPUT_BUTTON_DOWN)) {
				actors_anim_set(anim_id, now, walk_down, ARRSZ(walk_down));
			}
		} else if (input_buttons_states&INPUT_BUTTON_UP) {
			mov.pos.y -= velocity * dt;
			if (!(prev_buttons_states&INPUT_BUTTON_UP)) {
				actors_anim_set(anim_id, now, walk_up, ARRSZ(walk_up));
			}
		} else if (input_buttons_states&INPUT_BUTTON_LEFT) {
			mov.pos.x -= velocity * dt;
			if (!(prev_buttons_states&INPUT_BUTTON_LEFT)) {
				actors_anim_set(anim_id, now, walk_left, ARRSZ(walk_left));
			}
		} else if (input_buttons_states&INPUT_BUTTON_RIGHT) {
			mov.pos.x += velocity * dt;
			if (!(prev_buttons_states&INPUT_BUTTON_RIGHT)) {
				actors_anim_set(anim_id, now, walk_right, ARRSZ(walk_right));
			}
		}

		mov.pos.x += mov.size.x / 2.f;
		mov.pos.y += mov.size.y / 2.f;
		if (!map_is_blocking(&mov)) {
			mov.pos.x -= mov.size.x / 2.f;
			mov.pos.y -= mov.size.y / 2.f;
			actor->scr = mov;
		}

	}

	prev_buttons_states = input_buttons_states;

}

