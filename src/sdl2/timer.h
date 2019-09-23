#ifndef GPROJ_TIMER_H_
#define GPROJ_TIMER_H_
#include <SDL2/SDL_timer.h>
#include <stdint.h>


#define TIMER_SECOND ((Uint32)1000)

#define timer_sleep(ms) SDL_Delay(ms)
#define timer_now() SDL_GetTicks()
#define timer_high_precision_counter()   SDL_GetPerformanceCounter()
#define timer_high_precision_frequency() SDL_GetPerformanceFrequency()

#define timer_profiler_start() timer_high_precision_counter()

#define timer_profiler_end(start) \
	(((double)timer_high_precision_counter() - (double)start) / timer_high_precision_frequency())


#endif
