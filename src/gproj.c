#include <SDL2/SDL.h>
#include "log.h"
#include "timer.h"
#include "events.h"
#include "render.h"
#include "gproj.h"
#include "game.h"


int gproj(void)
{
	game_init();

	uint32_t clk = timer_now();
	uint32_t lastclk = clk;
	int fps = 0;
	int currfps = 0;

	while (events_update()) {
		const uint32_t now = timer_now();
		const float dt = (((float)now) - ((float)lastclk)) / TIMER_SECOND;


		game_step(now, dt);

		render_present();

		++fps;
		if ((timer_now() - clk) >= TIMER_SECOND) {
			currfps = fps;
			fps = 0;
			clk = timer_now();
		}

		render_text(RENDER_LAYER_FG, NULL, "FPS: %d", currfps);


		lastclk = now;
	}


	game_term();

	return 0;
}


