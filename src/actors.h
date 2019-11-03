#ifndef GPROJ_ACTORS_H_
#define GPROJ_ACTORS_H_
#include "platform_defs.h"

/*
	An actor is an object that appears in the game world with position and size.
	An actor can have an animation (a sequence of frames).
	An actor can have a single position and size in game world
	An actor can have multiple positions and sizes in the sprite sheet
*/

struct frame {
	const s16 ms;
	const struct recti ss;
};

struct animation {
	const struct frame* frames;
	s8 cnt;
	s8 idx;
};

struct frame_timing {
	timer_clk_t clk;
	s16 ms;
};



extern void actors_move(float dt,
                        const struct vec2f* restrict const vels,
                        struct vec2f* restrict const wpos,
                        int cnt);

extern void actors_animate(const timer_clk_t now,
                           struct frame_timing* timings,
                           struct animation* animations,
                           struct vec2i* restrict spos,
                           struct vec2i* restrict ssize,
                           int cnt);


#endif

