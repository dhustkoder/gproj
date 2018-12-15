#ifndef GPROJ_ACTORS_H_
#define GPROJ_ACTORS_H_
#include "types.h"


enum animation_flags {
	ANIM_FLAG_ENABLED = (0x01),
	ANIM_FLAG_LOOP    = (0x02)
};


struct actor_frame {
	uint32_t duration;
	struct recti ts;
};

struct actor_animation {
	const struct actor_frame* frames;
	uint32_t clk;
	int cnt;
	int idx;
	uint8_t flags;
};

struct actor {
	struct rectf scr;
	struct actor_animation anim;
};

extern int actors_add(struct actor* actor);
extern void actors_update(uint32_t now);


#endif
