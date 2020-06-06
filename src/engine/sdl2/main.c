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
	log_dbg("Button Pressed: %d", ev->key.keysym.scancode);
}


b32 update_events(void)
{
	SDL_Event ev;

	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				update_keys(&ev);
				break;
			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_RESIZED) 
					render_resize(ev.window.data1, ev.window.data2);
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

		game_step(0, now, dt);
		
		lastclk = now;
	};


	game_term();
	platform_term();
	return EXIT_SUCCESS;
}


