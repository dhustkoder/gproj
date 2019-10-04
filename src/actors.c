#include <assert.h>
#include "workman.h"
#include "logger.h"
#include "timer.h"
#include "render.h"
#include "actors.h"


void actors_move(const float dt,
                 const struct vec2f* const wpos,
                 const struct vec2f* const vels,
                 struct vec2f* const out,
                 const int cnt)
{
	for (int i = 0; i < cnt; ++i) {
		out[i].x = wpos[i].x + vels[i].x * dt;
		out[i].y = wpos[i].y + vels[i].y * dt;
	}
}


void actors_animate(const timer_clk_t now,
                    struct frame_timing* restrict const timings,
                    struct animation* restrict const animations,
                    const int cnt)
{
	for (int i = 0; i < cnt; ++i) {
		if ((int)(now - timings[i].clk) > timings[i].ms) {
			timings[i].clk = now;
			if ((animations[i].idx + 1) < animations[i].cnt) {
				animations[i].idx += 1;
			} else {
				animations[i].idx = 0;
			}
			timings[i].ms = animations[i].frames[animations[i].idx].ms;
		}
	}
}


