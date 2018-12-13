#include <SDL2/SDL.h>
#include "log.h"
#include "events.h"
#include "render.h"
#include "map.h"
#include "timer.h"
#include "actors.h"
#include "gproj.h"



int gproj(void)
{
	map_load("../assets/test.tmx");



	actors_add(&(struct actor) {
		.type = ACTOR_TYPE_PLAYER,
		.ts = { .size = { 16, 16 }, .pos = { 526, 3040 }},
		.scr = { .size = { 32, 32}, .pos = { GPROJ_FB_WIDTH / 2, GPROJ_FB_HEIGHT / 2}}
	});

	uint32_t clk = timer_now();
	int fps = 0;

	while (events_update()) {

		render_clear(RENDER_CLEAR_FG);
		
		map_update();
		actors_update();

		render_present();

		++fps;

		if ((timer_now() - clk) >= 1000) {
			LOG("FPS: %d", fps);
			fps = 0;
			clk = timer_now();
		}

	}

	return 0;
}


