#ifndef GPROJ_TIMER_H_
#define GPROJ_TIMER_H_
#include "platform_defs.h"


typedef Uint32 timer_clk_t;
#define TIMER_CLK_FMT      SDL_PRIu32
#define timer_sleep(ms)    SDL_Delay(ms)
#define timer_now()        ((timer_clk_t)SDL_GetTicks())


typedef double timer_hp_clk_t;
#define TIMER_HP_CLK_FMT ".12lf"

#define timer_high_precision_counter() (SDL_GetPerformanceCounter())
#define timer_high_precision_counter_frequency() (SDL_GetPerformanceFrequency())


#endif

