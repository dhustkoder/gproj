#ifndef GPROJ_TIMER_H_
#define GPROJ_TIMER_H_
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include "types.h"


typedef Uint32 timer_clk_t;
#define TIMER_CLK_FMT   SDL_PRIu32
#define timer_sleep(ms) SDL_Delay(ms)
#define timer_now()     ((timer_clk_t)SDL_GetTicks())



#ifdef GPROJ_PROFILING

extern void timer_profiler_init();
extern void timer_profiler_block_start(const char* id, int max_hits);
extern void timer_profiler_block_end();

#else

#define timer_profiler_init(...) 
#define timer_profiler_block_start(id, max_hits)
#define timer_profiler_block_end(...)

#endif

#endif
