#ifndef GPROJ_GAME_H_
#define GPROJ_GAME_H_
#include "types.h"


extern void game_init();
extern void game_step(timer_clk_t now, float dt);
extern void game_term();


#endif
