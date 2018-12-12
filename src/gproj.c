#include <SDL2/SDL.h>
#include "log.h"
#include "events.h"
#include "render.h"
#include "map.h"
#include "gproj.h"


input_button_t input_buttons_states;


int gproj(void)
{
	map_load("../assets/test.tmx");


	while (events_update()) {
		Uint32 clk = SDL_GetTicks();

		render_clear(RENDER_CLEAR_FG);
		
		map_update();
		
		render_present();

		LOG("Frames Per Second: %u", (SDL_GetTicks() - clk));
	}

	return 0;
}


