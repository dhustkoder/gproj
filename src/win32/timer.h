#ifndef GPROJ_TIMER_H_
#define GPROJ_TIMER_H_
#include "platform_defs.h"


typedef DWORD timer_clk_t;
#define TIMER_CLK_FMT     U32_FMT
#define timer_sleep(ms)   Sleep(ms)
#define timer_now()       GetTickCount()


typedef double timer_hp_clk_t;
timer_hp_clk_t timer_hp_frequency;
#define TIMER_HP_CLK_FMT ".12lf"


#define timer_high_precision_counter()   ((timer_hp_clk_t)0)






#endif
