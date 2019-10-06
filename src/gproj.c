#include <SDL2/SDL.h>
#include "logger.h"
#include "timer.h"
#include "events.h"
#include "render.h"
#include "gproj.h"
#include "game.h"


int gproj(int argc, char** argv)
{
	game_init(argc, argv);

	timer_clk_t clk = timer_now();
	timer_clk_t lastclk = clk;

	while (events_update()) {
		const timer_clk_t now = timer_now();
		const float dt = (((float)now) - ((float)lastclk)) / 1000.f;

		game_step(now, dt);

		lastclk = now;
	}


	game_term();

	return 0;
}


