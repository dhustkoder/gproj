#ifndef GPROJ_TIMER_H_
#define GPROJ_TIMER_H_
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include "platform_defs.h"


typedef Uint32 timer_clk_t;
#define TIMER_CLK_FMT   SDL_PRIu32
#define timer_sleep(ms) SDL_Delay(ms)
#define timer_now()     ((timer_clk_t)SDL_GetTicks())



#ifdef GPROJ_PROFILING

extern void timer_profiler_init(void);
extern void timer_profiler_block_start(const char* id, int max_hits);
extern void timer_profiler_block_end(void);
extern void timer_profiler_term(void);

#else

#define timer_profiler_init(...)
#define timer_profiler_block_start(id, max_hits)
#define timer_profiler_block_end(...)
#define timer_profiler_term(...)

#endif

#endif
