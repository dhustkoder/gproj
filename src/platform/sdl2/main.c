#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "SDL_FontCache.h"
#include "workman.h"
#include "input.h"
#include "logger.h"
#include "render.h"
#include "audio.h"
#include "render.h"
#include "sdl2_render.h"
#include "ogl_render.h"
#include "game.h"



timer_hp_clk_t gproj_timer_hp_frequency;



static SDL_Scancode sdl_keys[] = {
	SDL_SCANCODE_W,
	SDL_SCANCODE_S,
	SDL_SCANCODE_A,
	SDL_SCANCODE_D

#ifdef GPROJ_DEBUG
	,
	SDL_SCANCODE_K,
	SDL_SCANCODE_J,
	SDL_SCANCODE_H,
	SDL_SCANCODE_L,
	SDL_SCANCODE_M,
	SDL_SCANCODE_N,
	SDL_SCANCODE_B
#endif
};

static input_t gproj_buttons[] = {
	INPUT_BUTTON_UP,
	INPUT_BUTTON_DOWN,
	INPUT_BUTTON_LEFT,
	INPUT_BUTTON_RIGHT

#ifdef GPROJ_DEBUG
	,
	INPUT_BUTTON_CAM_UP,
	INPUT_BUTTON_CAM_DOWN,
	INPUT_BUTTON_CAM_LEFT,
	INPUT_BUTTON_CAM_RIGHT,
	INPUT_BUTTON_WORLD_SCALE_UP,
	INPUT_BUTTON_WORLD_SCALE_DOWN,
	INPUT_BUTTON_WORLD_FWD
#endif
};

static input_t ginput;


static void platform_init(void)
{
	log_dbg("INITIALIZING PLATFORM SDL2");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		INVALID_CODE_PATH;

	if (IMG_Init(IMG_INIT_PNG) == 0)
		INVALID_CODE_PATH;

	if (TTF_Init() != 0)
		INVALID_CODE_PATH;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
		INVALID_CODE_PATH;

	
	gproj_timer_hp_frequency = SDL_GetPerformanceFrequency();

	return true;
}

static void platform_term(void)
{
	log_dbg("TERMINATING PLATFORM SDL2");

	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}




static inline void update_keys(const SDL_Event* const ev)
{
	input_t buttons = ginput;

	for (int idx = 0; idx < INPUT_BUTTON_NBUTTONS; ++idx) {
		if (sdl_keys[idx] == ev->key.keysym.scancode) {
			input_t bt = gproj_buttons[idx];
			if (ev->type == SDL_KEYDOWN)
				buttons |= bt;
			else
				buttons &= ~bt;
			break;
		}
	}

	INPUT_SET_NEW_VALUE(ginput, buttons);
}


b32 update_events(void)
{
	SDL_Event ev;

	INPUT_UPDATE_OLD_VALUE(ginput);

	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				update_keys(&ev);
				break;
			case SDL_WINDOWEVENT:
				// implement
				break;
		}
	}

	return true;
}



int main(int argc, char** argv)
{
	SDL_SetMainReady();

	platform_init();
	game_init();


	timer_clk_t clk = timer_now();
	timer_clk_t lastclk = clk;

	while (update_events()) {
		const timer_clk_t now = timer_now();
		const float dt = (now - lastclk) / 1000.f;

		game_step(ginput, now, dt);
		
		lastclk = now;
	};




	game_term();
	platform_term();
}


