#include "timer.h"
#include "events.h"
#include "gproj.h"
#include "profiler.h"
#include "logger.h"
#include "game.h"


struct events gproj_events;


int gproj_main(int argc, char** argv)
{
	game_init(argc, argv);

	timer_clk_t clk = timer_now();
	timer_clk_t lastclk = clk;

	do {
		const timer_clk_t now = timer_now();
		const float dt = (now - lastclk) / 1000.f;

		events_update(&gproj_events);

		game_step(now, dt);
		
		lastclk = now;
	} while (!(gproj_events.flags&EVENT_FLAG_QUIT));

	game_term();
	return 0;
}


