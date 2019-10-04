#ifndef GPROJ_ACTORS_H_
#define GPROJ_ACTORS_H_
#include <limits.h>
#include "types.h"

/*
	An actor is an object that appears in the game world with position and size.
	An actor can have an animation (a sequence of frames).
	An actor can have a single position and size in game world
	An actor can have multiple positions and sizes in the sprite sheet
*/

struct frame {
	const int16_t ms;
	const struct recti ss;
};

struct animation {
	const struct frame* frames;
	int8_t cnt;
	int8_t idx;
};

struct frame_timing {
	timer_clk_t clk;
	int16_t ms;
};

 

extern void actors_move(const float dt,
                        const struct vec2f* restrict const vels,
                        struct vec2f* restrict const wpos,
                        const int cnt);

extern void actors_animate(const timer_clk_t now,
                           struct frame_timing* restrict timings,
                           struct animation* restrict animations,
                           struct vec2i* restrict spos,
                           struct vec2i* restrict ssize,
                           int cnt);


#endif

