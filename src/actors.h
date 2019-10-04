#ifndef GPROJ_ACTORS_H_
#define GPROJ_ACTORS_H_
#include <limits.h>
#include "types.h"

/*
	An actor is a single object that appears in the game world 
	that has a set of animations (each animation is a sequence of frames).
	single position and size in game world
	multiple positions and sizes in the sprite sheet
*/


enum animation_flag {
	ANIMATION_FLAG_LOOP     = (0x01),
	ANIMATION_FLAG_BIDIR    = (0x02),
	ANIMATION_FLAG_BKWD     = (0x04),
};

enum sprite_flag {
	SPRITE_FLAG_FLIPH = (0x01),
};

typedef uint8_t animation_flag_t;
typedef uint8_t sprite_flag_t;

struct actor_frame {
	const int16_t ms;
	const struct recti ss;
};

 
extern void actors_move(float dt,
                        const struct vec2f* wpos,
                        const struct vec2f* vels,
                        struct vec2f* out,
                        int cnt);



#endif
