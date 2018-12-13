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

void render_bkg_tiles(const int32_t* gids)
{
	SDL_SetRenderTarget(sdl_rend, sdl_tex_bkg);

	SDL_Rect src, dst;
	for (int l = 0; l < 2; ++l) {
		for (int y = 0; y < GPROJ_Y_TILES; ++y) {
			for (int x = 0; x < GPROJ_X_TILES; ++x) {
				const int32_t gid = *gids++;
				if (gid == 0)
					continue;
				const int32_t id = (gid&TMX_FLIP_BITS_REMOVAL) - 1;

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

				if ((gid&(~TMX_FLIP_BITS_REMOVAL)) == 0x00) {
					SDL_RenderCopy(sdl_rend, sdl_tex_tileset, &src, &dst);
				} else {
					SDL_RendererFlip flips = SDL_FLIP_NONE;
					double degrees = 0;
					
					if (gid&TMX_FLIPPED_DIAGONALLY) {
						if (gid&TMX_FLIPPED_HORIZONTALLY)
							degrees = -90;
						else
							degrees = 90;
						flips = SDL_FLIP_HORIZONTAL|SDL_FLIP_VERTICAL;
					} else {
						if (gid&TMX_FLIPPED_VERTICALLY)
							flips |= SDL_FLIP_VERTICAL;
						if (gid&TMX_FLIPPED_HORIZONTALLY)
							flips |= SDL_FLIP_HORIZONTAL;
					}
					
					SDL_RenderCopyEx(sdl_rend,
					                 sdl_tex_tileset,
					                 &src, &dst, degrees,
					                 NULL, flips);
				}
			}
		}
	}

	SDL_SetRenderTarget(sdl_rend, NULL);
}

void render_update_bkg_tiles(const int32_t* const gids,
                             const int32_t** const gids_to_update,
                             const int update_len)
{
	SDL_SetRenderTarget(sdl_rend, sdl_tex_bkg);

	SDL_Rect src, dst;
	for (int i = 0; i < update_len; ++i) {
		const int32_t* const gid_ptr = gids_to_update[i];
		const int32_t gid = *gid_ptr;

		if (gid == 0)
			continue;

		const int id = (gid&TMX_FLIP_BITS_REMOVAL) - 1;

		src = (SDL_Rect) {
			.x = (id * GPROJ_TILE_WIDTH) % GPROJ_TILESET_WIDTH,
			.y = (id / (GPROJ_TILESET_WIDTH / GPROJ_TILE_WIDTH)) * GPROJ_TILE_WIDTH,
			.w = 32,
			.h = 32
		};

		int diff = gid_ptr - gids;
		
		if (diff >= (GPROJ_X_TILES * GPROJ_Y_TILES))
			diff -= (GPROJ_X_TILES * GPROJ_Y_TILES);

		dst = (SDL_Rect) {
			.x = (diff % GPROJ_X_TILES) * GPROJ_TILE_WIDTH,
			.y = (diff / GPROJ_X_TILES) * GPROJ_TILE_HEIGHT,
			.w = 32,
			.h = 32
		};

		if ((gid&(~TMX_FLIP_BITS_REMOVAL)) == 0x00) {
			SDL_RenderCopy(sdl_rend, sdl_tex_tileset, &src, &dst);
		} else {
			SDL_RendererFlip flips = SDL_FLIP_NONE;
			double degrees = 0;
			
			if (gid&TMX_FLIPPED_DIAGONALLY) {
				if (gid&TMX_FLIPPED_HORIZONTALLY)
					degrees = -90;
				else
					degrees = 90;
				flips = SDL_FLIP_HORIZONTAL|SDL_FLIP_VERTICAL;
			} else {
				if (gid&TMX_FLIPPED_VERTICALLY)
					flips |= SDL_FLIP_VERTICAL;
				if (gid&TMX_FLIPPED_HORIZONTALLY)
					flips |= SDL_FLIP_HORIZONTAL;
			}
			
			SDL_RenderCopyEx(sdl_rend,
			                 sdl_tex_tileset,
			                 &src, &dst, degrees,
			                 NULL, flips);
		}
	}

	SDL_SetRenderTarget(sdl_rend, NULL);
}

void render_actors(const struct actor* const actors, const int count)
{
	SDL_SetRenderTarget(sdl_rend, sdl_tex_fg);
	SDL_SetRenderDrawColor(sdl_rend, 0xFF, 0xFF, 0xFF, 0xFF);

	SDL_Rect scr, ts;
	for (int i = 0; i < count; ++i) {
		scr = (SDL_Rect) {
			.x = actors[i].scr.pos.x,
			.y = actors[i].scr.pos.y,
			.w = actors[i].scr.size.x,
			.h = actors[i].scr.size.y
		};
		ts = (SDL_Rect) {
			.x = actors[i].ts.pos.x,
			.y = actors[i].ts.pos.y,
			.w = actors[i].ts.size.x,
			.h = actors[i].ts.size.y
		};
		SDL_RenderCopy(sdl_rend, sdl_tex_tileset, &ts, &scr);
	}

	SDL_SetRenderTarget(sdl_rend, NULL);
}

void render_present(void)
{
	SDL_RenderCopy(sdl_rend, sdl_tex_bkg, NULL, NULL);
	SDL_RenderCopy(sdl_rend, sdl_tex_fg, NULL, NULL);
	SDL_RenderPresent(sdl_rend);
}
