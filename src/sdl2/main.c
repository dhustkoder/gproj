#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
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
SDL_Texture* sdl_tex_ts = NULL;
SDL_Texture* sdl_tex_ss = NULL;
SDL_Texture* sdl_tex_txt = NULL;

int sfxs_cnt = 0;
int bgms_cnt = 0;
Mix_Chunk* sfxs[MAX_SFXS];
Mix_Music* bgms[MAX_BGMS];


TTF_Font* ttf_font = NULL;


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

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER) != 0)
		goto Lfailure;

	if (IMG_Init(IMG_INIT_PNG) == 0)
		goto Lfailure;

	if (TTF_Init() != 0)
		goto Lfailure;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
		goto Lfailure;

	win = SDL_CreateWindow("GProj",
	                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                       GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT,
	                       SDL_WINDOW_RESIZABLE);
	if (win == NULL)
		goto Lfailure;

	sdl_rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED|
	                             (vsync ? SDL_RENDERER_PRESENTVSYNC : 0));
	if (sdl_rend == NULL)
		goto Lfailure;

	sdl_tex_bg = SDL_CreateTexture(sdl_rend,
	                        SDL_PIXELFORMAT_RGB888,
	                        SDL_TEXTUREACCESS_TARGET,
	                        GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	if (sdl_tex_bg == NULL)
		goto Lfailure;

	sdl_tex_actors = SDL_CreateTexture(sdl_rend,
	                        SDL_PIXELFORMAT_RGB888,
	                        SDL_TEXTUREACCESS_TARGET,
	                        GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	if (sdl_tex_actors == NULL)
		goto Lfailure;

	sdl_tex_fg = SDL_CreateTexture(sdl_rend,
	                        SDL_PIXELFORMAT_RGB888,
	                        SDL_TEXTUREACCESS_TARGET,
	                        GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	if (sdl_tex_fg == NULL)
		goto Lfailure;

	sdl_tex_txt = SDL_CreateTexture(sdl_rend,
	                        SDL_PIXELFORMAT_RGB888,
	                        SDL_TEXTUREACCESS_TARGET,
	                        GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	if (sdl_tex_txt == NULL)
		goto Lfailure;


	ttf_font = TTF_OpenFont("8bit-madness.ttf", 32);
	if (ttf_font == NULL)
		goto Lfailure;


	SDL_SetTextureBlendMode(sdl_tex_actors, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(sdl_tex_fg, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(sdl_tex_txt, SDL_BLENDMODE_BLEND);

	render_clear(RENDER_CLEAR_BKG|RENDER_CLEAR_FG|RENDER_CLEAR_ACTORS);
	render_present();

	return true;

Lfailure:
	LOG_ERR("Couldn't initialize platform: %s\n", SDL_GetError());
	return false;
}

static void platform_term(void)
{
	LOG_DEBUG("Terminating Platform");

	for (int i = 0; i < bgms_cnt; ++i)
		Mix_FreeMusic(bgms[i]);
	for (int i = 0; i < sfxs_cnt; ++i)
		Mix_FreeChunk(sfxs[i]);

	TTF_CloseFont(ttf_font);

	if (sdl_tex_txt != NULL)
		SDL_DestroyTexture(sdl_tex_txt);
	if (sdl_tex_ts != NULL)
		SDL_DestroyTexture(sdl_tex_ts);
	if (sdl_tex_ss != NULL)
		SDL_DestroyTexture(sdl_tex_ss);
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

	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
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


