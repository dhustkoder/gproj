#ifndef GPROJ_ACTORS_H_
#define GPROJ_ACTORS_H_
#include "types.h"

#define GPROJ_MAX_ACTORS 64


extern int actors_create(const struct rectf* scr);

extern void actors_anim_set(const int actor_id,
                            const struct actor_frame* frames,
                            uint8_t cnt,
                            actor_anim_flag_t flags,
                            timer_clk_t clk);

extern int actors_anim_get_flags(int actor_id);
extern void actors_anim_set_flags(int actor_id, int flags);
extern void actors_mov_set(int actor_id, float velx, float vely);
extern void actors_update(uint32_t now, float dt);
extern struct vec2f actors_get_pos(int actor_id);

#endif
