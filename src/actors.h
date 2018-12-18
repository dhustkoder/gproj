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

struct actor {
	struct rectf scr;
	struct recti ts;
};

extern struct actor* actors_create(int cnt);
extern int actors_anim_create(struct actor* actor,
                              const struct actor_frame* frames,
                              int cnt);
extern void actors_anim_set(const int anim_id,
                            uint32_t clk,
                            const struct actor_frame* frames,
                            int cnt);
extern void actors_update(uint32_t now);


#endif
