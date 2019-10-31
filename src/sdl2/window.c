#include "logger.h"
#include "render.h"
#include "window.h"


SDL_Window* sdl_window = NULL;


bool window_init(const char* const name)
{
	LOG_DEBUG("INITIALIZING WINDOW");
	int err;
	((void)err);

	assert(name != NULL);

	sdl_window = SDL_CreateWindow(name,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT,
			SDL_WINDOW_RESIZABLE);

	assert(sdl_window != NULL);
}

void window_term(void)
{
	if (sdl_window != NULL)
		SDL_DestroyWindow(sdl_window);

	LOG_DEBUG("TERMINATING WINDOW");
}




