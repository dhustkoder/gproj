#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "render.h"
#include "log.h"

extern SDL_Renderer* sdl_rend;
extern SDL_Texture* sdl_tex_bkg;
extern SDL_Texture* sdl_tex_sprs;


void render_clear(const uint8_t flags)
{
	if (flags&RENDER_CLEAR_BKG) {
		SDL_SetRenderTarget(sdl_rend, sdl_tex_bkg);
		SDL_SetRenderDrawColor(sdl_rend, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(sdl_rend);
	}

	if (flags&RENDER_CLEAR_SPRS) {
		SDL_SetRenderTarget(sdl_rend, sdl_tex_sprs);
		SDL_SetRenderDrawColor(sdl_rend, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(sdl_rend);
	}

	SDL_SetRenderTarget(sdl_rend, NULL);
}

void render_bkg(const uint32_t* const pixels)
{
	SDL_UpdateTexture(sdl_tex_bkg, NULL, pixels, 4 * GPROJ_FB_WIDTH);
}

void render_sprs(const struct sprite* const sprs, const int count)
{
	if (SDL_SetRenderTarget(sdl_rend, sdl_tex_sprs) != 0) {
		LOG_ERR("Could'nt set render target: %s\n", SDL_GetError());
	}
	SDL_SetRenderDrawColor(sdl_rend, 0xFF, 0xFF, 0xFF, 0xFF);

	SDL_Rect rects[count];
	for (int i = 0; i < count; ++i) {
		rects[i] = (SDL_Rect) {
			.x = sprs[i].pos.x,
			.y = sprs[i].pos.y,
			.w = sprs[i].size.x,
			.h = sprs[i].size.y
		};
	}

	SDL_RenderFillRects(sdl_rend, rects, count);

	SDL_SetRenderTarget(sdl_rend, NULL);
}

void render_present(void)
{
	SDL_RenderCopy(sdl_rend, sdl_tex_bkg, NULL, NULL);
	SDL_RenderCopy(sdl_rend, sdl_tex_sprs, NULL, NULL);
	SDL_RenderPresent(sdl_rend);
}
