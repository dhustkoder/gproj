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


#define ACTOR_MAX_ANIMATIONS         (INT8_MAX - 1)
#define ACTOR_MAX_ANIMATION_FRAMES   (INT8_MAX - 1)
#define ACTOR_MAX_ANIMATION_FRAME_MS (INT16_MAX - 1)

#ifndef GPROJ_MAX_ACTORS
#error "GPROJ_MAX_ACTORS must be defined"
#endif

#if GPROJ_MAX_ACTORS > INT_MAX || GPROJ_MAX_ACTORS < 2 || (GPROJ_MAX_ACTORS % 2) != 0
#error "GPROJ_MAX_ACTORS out of range; Must fit into an int. Must be multiple of 2"
#endif

enum actor_flag {
	ACTOR_FLAG_DISABLED = (0x01),
	ACTOR_FLAG_LOOP     = (0x02),
	ACTOR_FLAG_BIDIR    = (0x04),
	ACTOR_FLAG_BKWD     = (0x08),
	ACTOR_FLAG_FLIPH    = (0x10)
};

typedef uint8_t actor_flag_t;

struct actor_frame {
	const int16_t ms;
	const struct recti ss;
};

struct actor_animation {
	const struct actor_frame* frames;
	int8_t cnt;
};


struct actor_animation_collection  {
	const struct actor_animation* animations;
	int8_t cnt;
};

struct actors_ctrl {
	struct rectf* wrects;
	struct vec2f* vels;
	int8_t* anim_idxs;
	actor_flag_t* flags;
};

extern struct actors_ctrl actors_create(
	const struct actor_animation_collection* collections,
	int cnt
);

extern void actors_update(timer_clk_t now, float dt);



#endif
