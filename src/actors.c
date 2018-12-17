#include <assert.h>
#include "timer.h"
#include "render.h"
#include "actors.h"


static struct actor actors[64];
static int actors_cnt;


struct actor* actors_create(const int cnt)
{
	struct actor* const ret = &actors[actors_cnt];
	actors_cnt += cnt;
	return ret;
}

void actors_update(const uint32_t now)
{
	for (int i = 0; i < actors_cnt; ++i) {
		struct actor_animation* const anim = &actors[i].anim;
		
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

	render_actors(actors, actors_cnt);
}

