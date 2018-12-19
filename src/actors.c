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
	int actor_id;
	uint8_t flags;
};

struct movement {
	float velx;
	float vely;
	int actor_id;
};


static struct {
	struct recti ts[64];
	struct rectf scr[64];
	int cnt;
} actors;
static struct animation anims[64];
static struct movement movs[64];
static int anims_cnt  = 0;
static int movs_cnt   = 0;


int actors_create(const struct rectf* const scr, const struct recti* const ts)
{
	const int id = actors.cnt;
	++actors.cnt;

	actors.scr[id] = *scr;
	actors.ts[id]  = *ts;

	return id;
}

int actors_anim_create(const int actor_id,
                       const struct actor_frame* frames,
                       const int cnt, const int flags)
{
	memset(&anims[anims_cnt], 0, sizeof(anims[0]));
	const int id = anims_cnt;
	++anims_cnt;
	anims[id].actor_id = actor_id;
	actors_anim_set(id, timer_now(), frames, cnt, flags);
	return id;
}



void actors_anim_set(const int anim_id,
                     const uint32_t clk,
                     const struct actor_frame* frames,
                     const int cnt,
                     const int flags)
{
	anims[anim_id].idx = 0;
	anims[anim_id].clk = clk;
	anims[anim_id].frames = frames;
	anims[anim_id].cnt = cnt;
	anims[anim_id].flags = flags;
	if (frames != NULL) {
		anims[anim_id].duration = frames[0].duration;
		actors.ts[anims[anim_id].actor_id] = frames[0].ts;
	}
}


int actors_anim_flags(const int anim_id)
{
	return anims[anim_id].flags;
}


int actors_mov_create(const int actor_id,
                      const float velx,
                      const float vely)
{
	const int id = movs_cnt;
	++movs_cnt;
	
	movs[id].actor_id = actor_id;
	actors_mov_set(id, velx, vely);

	return id;
}

void actors_mov_set(const int mov_id,
                    const float velx,
                    const float vely)
{
	movs[mov_id].velx = velx;
	movs[mov_id].vely = vely;
}

void actors_update(const uint32_t now, const float dt)
{

	for (int i = 0; i < movs_cnt; ++i) {
		actors.scr[movs[i].actor_id].pos.x += movs[i].velx * dt;
		actors.scr[movs[i].actor_id].pos.y += movs[i].vely * dt;
	}

	for (int i = 0; i < anims_cnt; ++i) {
		struct animation* const anim = &anims[i];
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
			actors.ts[anim->actor_id] = anim->frames[anim->idx].ts;
		}
	}

	render_ts(actors.ts, actors.scr, actors.cnt);
}

