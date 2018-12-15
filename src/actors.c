#include <assert.h>
#include "timer.h"
#include "render.h"
#include "actors.h"


static struct actor* actors[64];
static int actors_cnt;


int actors_add(struct actor* const actor)
{
	actors[actors_cnt] = actor;
	return actors_cnt++;
}

void actors_update(const uint32_t now)
{
	for (int i = 0; i < actors_cnt; ++i) {
		struct actor_animation* const anim = &actors[i]->anim;
		
		assert(anim != NULL);
		
		if (!(anim->flags&ANIM_FLAG_ENABLED))
			continue;
		if ((now - anim->clk) >= anim->frames[anim->idx].duration) {
			anim->clk = now;
			anim->idx++;
			if (anim->idx >= anim->cnt) {
				if (anim->flags&ANIM_FLAG_LOOP)
					anim->idx = 0;
				else
					continue;
			}
		}
	}

	render_actors((const struct actor**)actors, actors_cnt);
}

