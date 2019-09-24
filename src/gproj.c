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
	int fps = 0;
	int currfps = 0;
	timer_hp_clk_t game_step_cnter = 0;
	timer_hp_clk_t game_step_medium = 0;

	while (events_update()) {
		const timer_clk_t now = timer_now();
		const float dt = (((float)now) - ((float)lastclk)) / 1000;

		render_text("FPS: %d", currfps);
		render_text("GAME STEP TOOK: " TIMER_HP_CLK_FMT, game_step_medium);

		const timer_hp_clk_t start = timer_profiler_start();
		game_step(now, dt);
		game_step_cnter += timer_profiler_end(start);

		render_present();

		++fps;
		if ((timer_now() - clk) >= 1000) {
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


