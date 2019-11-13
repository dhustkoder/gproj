#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "SDL_FontCache.h"
#include "workman.h"
#include "events.h"
#include "logger.h"
#include "render.h"
#include "audio.h"
#include "render.h"
#include "sdl2_render.h"
#include "ogl_render.h"
#include "gproj.h"



timer_hp_clk_t gproj_timer_hp_frequency;



static bool platform_init(void)
{
	log_dbg("INITIALIZING PLATFORM SDL2");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		goto Lfailure;

	if (IMG_Init(IMG_INIT_PNG) == 0)
		goto Lfailure;

	if (TTF_Init() != 0)
		goto Lfailure;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
		goto Lfailure;

	
	gproj_timer_hp_frequency = SDL_GetPerformanceFrequency();

	return true;

Lfailure:
	log_err("Couldn't initialize platform: %s\n", SDL_GetError());
	return false;
}

static void platform_term(void)
{
	log_dbg("TERMINATING PLATFORM SDL2");

	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}



int main(int argc, char** argv)
{
	SDL_SetMainReady();

	atexit(platform_term);

	if (!platform_init())
		return EXIT_FAILURE;

	return gproj_main(argc, argv);
/*
	workman_init();

	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);
	workman_push_sleep(500);

	workman_work_until_all_finished();
	workman_term();
*/
}


