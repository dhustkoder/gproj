#include <SDL2/SDL.h>
#include "log.h"
#include "events.h"
#include "render.h"
#include "audio.h"
#include "map.h"
#include "timer.h"
#include "actors.h"
#include "characters.h"
#include "gproj.h"



int gproj(void)
{
	characters_init();
	map_load("../assets/map01.tmx");
	audio_play_bgm(audio_load_bgm("../assets/bloodlines.ogg"));


	uint32_t clk = timer_now();
	uint32_t lastclk = clk;
	int fps = 0;

	while (events_update()) {
		render_clear(RENDER_CLEAR_ACTORS);

		const uint32_t now = timer_now();
		const float dt = (((float)now) - ((float)lastclk)) / 1000;

		characters_update(now, dt);
		map_update(now);
		actors_update(now, dt);

		render_present();

		++fps;
		if ((timer_now() - clk) >= 1000) {
			LOG("FPS: %d", fps);
			fps = 0;
			clk = timer_now();
		}

		lastclk = now;
	}


	map_free();

	return 0;
}


