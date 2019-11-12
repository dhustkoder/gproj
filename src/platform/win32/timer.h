#ifndef GPROJ_TIMER_H_
#define GPROJ_TIMER_H_
#include "platform_defs.h"

typedef double timer_hp_clk_t;
#define TIMER_HP_CLK_FMT ".12lf"

static inline timer_hp_clk_t timer_high_precision_counter_frequency(void)
{
	extern timer_hp_clk_t gproj_win32_hp_freq;
	return gproj_win32_hp_freq;
}

static inline timer_hp_clk_t timer_high_precision_counter(void)
{
	LARGE_INTEGER cnt;
	QueryPerformanceCounter(&cnt);
	return cnt.QuadPart;
}


typedef s32 timer_clk_t;
#define TIMER_CLK_FMT     S32_FMT
#define timer_sleep(ms)   Sleep(ms)


static inline timer_clk_t timer_now(void)
{
	const timer_hp_clk_t now = timer_high_precision_counter();
	const timer_hp_clk_t secs = now / timer_high_precision_counter_frequency();
	return (timer_clk_t) (secs * 1000);
}





#endif
