#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "log.h"
#include "gproj.h"


static bool platform_init(void)
{
	LOG_DEBUG("Initializing Platfrom\n");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
		LOG_ERR("Couldn't initialize SDL2: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

static void platform_term(void)
{
	LOG_DEBUG("Terminating Platform\n");
	SDL_Quit();
}


int main(int argc, char** argv)
{
	if (!platform_init())
		return EXIT_FAILURE;
	
	atexit(platform_term);

	return gproj(argc, argv);
}
