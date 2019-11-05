#ifndef GPROJ_TIMER_H_
#define GPROJ_TIMER_H_
#include "platform_defs.h"


typedef DWORD timer_clk_t;
#define TIMER_CLK_FMT     U32_FMT
#define timer_sleep(ms)   Sleep(ms)
#define timer_now()       GetTickCount()



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
