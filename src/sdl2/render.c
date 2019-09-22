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

extern struct vec2i map_ts_size;
extern struct vec2i map_tile_size;
extern struct vec2i map_map_size;
extern struct vec2f map_scrl_pos;

static enum render_layer layers_cleared;
static struct vec2i text_pos = { 0, 0 };


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

static void draw_tile_layers(const int32_t* const gids)
{
	SDL_Rect src = {
		.w = map_tile_size.x,
		.h = map_tile_size.y
	};
	SDL_Rect dst = {
		.y = 0,
		.w = map_tile_size.x,
		.h = map_tile_size.y
	};

	for (int y = 0; y < 5; ++y) {

		for (int x = 0; x < 24; ++x) {

			const int32_t gid = gids[(y * map_map_size.x) + x];
			if (gid == 0)
				continue;
			const int32_t id = (gid&TMX_FLIP_BITS_REMOVAL) - 1;

			src.x = (id * map_tile_size.x) % map_ts_size.x,
			src.y = (id / (map_ts_size.x / map_tile_size.x)) * map_tile_size.y;
			dst.x = x * map_tile_size.x;

			if ((gid&(~TMX_FLIP_BITS_REMOVAL)) == 0x00)
				SDL_RenderCopy(sdl_rend, sdl_tex_ts, &src, &dst);
			else
				draw_flipped_gid(gid, &src, &dst);
		}

		dst.y += map_tile_size.y;
	}
}


void render_load_ts(const char* const path)
{
	if (sdl_tex_ts)
		SDL_DestroyTexture(sdl_tex_ts);

	LOG_DEBUG("LOADING TILESHEET: %s", path);

	sdl_tex_ts = IMG_LoadTexture(sdl_rend, path);

	assert(sdl_tex_ts != NULL);
}

void render_load_ss(const char* path)
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
		if ((layers&layers_arr[i]) && !(layers_arr[i]&layers_cleared)) {
			SDL_SetRenderTarget(sdl_rend, tex_targets_arr[i]);
			SDL_RenderClear(sdl_rend);
		}
	}

	layers_cleared |= layers;
}


void render_map(const int32_t* const gids)
{
	render_clear(RENDER_LAYER_BG|RENDER_LAYER_FG);
	SDL_SetRenderTarget(sdl_rend, sdl_tex_bg);
	draw_tile_layers(gids);
	SDL_SetRenderTarget(sdl_rend, sdl_tex_fg);
	draw_tile_layers(gids + MAP_LAYER_FG * map_map_size.x * map_map_size.y);
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
}


void render_text(const char* const text, ...)
{
	va_list vargs;
	va_start(vargs, text);


	render_clear(RENDER_LAYER_FG);
	SDL_SetRenderTarget(sdl_rend, sdl_tex_fg);
	const SDL_Rect dirty =
		FC_Draw_v(sdl_font, sdl_rend,
		          text_pos.x, text_pos.y,
		          text, vargs);
	//x += dirty.w;
	text_pos.y += dirty.h;

	va_end(vargs);

}


void render_present(void)
{
	const SDL_Rect src_bg = {
		.x = map_scrl_pos.x,
		.y = 0,
		.w = GPROJ_SCR_WIDTH,
		.h = GPROJ_SCR_HEIGHT,
	};

	SDL_SetRenderTarget(sdl_rend, NULL);
	SDL_RenderCopy(sdl_rend, sdl_tex_bg, &src_bg, NULL);
	SDL_RenderCopy(sdl_rend, sdl_tex_actors, NULL, NULL);
	SDL_RenderCopy(sdl_rend, sdl_tex_fg, NULL, NULL);
	SDL_RenderPresent(sdl_rend);


	text_pos.x = 0;
	text_pos.y = 0;

	layers_cleared = 0;
}

