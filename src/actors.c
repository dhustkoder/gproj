#include <assert.h>
#include "logger.h"
#include "timer.h"
#include "render.h"
#include "actors.h"

static struct recti ss_rects[GPROJ_MAX_ACTORS];
static struct rectf scr_rects[GPROJ_MAX_ACTORS];
static struct vec2f movs[GPROJ_MAX_ACTORS];
static const struct actor_frame* anim_frames[GPROJ_MAX_ACTORS];
static timer_clk_t anim_clks[GPROJ_MAX_ACTORS];
static int16_t anim_ms[GPROJ_MAX_ACTORS];
static int8_t anim_cnts[GPROJ_MAX_ACTORS];
static int8_t anim_idxs[GPROJ_MAX_ACTORS];
static actor_anim_flag_t anim_flags[GPROJ_MAX_ACTORS];
static int nacts;

static bool need_render = false;

int actors_create(const struct rectf* const scr)
{
	const int id = nacts++;
	assert(nacts < GPROJ_MAX_ACTORS);
	scr_rects[id] = *scr;
	return id;
}

void actors_anim_set(const int actor_id,
                     const struct actor_frame* const frames,
                     const int8_t nframes,
                     const actor_anim_flag_t flags,
                     const timer_clk_t clk)
{
	assert(frames != NULL);
	assert(nframes <= MAX_ACTOR_FRAMES);
	need_render = true;
	anim_idxs[actor_id] = 0;
	anim_clks[actor_id] = clk;
	anim_frames[actor_id] = frames;
	anim_cnts[actor_id] = nframes;
	anim_flags[actor_id] = flags;
	anim_ms[actor_id] = frames[0].ms;
	ss_rects[actor_id] = frames[0].ss;
}


int actors_anim_get_flags(int actor_id)
{
	return anim_flags[actor_id];
}


void actors_anim_set_flags(int actor_id, int flags)
{
	anim_flags[actor_id] = flags;
}


void actors_mov_set(const int actor_id,
                    const float velx,
                    const float vely)
{
	movs[actor_id].x = velx;
	movs[actor_id].y = vely;
}


struct vec2f actors_get_pos(int actor_id)
{
	return scr_rects[actor_id].pos;
}


void actors_update(const timer_clk_t now, const float dt)
{
	const int cnt = nacts;

	for (int i = 0; i < cnt; ++i) {
		if (movs[i].x < -0.01f || movs[i].x > 0.01f ||
		    movs[i].y < -0.01f || movs[i].y > 0.01f) {
			scr_rects[i].pos.x += movs[i].x * dt;
			scr_rects[i].pos.y += movs[i].y * dt;
			need_render = true;
		}
	}

	for (int i = 0; i < cnt; ++i) {
		const int flags = anim_flags[i];
		if (flags&ANIM_FLAG_DISABLED)
			continue;

		if (((int)(now - anim_clks[i])) >= anim_ms[i]) {
			need_render = true;
			anim_clks[i] = now;
			anim_idxs[i] += flags&ANIM_FLAG_BKWD ? -1 : 1;
			if (anim_idxs[i] >= anim_cnts[i] || anim_idxs[i] < 0) {
				if (flags&ANIM_FLAG_LOOP) {
					if (anim_idxs[i] > 0 && flags&ANIM_FLAG_BIDIR) {
						anim_idxs[i] = anim_cnts[i] - 2;
						anim_flags[i] |= ANIM_FLAG_BKWD;
					} else if (anim_idxs[i] < 0) {
						anim_idxs[i] = 1;
						anim_flags[i] &= ~ANIM_FLAG_BKWD;
					} else {
						anim_idxs[i] = 0;
					}
				} else {
					anim_idxs[i] = 0;
					anim_flags[i] |= ANIM_FLAG_DISABLED;
				}
			}

			anim_ms[i] = anim_frames[i][anim_idxs[i]].ms;
			ss_rects[i] = anim_frames[i][anim_idxs[i]].ss;
		}
	}

	if (need_render) {
		need_render = false;
		render_actors(ss_rects, scr_rects, anim_flags, cnt);
	}
}

