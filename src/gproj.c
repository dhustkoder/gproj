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

	while (events_update()) {
		const timer_clk_t now = timer_now();
		const float dt = (((float)now) - ((float)lastclk)) / 1000;

		render_text("FPS: %d", currfps);
		game_step(now, dt);
		render_present();

		++fps;
		if ((timer_now() - clk) >= 1000) {
			currfps = fps;
			fps = 0;
			clk = timer_now();
		}

		lastclk = now;
	}


	game_term();

	return 0;
}


