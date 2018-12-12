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
		render_clear(RENDER_CLEAR_FG);
		render_present();
	}

	return 0;
}


