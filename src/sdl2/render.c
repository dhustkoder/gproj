#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <tmx.h>
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

void render_bkg(const int32_t* gids)
{
	SDL_SetRenderTarget(sdl_rend, sdl_tex_bkg);

	SDL_Rect src, dst;
	for (int l = 0; l < 2; ++l) {
		for (int y = 0; y < GPROJ_Y_TILES; ++y) {
			for (int x = 0; x < GPROJ_X_TILES; ++x) {
				int32_t gid = *gids & TMX_FLIP_BITS_REMOVAL;
				++gids;
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
					.x = x * GPROJ_TILE_WIDTH,
					.y = y * GPROJ_TILE_HEIGHT,
					.w = GPROJ_TILE_WIDTH,
					.h = GPROJ_TILE_HEIGHT
				};
				SDL_RenderCopy(sdl_rend, sdl_tex_tileset, &src, &dst);
			}
		}
	}

	SDL_SetRenderTarget(sdl_rend, NULL);
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
	SDL_RenderCopy(sdl_rend, sdl_tex_fg, NULL, NULL);
	SDL_RenderPresent(sdl_rend);
}
