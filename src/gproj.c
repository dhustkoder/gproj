#include <SDL2/SDL.h>
#include "log.h"
#include "events.h"
#include "render.h"
#include "map.h"
#include "timer.h"
#include "gproj.h"


input_button_t input_buttons_states;


int gproj(void)
{
	map_load("../assets/test.tmx");

	while (events_update()) {
		uint32_t clk = timer_now();

		render_clear(RENDER_CLEAR_FG);
		
		map_update();
		
		render_present();

		LOG("Frames Per Second: %u", (timer_now() - clk));
	}

	return 0;
}


