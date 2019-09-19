#include <assert.h>
#include "timer.h"
#include "render.h"
#include "actors.h"



static struct recti tss[64];
static struct rectf scrs[64];
static struct actor_anim anims[64];
static struct vec2f movs[64];
static int nacts;


int actors_create(const struct rectf* const scr)
{
	const int id = nacts++;
	scrs[id] = *scr;
	memset(&tss[id], 0, sizeof(tss[id]));
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
		if (anims[i].frames == NULL)
			continue;

		const int flags = anims[i].flags;
		if (flags&ANIM_FLAG_DISABLED)
			continue;

		if ((now - anims[i].clk) >= anims[i].duration) {
			anims[i].clk = now;
			anims[i].idx += flags&ANIM_FLAG_BKWD ? -1 : 1;
			if (anims[i].idx >= anims[i].cnt || anims[i].idx < 0) {
				if (flags&ANIM_FLAG_LOOP) {
					if (anims[i].idx > 0 && flags&ANIM_FLAG_BIDIR) {
						anims[i].idx = anims[i].cnt - 2;
						anims[i].flags |= ANIM_FLAG_BKWD;
					} else if (anims[i].idx < 0) {
						anims[i].idx = 1;
						anims[i].flags &= ~ANIM_FLAG_BKWD;
					} else {
						anims[i].idx = 0;
					}
				} else {
					anims[i].idx = 0;
					anims[i].flags |= ANIM_FLAG_DISABLED;
				}
			}

			anims[i].duration = anims[i].frames[anims[i].idx].duration;
			tss[i] = anims[i].frames[anims[i].idx].ts;
		}
	}

	render_ts(tss, scrs, anims, nacts);
}

