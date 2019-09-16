#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "log.h"
#include "render.h"
#include "audio.h"
#include "types.h"
#include "render.h"
#include "gproj.h"


extern input_button_t input_buttons_states;

SDL_Renderer* sdl_rend = NULL;
SDL_Texture* sdl_tex_bg = NULL;
SDL_Texture* sdl_tex_actors = NULL;
SDL_Texture* sdl_tex_fg = NULL;
SDL_Texture* sdl_tex_tileset = NULL;

int sfx_cnt = 0;
int bgms_cnt = 0;
Mix_Chunk* sfxs[MAX_SFXS];
Mix_Music* bgms[MAX_BGMS];



static SDL_Window* win = NULL;

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




static bool platform_init(bool vsync)
{
	LOG_DEBUG("Initializing Platfrom");

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK|SDL_INIT_TIMER) != 0) {
		LOG_ERR("Couldn't initialize SDL2: %s", SDL_GetError());
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
		return false;


	IMG_Init(IMG_INIT_PNG);

	win = SDL_CreateWindow("GProj",
	                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                       GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT,
	                       SDL_WINDOW_RESIZABLE);
	if (win == NULL)
		return false;

	sdl_rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED|
	                             (vsync ? SDL_RENDERER_PRESENTVSYNC : 0));
	if (sdl_rend == NULL)
		return false;

	sdl_tex_bg = SDL_CreateTexture(sdl_rend,
	                        SDL_PIXELFORMAT_RGB888,
	                        SDL_TEXTUREACCESS_TARGET,
	                        GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	if (sdl_tex_bg == NULL)
		return false;

	sdl_tex_actors = SDL_CreateTexture(sdl_rend,
	                        SDL_PIXELFORMAT_RGB888,
	                        SDL_TEXTUREACCESS_TARGET,
	                        GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	if (sdl_tex_actors == NULL)
		return false;

	sdl_tex_fg = SDL_CreateTexture(sdl_rend,
	                        SDL_PIXELFORMAT_RGB888,
	                        SDL_TEXTUREACCESS_TARGET,
	                        GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	if (sdl_tex_fg == NULL)
		return false;

	sdl_tex_tileset = IMG_LoadTexture(sdl_rend, "../assets/richter.png");
	if (sdl_tex_tileset == NULL) {
		LOG_ERR("Couldn't load assets: %s", IMG_GetError());
		return false;
	}

	SDL_SetTextureBlendMode(sdl_tex_actors, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(sdl_tex_fg, SDL_BLENDMODE_BLEND);


	render_clear(RENDER_CLEAR_BKG|RENDER_CLEAR_FG|RENDER_CLEAR_ACTORS);
	render_present();

	return true;
}

static void platform_term(void)
{
	LOG_DEBUG("Terminating Platform");

	for (int i = 0; i < bgms_cnt; ++i)
		Mix_FreeMusic(bgms[i]);
	for (int i = 0; i < sfx_cnt; ++i)
		Mix_FreeChunk(sfxs[i]);


	if (sdl_tex_tileset != NULL)
		SDL_DestroyTexture(sdl_tex_tileset);
	if (sdl_tex_fg != NULL)
		SDL_DestroyTexture(sdl_tex_fg);
	if (sdl_tex_actors != NULL)
		SDL_DestroyTexture(sdl_tex_actors);
	if (sdl_tex_bg != NULL)
		SDL_DestroyTexture(sdl_tex_bg);
	if (sdl_rend != NULL)
		SDL_DestroyRenderer(sdl_rend);
	if (win != NULL)
		SDL_DestroyWindow(win);

	IMG_Quit();
	Mix_CloseAudio();
	SDL_Quit();
}

bool events_update(void)
{
	SDL_Event ev;

	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_QUIT: return false;
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


int main(int argc, char** argv)
{
	((void)argv);
	SDL_SetMainReady();

	atexit(platform_term);

	if (!platform_init(argc > 1))
		return EXIT_FAILURE;

	return gproj();
}


