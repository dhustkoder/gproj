#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "render.h"
#include "log.h"

extern SDL_Renderer* sdl_rend;
extern SDL_Texture* sdl_tex_bkg;
extern SDL_Texture* sdl_tex_sprs;

static SDL_Texture* curr_spr_sheet;

sprite_sheet_t* render_load_spr_sheet(const char* name)
{
	SDL_Surface* const img = IMG_Load(name);
	SDL_Texture* const tex = SDL_CreateTextureFromSurface(sdl_rend, img);
	return tex;
}

void render_free_spr_sheet(sprite_sheet_t* const ss)
{
	SDL_DestroyTexture(ss);
}

void render_set_spr_sheet(sprite_sheet_t* const ss)
{
	curr_spr_sheet = ss;
}

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
	SDL_SetRenderTarget(sdl_rend, sdl_tex_sprs);
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
		SDL_RenderCopy(sdl_rend, curr_spr_sheet, &ss, &scr);
	}

	SDL_SetRenderTarget(sdl_rend, NULL);
}

void render_present(void)
{
	SDL_RenderCopy(sdl_rend, sdl_tex_bkg, NULL, NULL);
	SDL_RenderCopy(sdl_rend, sdl_tex_sprs, NULL, NULL);
	SDL_RenderPresent(sdl_rend);
}
