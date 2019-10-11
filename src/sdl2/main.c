#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "SDL_FontCache.h"
#include "workman.h"
#include "logger.h"
#include "render.h"
#include "audio.h"
#include "types.h"
#include "render.h"
#include "gproj.h"


static SDL_Scancode sdl_keys[] = {
	SDL_SCANCODE_W,
	SDL_SCANCODE_S,
	SDL_SCANCODE_A,
	SDL_SCANCODE_D,
	SDL_SCANCODE_RSHIFT,
	SDL_SCANCODE_SPACE

#ifdef GPROJ_DEBUG
	,
	SDL_SCANCODE_K,
	SDL_SCANCODE_J,
	SDL_SCANCODE_H,
	SDL_SCANCODE_L,
	SDL_SCANCODE_M,
	SDL_SCANCODE_N
#endif
};

static input_button_t gproj_buttons[] = {
	INPUT_BUTTON_UP,
	INPUT_BUTTON_DOWN,
	INPUT_BUTTON_LEFT,
	INPUT_BUTTON_RIGHT,
	INPUT_BUTTON_RUN,
	INPUT_BUTTON_ACTION

#ifdef GPROJ_DEBUG
	,
	INPUT_BUTTON_CAM_UP,
	INPUT_BUTTON_CAM_DOWN,
	INPUT_BUTTON_CAM_LEFT,
	INPUT_BUTTON_CAM_RIGHT,
	INPUT_BUTTON_WORLD_SCALE_UP,
	INPUT_BUTTON_WORLD_SCALE_DOWN
#endif
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

static inline void update_input_event(const SDL_Event* const ev,
                                      struct input_event* const ie)
{
	input_button_t buttons = ie->buttons;
	for (int idx = 0; idx < INPUT_BUTTON_NBUTTONS; ++idx) {
		if (sdl_keys[idx] == ev->key.keysym.scancode) {
			input_button_t bt = gproj_buttons[idx];
			if (ev->type == SDL_KEYDOWN)
				buttons |= bt;
			else
				buttons &= ~bt;
			break;
		}
	}
	if (buttons != ie->buttons) {
		ie->buttons = buttons;
		ie->new_state = true;
	}
}



void events_update(struct events* const gproj_ev)
{
	SDL_Event ev;
	
	gproj_ev->input.new_state = false;
	gproj_ev->quit = false;

	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
			case SDL_QUIT: 
				gproj_ev->quit = true;
				return;
			case SDL_KEYDOWN:
			case SDL_KEYUP: 
				update_input_event(&ev, &gproj_ev->input);
				break;
		}
	}
}




int main(int argc, char** argv)
{
	SDL_SetMainReady();

	atexit(platform_term);

	if (!platform_init())
		return EXIT_FAILURE;

	return gproj(argc, argv);
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


