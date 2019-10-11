#include <assert.h>
#include "workman.h"
#include "logger.h"
#include "timer.h"
#include "render.h"
#include "actors.h"


void actors_move(const float dt,
                 const struct vec2f* restrict const vels,
                 struct vec2f* restrict const wpos,
                 const int cnt)
{
	for (int i = 0; i < cnt; ++i) {
		wpos[i].x += vels[i].x * dt;
		wpos[i].y += vels[i].y * dt;
	}
}


void actors_animate(const timer_clk_t now,
                    struct frame_timing* const timings,
                    struct animation* const animations,
                    struct vec2i* const restrict spos,
                    struct vec2i* const restrict ssize,
                    const int cnt)
{
	for (int i = 0; i < cnt; ++i) {
		if ((now - timings[i].clk) > (timer_clk_t)timings[i].ms) {
			timings[i].clk = now;
			if ((animations[i].idx + 1) < animations[i].cnt) {
				animations[i].idx += 1;
			} else {
				animations[i].idx = 0;
			}
			timings[i].ms = animations[i].frames[animations[i].idx].ms;
			spos[i] = animations[i].frames[animations[i].idx].ss.pos;
			ssize[i] = animations[i].frames[animations[i].idx].ss.size;
		}
	}
}


