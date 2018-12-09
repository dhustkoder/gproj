#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "log.h"
#include "input.h"
#include "render.h"
#include "gproj.h"


input_button_t input_buttons_states;

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

static SDL_Window* win;
static SDL_Renderer* rend;
static SDL_Texture* tex;


static bool platform_init(void)
{
	LOG_DEBUG("Initializing Platfrom\n");
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
		LOG_ERR("Couldn't initialize SDL2: %s\n", SDL_GetError());
		return false;
	}

	win = SDL_CreateWindow("GProj",
	                       SDL_WINDOWPOS_CENTERED,
	                       SDL_WINDOWPOS_CENTERED,
	                       1260, 720, 0);
	if (win == NULL)
		return false;

	rend = SDL_CreateRenderer(win,
	                          -1,
	                          SDL_RENDERER_ACCELERATED|
	                          SDL_RENDERER_PRESENTVSYNC);
	if (rend == NULL)
		return false;

	tex = SDL_CreateTexture(rend,
	                        SDL_PIXELFORMAT_RGB888,
	                        SDL_TEXTUREACCESS_STREAMING,
	                        1260, 720);
	if (tex == NULL)
		return false;

	return true;
}

static void platform_term(void)
{
	LOG_DEBUG("Terminating Platform\n");

	if (tex != NULL)
		SDL_DestroyTexture(tex);
	if (rend != NULL)
		SDL_DestroyRenderer(rend);
	if (win != NULL)
		SDL_DestroyWindow(win);
	
	SDL_Quit();
}

bool input_bump_events(void)
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


void render_begin(void)
{
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 0xFF);
	SDL_RenderClear(rend);
}

void render_end(void)
{
	SDL_RenderPresent(rend);
}

void render_draw_sprites(const struct sprite* const spr, const int count)
{
	for (int i = 0; i < count; ++i) {
		SDL_SetRenderDrawColor(rend, spr[i].r, spr[i].g, spr[i].b, 0xFF);
		SDL_RenderFillRect(rend, &(SDL_Rect){
			.x = spr[i].x,
			.y = spr[i].y,
			.w = spr[i].w,
			.h = spr[i].h
		});
	}
}

int main(int argc, char** argv)
{
	atexit(platform_term);

	if (!platform_init())
		return EXIT_FAILURE;

	return gproj(argc, argv);
}
