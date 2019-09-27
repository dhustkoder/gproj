#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "SDL_FontCache.h"
#include "threads.h"
#include "logger.h"
#include "render.h"
#include "audio.h"
#include "types.h"
#include "render.h"
#include "gproj.h"


extern input_button_t input_buttons_states;


static SDL_Scancode sdl_keys[] = {
	SDL_SCANCODE_W,
	SDL_SCANCODE_S,
	SDL_SCANCODE_A,
	SDL_SCANCODE_D,
	SDL_SCANCODE_RSHIFT,
	SDL_SCANCODE_SPACE
};

static input_button_t game_buttons[] = {
	INPUT_BUTTON_UP,
	INPUT_BUTTON_DOWN,
	INPUT_BUTTON_LEFT,
	INPUT_BUTTON_RIGHT,
	INPUT_BUTTON_RUN,
	INPUT_BUTTON_ACTION
};



static bool platform_init()
{
	LOG_DEBUG("Initializing Platfrom");

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
		goto Lfailure;

	if (IMG_Init(IMG_INIT_PNG) == 0)
		goto Lfailure;

	if (TTF_Init() != 0)
		goto Lfailure;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
		goto Lfailure;

	return true;

Lfailure:
	LOG_ERR("Couldn't initialize platform: %s\n", SDL_GetError());
	return false;
}

static void platform_term(void)
{
	LOG_DEBUG("Terminating Platform");

	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


volatile bool game_exit = false;


bool thr_events_update(void)
{
	SDL_Event ev;

	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_QUIT:
			game_exit = true;
			return false;
		case SDL_KEYDOWN:
		case SDL_KEYUP: {
			for (int idx = 0; idx < INPUT_BUTTON_NBUTTONS; ++idx) {
				if (sdl_keys[idx] == ev.key.keysym.scancode) {
					input_button_t but = game_buttons[idx];
					if (ev.type == SDL_KEYDOWN)
						input_buttons_states |= but;
					else
						input_buttons_states &= ~but;
					break;
				}
			}

		}

		}
	}

	return true;
}

bool events_update()
{
	return !game_exit;
}

static int game_thread_dispatcher(void* p)
{
	SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);
	void** parr = ((void**)p);
	int argc = *(int*)(parr[0]);
	char** argv = (char**)(parr[1]);
	LOG("ARGV[0]: %s ", argv[0]);
	return gproj(argc, argv);
}

int main(int argc, char** argv)
{
	SDL_SetMainReady();

	atexit(platform_term);

	if (!platform_init())
		return EXIT_FAILURE;


	void* args[2] = { (void*) &argc, (void*) argv };
	gproj_thread_t* gproj_thr = thread_start(game_thread_dispatcher,
	                                         "GPROJ_THREAD",
	                                         args);


	extern void render_worker();
	render_worker();

	return thread_join(gproj_thr);
}


