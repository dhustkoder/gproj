#ifndef GPROJ_GAME_MAPS_H_
#define GPROJ_GAME_MAPS_H_
#include "world.h"

#define GAME_NWORLDS (1)


extern void maps_init(void);
extern void maps_update(const struct vec2i* campos,
                        timer_clk_t now,
                        float dt);
extern void maps_send_render(void);


#endif

