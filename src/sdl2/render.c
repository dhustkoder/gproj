#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "render.h"
#include "log.h"

extern SDL_Renderer* sdl_rend;
extern SDL_Texture* sdl_tex_bkg;
extern SDL_Texture* sdl_tex_fg;
extern SDL_Texture* sdl_tex_tileset;


void render_clear(const uint8_t flags)
{
	if (flags&RENDER_CLEAR_BKG) {
		SDL_SetRenderTarget(sdl_rend, sdl_tex_bkg);
		SDL_SetRenderDrawColor(sdl_rend, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(sdl_rend);
	}

	if (flags&RENDER_CLEAR_FG) {
		SDL_SetRenderTarget(sdl_rend, sdl_tex_fg);
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
	SDL_SetRenderTarget(sdl_rend, sdl_tex_fg);
	SDL_SetRenderDrawColor(sdl_rend, 0xFF, 0xFF, 0xFF, 0xFF);

	SDL_Rect scr, ss;
	for (int i = 0; i < count; ++i) {
		scr = (SDL_Rect) {
			.x = sprs[i].scr.pos.x,
			.y = sprs[i].scr.pos.y,
			.w = sprs[i].scr.size.x,
			.h = sprs[i].scr.size.y
		};
		ss = (SDL_Rect) {
			.x = sprs[i].ss.pos.x,
			.y = sprs[i].ss.pos.y,
			.w = sprs[i].ss.size.x,
			.h = sprs[i].ss.size.y
		};
		SDL_RenderCopy(sdl_rend, sdl_tex_tileset, &ss, &scr);
	}

	SDL_SetRenderTarget(sdl_rend, NULL);
}

void render_present(void)
{
	SDL_RenderCopy(sdl_rend, sdl_tex_bkg, NULL, NULL);
	SDL_RenderPresent(sdl_rend);
}
