#ifndef GPROJ_PROFILER_H_
#define GPROJ_PROFILER_H_
#include "platform_defs.h"
#include "timer.h"


#ifdef GPROJ_PROFILING


extern void profiler_init(void);
extern void profiler_block_start(const char* id, int max_hits);
extern void profiler_block_end(void);
extern void profiler_term(void);


#else


#define profiler_init(...)        ((void)0)
#define profiler_block_start(...) ((void)0)
#define profiler_block_end(...)   ((void)0)
#define profiler_term(...)        ((void)0)


#endif


#endif
