#ifndef GPROJ_ACTORS_H_
#define GPROJ_ACTORS_H_
#include "types.h"


#define ACTOR_TYPE_PLAYER (0x00)

struct actor_frame {
	uint32_t duration;
	struct recti ts;
};

struct actor {
	struct recti scr;
	struct recti ts;
};


extern int actors_add(const struct actor* actor);
extern void actors_update(void);


#endif
