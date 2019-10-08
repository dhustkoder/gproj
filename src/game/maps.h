#ifndef GPROJ_GAME_MAPS_H_
#define GPROJ_GAME_MAPS_H_
#include "world.h"

extern void maps_init(void);
extern void maps_term(void);
extern void maps_update(const struct vec2f* restrict cam,
                        timer_clk_t now,
                        float dt);
extern void maps_send_render(void);


#endif

