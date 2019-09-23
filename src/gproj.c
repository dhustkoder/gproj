#include <SDL2/SDL.h>
#include "log.h"
#include "timer.h"
#include "events.h"
#include "render.h"
#include "gproj.h"
#include "game.h"


int gproj(int argc, char** argv)
{
	game_init(argc, argv);

	uint32_t clk = timer_now();
	uint32_t lastclk = clk;
	int fps = 0;
	int currfps = 0;
	double game_step_cnter = 0;
	double game_step_medium = 0;

	while (events_update()) {
		const uint32_t now = timer_now();
		const float dt = (((float)now) - ((float)lastclk)) / TIMER_SECOND;

		render_text("FPS: %d", currfps);
		render_text("GAME STEP TOOK: %.9lf", game_step_medium);

		const double start = timer_profiler_start();
		game_step(now, dt);
		game_step_cnter += timer_profiler_end(start);

		render_present();

		++fps;
		if ((timer_now() - clk) >= TIMER_SECOND) {
			game_step_medium = game_step_cnter / fps;
			game_step_cnter = 0;
			currfps = fps;
			fps = 0;
			clk = timer_now();
		}

		lastclk = now;
	}


	game_term();

	return 0;
}


