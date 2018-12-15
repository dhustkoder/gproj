#include "timer.h"
#include "render.h"
#include "actors.h"


static struct actor* actors[64];
static struct actor_animation* animations[64];
static int actors_cnt;


int actors_add(struct actor* const actor)
{
	actors[actors_cnt] = actor;
	return actors_cnt++;
}

void actors_set_animation(const int actor_id, struct actor_animation* anim)
{
	animations[actor_id] = anim;
}

void actors_update(const uint32_t now)
{
	for (int i = 0; i < 64; ++i) {
		struct actor_animation* const anim = animations[i];
		if (anim == NULL)
			continue;
		if (!(anim->flags&ANIM_FLAG_ENABLED))
			continue;

		if ((now - anim->clk) >= anim->frames[anim->idx].duration) {
			anim->idx++;
			if (anim->idx >= anim->cnt) {
				if (anim->flags&ANIM_FLAG_LOOP)
					anim->idx = 0;
				else
					continue;
			}
			anim->clk = now;
			actors[i]->ts = anim->frames[anim->idx].ts;
		} else if (memcmp(&actors[i]->ts, &anim->frames[anim->idx].ts, sizeof(actors[i]->ts)) != 0) {
			actors[i]->ts = anim->frames[anim->idx].ts;
		}
	}

	render_actors(actors, actors_cnt);
}

