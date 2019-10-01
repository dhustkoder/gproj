#ifndef GPROJ_ACTORS_H_
#define GPROJ_ACTORS_H_
#include <limits.h>
#include "types.h"


#ifndef GPROJ_MAX_ACTORS
#error "GPROJ_MAX_ACTORS must be defined"
#endif

#if GPROJ_MAX_ACTORS > INT_MAX || GPROJ_MAX_ACTORS < 2 || (GPROJ_MAX_ACTORS % 2) != 0
#error "GPROJ_MAX_ACTORS out of range; Must fit into an int. Must be multiple of 2"
#endif

extern int actors_create(const struct rectf* scr);

extern void actors_anim_set(const int actor_id,
                            const struct actor_frame* frames,
                            int8_t nframes,
                            actor_anim_flag_t flags,
                            timer_clk_t clk);

extern int actors_anim_get_flags(int actor_id);
extern void actors_anim_set_flags(int actor_id, int flags);
extern void actors_mov_set(int actor_id, float velx, float vely);
extern void actors_update(timer_clk_t now, float dt);
extern struct vec2f actors_get_pos(int actor_id);
extern void actors_send_render(void);


#endif
