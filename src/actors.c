#include <assert.h>
#include "timer.h"
#include "render.h"
#include "actors.h"


struct animation {
	const struct actor_frame* frames;
	uint32_t clk;
	uint32_t duration;
	int cnt;
	int idx;
	uint8_t flags;
};


static struct recti tss[64];
static struct rectf scrs[64];
static struct animation anims[64];
static struct vec2f movs[64];
static int nacts;


int actors_create(const struct rectf* const scr, const struct recti* const ts)
{
	const int id = nacts++;
	scrs[id] = *scr;
	tss[id]  = *ts;
	memset(&anims[id], 0, sizeof(anims[id]));
	memset(&movs[id], 0, sizeof(movs[id]));
	return id;
}

void actors_anim_set(const int actor_id,
                     const uint32_t clk,
                     const struct actor_frame* const frames,
                     const int cnt,
                     const int flags)
{
	anims[actor_id].idx = 0;
	anims[actor_id].clk = clk;
	anims[actor_id].frames = frames;
	anims[actor_id].cnt = cnt;
	anims[actor_id].flags = flags;
	if (frames != NULL) {
		anims[actor_id].duration = frames[0].duration;
		tss[actor_id] = frames[0].ts;
	}
}


int actors_anim_get_flags(int actor_id)
{
	return anims[actor_id].flags;
}

void actors_anim_set_flags(int actor_id, int flags)
{
	anims[actor_id].flags = flags;
}



void actors_mov_set(const int actor_id,
                    const float velx,
                    const float vely)
{
	movs[actor_id].x = velx;
	movs[actor_id].y = vely;
}

void actors_update(const uint32_t now, const float dt)
{

	for (int i = 0; i < nacts; ++i) {
		scrs[i].pos.x += movs[i].x * dt;
		scrs[i].pos.y += movs[i].y * dt;
	}


	for (int i = 0; i < nacts; ++i) {
		struct animation* const anim = &anims[i];
		if (anim->frames == NULL)
			continue;
		if (anim->flags&ANIM_FLAG_DISABLED)
			continue;
		if ((now - anim->clk) >= anim->duration) {
			anim->clk = now;
			anim->idx++;
			if (anim->idx >= anim->cnt) {
				anim->idx = 0;
				if (!(anim->flags&ANIM_FLAG_LOOP))
					anim->flags |= ANIM_FLAG_DISABLED;
			}
			anim->duration = anim->frames[anim->idx].duration;
			tss[i] = anim->frames[anim->idx].ts;
		}
	}

	render_ts(tss, scrs, nacts);
}

