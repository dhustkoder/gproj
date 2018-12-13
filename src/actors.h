#ifndef GPROJ_ACTORS_H_
#define GPROJ_ACTORS_H_
#include "types.h"


#define ACTOR_TYPE_PLAYER (0x00)


struct actor {
	int type;
	struct recti ts;
	struct recti scr;
};


extern int actors_add(const struct actor* actor);
extern void actors_update(void);


#endif
