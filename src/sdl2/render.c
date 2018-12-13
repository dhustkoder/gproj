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

void render_bkg_tiles(const int32_t* tile_ids)
{
	SDL_SetRenderTarget(sdl_rend, sdl_tex_bkg);

	SDL_Rect src, dst;
	for (int l = 0; l < 2; ++l) {
		for (int y = 0; y < GPROJ_Y_TILES; ++y) {
			for (int x = 0; x < GPROJ_X_TILES; ++x) {
				const int32_t id = *tile_ids++;
				if (id < 0)
					continue;

				src = (SDL_Rect) {
					.x = (id * GPROJ_TILE_WIDTH) % GPROJ_TILESET_WIDTH,
					.y = (id / (GPROJ_TILESET_WIDTH / GPROJ_TILE_WIDTH)) * GPROJ_TILE_WIDTH,
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

void render_update_bkg_tiles(const int32_t* const tile_ids,
                             const int32_t** const ids_to_update,
                             const int update_len)
{
	SDL_SetRenderTarget(sdl_rend, sdl_tex_bkg);

	SDL_Rect src, dst;
	for (int i = 0; i < update_len; ++i) {
		const int32_t* const idptr = ids_to_update[i];

		if (*idptr < 0)
			continue;

		src = (SDL_Rect) {
			.x = (*idptr * GPROJ_TILE_WIDTH) % GPROJ_TILESET_WIDTH,
			.y = (*idptr / (GPROJ_TILESET_WIDTH / GPROJ_TILE_WIDTH)) * GPROJ_TILE_WIDTH,
			.w = 32,
			.h = 32
		};

		int diff = idptr - tile_ids;
		
		if (diff >= (GPROJ_X_TILES * GPROJ_Y_TILES))
			diff -= (GPROJ_X_TILES * GPROJ_Y_TILES);

		dst = (SDL_Rect) {
			.x = (diff % GPROJ_X_TILES) * GPROJ_TILE_WIDTH,
			.y = (diff / GPROJ_X_TILES) * GPROJ_TILE_HEIGHT,
			.w = 32,
			.h = 32
		};

		SDL_RenderCopy(sdl_rend, sdl_tex_tileset, &src, &dst);
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
