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

 
extern void actors_move(float dt,
                        const struct vec2f* wpos,
                        const struct vec2f* vels,
                        struct vec2f* out,
                        int cnt);


void actors_animate(const timer_clk_t now,
                    struct frame_timing* restrict const timings,
                    struct animation* restrict const animations,
                    int cnt);


#endif

