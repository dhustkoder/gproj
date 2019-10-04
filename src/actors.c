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



