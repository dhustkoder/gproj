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

	while (events_update()) {
		render_clear(RENDER_CLEAR_ACTORS);

		const uint32_t now = timer_now();
		const float dt = (((float)now) - ((float)lastclk)) / 1000;

		game_step(now, dt);

		render_present();

		++fps;
		if ((timer_now() - clk) >= 1000) {
			LOG("FPS: %d", fps);
			fps = 0;
			clk = timer_now();
		}

		lastclk = now;
	}


	game_term();

	return 0;
}


