#include <assert.h>
#include "timer.h"
#include "render.h"
#include "actors.h"


struct animation {
	const struct actor_frame* frames;
	uint32_t clk;
	int cnt;
	int idx;
	int actor_id;
	uint8_t flags;
};


static struct actor actors[64];
static struct animation anims[64];
static int actors_cnt = 0;
static int anims_cnt  = 0;


struct actor* actors_create(const int cnt)
{
	struct actor* const ret = &actors[actors_cnt];
	actors_cnt += cnt;
	return ret;
}

int actors_anim_create(struct actor* actor,
                       const struct actor_frame* frames,
                       const int cnt)
{
	memset(&anims[anims_cnt], 0, sizeof(anims[0]));
	const int id = anims_cnt;
	++anims_cnt;
	anims[id].actor_id = actors - actor;
	actors_anim_set(id, timer_now(), frames, cnt);
	return id;
}

void actors_anim_set(const int anim_id,
                     const uint32_t clk,
                     const struct actor_frame* frames,
                     const int cnt)
{
	anims[anim_id].clk = clk;
	anims[anim_id].frames = frames;
	anims[anim_id].cnt = cnt;
	anims[anim_id].flags = ANIM_FLAG_LOOP|ANIM_FLAG_ENABLED;
	actors[anims[anim_id].actor_id].ts = frames[0].ts;
} 

void actors_update(const uint32_t now)
{
	for (int i = 0; i < anims_cnt; ++i) {
		struct animation* const anim = &anims[i];
		
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
			actors[anim->actor_id].ts = anim->frames[anim->idx].ts;
		}

	}

	render_actors(actors, actors_cnt);
}

