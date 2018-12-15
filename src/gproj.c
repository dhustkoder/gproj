#include <SDL2/SDL.h>
#include "log.h"
#include "events.h"
#include "render.h"
#include "map.h"
#include "timer.h"
#include "actors.h"
#include "player.h"
#include "gproj.h"



int gproj(void)
{
	map_load("../assets/test.tmx");

	uint32_t clk = timer_now();
	uint32_t lastclk = clk;
	int fps = 0;


	player_init();

	while (events_update()) {

		render_clear(RENDER_CLEAR_ACTORS);
		

		const uint32_t now = timer_now();
		const float dt = (((float)now) - ((float)lastclk)) / 1000;

		map_update(now);
		player_update(now, dt);
		actors_update(now);
		
		render_present();

		++fps;

		if ((timer_now() - clk) >= 1000) {
			LOG("FPS: %d", fps);
			fps = 0;
			clk = timer_now();
		}

		lastclk = now;

	}

	return 0;
}


