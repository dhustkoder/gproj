#ifndef GPROJ_GAME_H_
#define GPROJ_GAME_H_
#include "platform_defs.h"
#include "timer.h"
#include "input.h"

extern void game_init(void);
extern void game_step(input_t input, timer_clk_t now, float dt);
extern void game_term(void);


#endif
