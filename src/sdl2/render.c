#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL_FontCache.h"
#include <tmx.h>
#include "render.h"
#include "log.h"
#include "map.h"

extern SDL_Renderer* sdl_rend;
extern SDL_Texture* sdl_tex_bg;
extern SDL_Texture* sdl_tex_fg;
extern SDL_Texture* sdl_tex_actors;
extern SDL_Texture* sdl_tex_ts;
extern SDL_Texture* sdl_tex_ss;
extern FC_Font* sdl_font;

extern const enum render_layer layers_arr[RENDER_LAYER_NLAYERS];
extern SDL_Texture* tex_targets_arr[RENDER_LAYER_NLAYERS];

static enum render_layer layers_cleared;



static void draw_flipped_gid(const int32_t gid,
                             const SDL_Rect* const src,
                             const SDL_Rect* const dst)
{
	SDL_RendererFlip flips = SDL_FLIP_NONE;
	double degrees = 0;

	if (gid&TMX_FLIPPED_DIAGONALLY) {
		degrees = 90;
		flips |= SDL_FLIP_VERTICAL;
		if (gid&TMX_FLIPPED_VERTICALLY)
			flips |= SDL_FLIP_HORIZONTAL;
		if (gid&TMX_FLIPPED_HORIZONTALLY)
			flips &= ~SDL_FLIP_VERTICAL;
	} else {
		if (gid&TMX_FLIPPED_VERTICALLY)
			flips |= SDL_FLIP_VERTICAL;

		if (gid&TMX_FLIPPED_HORIZONTALLY)
			flips |= SDL_FLIP_HORIZONTAL;
	}

	SDL_RenderCopyEx(sdl_rend,
	                 sdl_tex_ts,
	                 src, dst, degrees,
	                 NULL, flips);
}

static void draw_tile_layers(const int32_t* gids)
{
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

				if ((gid&(~TMX_FLIP_BITS_REMOVAL)) == 0x00)
					SDL_RenderCopy(sdl_rend, sdl_tex_ts, &src, &dst);
				else
					draw_flipped_gid(gid, &src, &dst);
			}
		}
	}
}


void render_set_ts(const char* const path)
{
	if (sdl_tex_ts)
		SDL_DestroyTexture(sdl_tex_ts);

	LOG_DEBUG("LOADING TILESHEET: %s", path);
	sdl_tex_ts = IMG_LoadTexture(sdl_rend, path);

	assert(sdl_tex_ts != NULL);
}

void render_set_ss(const char* path)
{
	if (sdl_tex_ss)
		SDL_DestroyTexture(sdl_tex_ss);

	LOG_DEBUG("LOADING SPRITESHEET: %s", path);
	sdl_tex_ss = IMG_LoadTexture(sdl_rend, path);

	assert(sdl_tex_ss != NULL);
}


void render_clear(const enum render_layer layers)
{
	assert(layers != 0);

	for (size_t i = 0; i < ARRSZ(layers_arr); ++i) {
		if (layers&layers_arr[i] && !(layers_arr[i]&layers_cleared)) {
			SDL_SetRenderTarget(sdl_rend, tex_targets_arr[i]);
			SDL_RenderClear(sdl_rend);
		}
	}

	layers_cleared |= layers;
}


void render_tile_layers(const int32_t* const gids)
{
	render_clear(RENDER_LAYER_BG|RENDER_LAYER_FG);
	SDL_SetRenderTarget(sdl_rend, sdl_tex_bg);
	draw_tile_layers(gids);
	SDL_SetRenderTarget(sdl_rend, sdl_tex_fg);
	draw_tile_layers(gids + MAP_LAYER_FG * GPROJ_X_TILES * GPROJ_Y_TILES);
	SDL_SetRenderTarget(sdl_rend, NULL);
}


void render_update_tile_layers(const int32_t* const gids,
                               const int32_t** const gids_to_update,
                               const int update_len)
{
	render_clear(RENDER_LAYER_BG|RENDER_LAYER_FG);

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

		const uintptr_t layer_idx = (gid_ptr - gids) / (GPROJ_X_TILES * GPROJ_Y_TILES);
		const uintptr_t diff = (gid_ptr - gids) - (layer_idx * GPROJ_X_TILES * GPROJ_Y_TILES);
		if (layer_idx < MAP_LAYER_FG) {
			SDL_SetRenderTarget(sdl_rend, sdl_tex_bg);
		} else {
			SDL_SetRenderTarget(sdl_rend, sdl_tex_fg);
		}

		dst = (SDL_Rect) {
			.x = (diff % GPROJ_X_TILES) * GPROJ_TILE_WIDTH,
			.y = (diff / GPROJ_X_TILES) * GPROJ_TILE_HEIGHT,
			.w = 32,
			.h = 32
		};

		if ((layer_idx&0x01) == 0)
			SDL_RenderFillRect(sdl_rend, &dst);

		if ((gid&(~TMX_FLIP_BITS_REMOVAL)) == 0x00)
			SDL_RenderCopy(sdl_rend, sdl_tex_ts, &src, &dst);
		else
			draw_flipped_gid(gid, &src, &dst);
	}

	SDL_SetRenderTarget(sdl_rend, NULL);
}


void render_actors(const struct recti* const ss_srcs,
                   const struct rectf* const scr_dsts,
                   const struct actor_anim* anims,
                   const int count)
{
	render_clear(RENDER_LAYER_ACTORS);

	SDL_Rect scr, ss;
	for (int i = 0; i < count; ++i) {
		scr = (SDL_Rect) {
			.x = scr_dsts[i].pos.x,
			.y = scr_dsts[i].pos.y,
			.w = scr_dsts[i].size.x,
			.h = scr_dsts[i].size.y
		};
		ss = (SDL_Rect) {
			.x = ss_srcs[i].pos.x,
			.y = ss_srcs[i].pos.y,
			.w = ss_srcs[i].size.x,
			.h = ss_srcs[i].size.y
		};
		const int flags = anims[i].flags&ANIM_FLAG_FLIPH ? SDL_FLIP_HORIZONTAL : 0;
		SDL_RenderCopyEx(sdl_rend, sdl_tex_ss, &ss, &scr, 0, NULL, flags);
	}

	SDL_SetRenderTarget(sdl_rend, NULL);
}


void render_text(const enum render_layer layers,
                 const struct vec2f* const scrdst,
                 const char* const text,
				 ...)
{
	va_list vargs;
	va_start(vargs, text);

	render_clear(layers);

	for (size_t i = 0; i < ARRSZ(layers_arr); ++i) {
		if (layers&layers_arr[i]) {
			SDL_SetRenderTarget(sdl_rend, tex_targets_arr[i]);
			FC_Draw_v(sdl_font, sdl_rend, scrdst->x, scrdst->y, text, vargs);
			SDL_SetRenderTarget(sdl_rend, NULL);
		}
	}

	va_end(vargs);
}


void render_present(void)
{
	for (size_t i = 0; i < ARRSZ(layers_arr); ++i)
		SDL_RenderCopy(sdl_rend, tex_targets_arr[i], NULL, NULL);

	layers_cleared = 0;

	SDL_RenderPresent(sdl_rend);
}

