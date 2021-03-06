#ifndef GPROJ_TIMER_H_
#define GPROJ_TIMER_H_
#include "platform_defs.h"


typedef DWORD timer_clk_t;
#define TIMER_CLK_FMT     S32_FMT
#define timer_sleep(ms)   Sleep(ms)
#define timer_now()       (GetTickCount())


typedef double timer_hp_clk_t;
#define TIMER_HP_CLK_FMT ".12lf"



static inline timer_hp_clk_t timer_high_precision_counter_frequency(void)
{
	extern timer_hp_clk_t gproj_timer_hp_freq;
	return gproj_timer_hp_freq;
}

static inline timer_hp_clk_t timer_high_precision_counter(void)
{
	LARGE_INTEGER cnt;
	QueryPerformanceCounter(&cnt);
	return cnt.QuadPart;
}


#endif

