#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <tmx.h>
#include "log.h"
#include "render.h"
#include "types.h"
#include "render.h"
#include "gproj.h"


extern input_button_t input_buttons_states;


SDL_Renderer* sdl_rend;
SDL_Texture* sdl_tex_bkg;
SDL_Texture* sdl_tex_fg;
SDL_Texture* sdl_tex_tileset;

static SDL_Window* win;

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
	INPUT_BUTTON_SHOOT
};




static bool platform_init(bool vsync)
{
	LOG_DEBUG("Initializing Platfrom");
	
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK|SDL_INIT_TIMER) != 0) {
		LOG_ERR("Couldn't initialize SDL2: %s", SDL_GetError());
		return false;
	}

	IMG_Init(IMG_INIT_PNG);

	win = SDL_CreateWindow("GProj",
	                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                       GPROJ_FB_WIDTH, GPROJ_FB_HEIGHT,
	                       SDL_WINDOW_RESIZABLE);
	if (win == NULL)
		return false;

	sdl_rend = SDL_CreateRenderer(win, -1,
	                             (vsync ? SDL_RENDERER_PRESENTVSYNC : 0));
	if (sdl_rend == NULL)
		return false;

	sdl_tex_bkg = SDL_CreateTexture(sdl_rend,
	                        SDL_PIXELFORMAT_RGB888,
	                        SDL_TEXTUREACCESS_TARGET,
	                        GPROJ_FB_WIDTH, GPROJ_FB_HEIGHT);
	if (sdl_tex_bkg == NULL)
		return false;

	sdl_tex_fg = SDL_CreateTexture(sdl_rend,
	                        SDL_PIXELFORMAT_RGB888,
	                        SDL_TEXTUREACCESS_TARGET,
	                        GPROJ_FB_WIDTH, GPROJ_FB_HEIGHT);
	if (sdl_tex_fg == NULL)
		return false;

	sdl_tex_tileset = IMG_LoadTexture(sdl_rend, "../assets/tileset.png");
	if (sdl_tex_tileset == NULL)
		return false;


	SDL_SetTextureBlendMode(sdl_tex_fg, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(sdl_tex_tileset, SDL_BLENDMODE_BLEND);

	return true;
}

static void platform_term(void)
{
	LOG_DEBUG("Terminating Platform");

	if (sdl_tex_tileset != NULL)
		SDL_DestroyTexture(sdl_tex_tileset);
	if (sdl_tex_fg != NULL)
		SDL_DestroyTexture(sdl_tex_fg);
	if (sdl_tex_bkg != NULL)
		SDL_DestroyTexture(sdl_tex_bkg);
	if (sdl_rend != NULL)
		SDL_DestroyRenderer(sdl_rend);
	if (win != NULL)
		SDL_DestroyWindow(win);
	
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


	tmx_map* map = tmx_load("../assets/test.tmx");

	if (map == NULL) {
		LOG_ERR("Couldn't load map: %s", tmx_strerr());
		return EXIT_FAILURE;
	}

	tmx_layer* layp = map->ly_head;

	render_clear(RENDER_CLEAR_BKG|RENDER_CLEAR_FG);

	SDL_SetRenderTarget(sdl_rend, sdl_tex_bkg);
	
	SDL_Rect src, dst;

	while (layp != NULL) {
		for (int i = 0; i < GPROJ_Y_TILES; ++i) {
			for (int j = 0; j < GPROJ_X_TILES; ++j) {
				int32_t gid = (layp->content.gids[i * GPROJ_TILE_WIDTH + j] & TMX_FLIP_BITS_REMOVAL);
				if (gid == 0)
					continue;
				--gid;
				src = (SDL_Rect) {
					.x = (gid * GPROJ_TILE_WIDTH) % GPROJ_TILESET_WIDTH,
					.y = (gid / (GPROJ_TILESET_WIDTH / GPROJ_TILE_WIDTH)) * GPROJ_TILE_WIDTH,
					.w = GPROJ_TILE_WIDTH,
					.h = GPROJ_TILE_HEIGHT
				};
				dst = (SDL_Rect) {
					.x = j * GPROJ_TILE_WIDTH,
					.y = i * GPROJ_TILE_HEIGHT,
					.w = GPROJ_TILE_WIDTH,
					.h = GPROJ_TILE_HEIGHT
				};
				SDL_RenderCopy(sdl_rend, sdl_tex_tileset, &src, &dst);
			}
		}
		layp = layp->next;
	}

	SDL_SetRenderTarget(sdl_rend, NULL);


	tmx_map_free(map);


	return gproj();
}


