#ifndef GPROJ_TIMER_H_
#define GPROJ_TIMER_H_
#include <SDL2/SDL_timer.h>
#include <stdint.h>
#include <inttypes.h>

typedef Uint32 timer_clk_t;
typedef double timer_hp_clk_t;

#define TIMER_CLK_FMT PRIu32
#define TIMER_HP_CLK_FMT "%.9lf"

#define timer_sleep(ms)                  SDL_Delay(ms)
#define timer_now()                      ((timer_clk_t)SDL_GetTicks())
#define timer_high_precision_counter()   ((timer_hp_clk_t)SDL_GetPerformanceCounter())
#define timer_high_precision_frequency() ((timer_hp_clk_t)SDL_GetPerformanceFrequency())

#define timer_profiler_start() timer_high_precision_counter()

#define timer_profiler_end(start) \
	((timer_high_precision_counter() - start) / timer_high_precision_frequency())


#endif
