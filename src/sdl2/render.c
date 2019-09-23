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

static SDL_Window* win = NULL;
static SDL_Renderer* rend;
static SDL_Texture* tex_bg;
static SDL_Texture* tex_actors;
static SDL_Texture* tex_fg;
static SDL_Texture* tex_txt;
static SDL_Texture* tex_ts;
static SDL_Texture* tex_ss;
static FC_Font* font;

const enum render_layer targets_flags[RENDER_LAYER_NLAYERS] = {
	RENDER_LAYER_BG,
	RENDER_LAYER_ACTORS,
	RENDER_LAYER_FG,
	RENDER_LAYER_TXT
};

SDL_Texture* targets_arr[RENDER_LAYER_NLAYERS];

extern struct vec2i map_ts_size;
extern struct vec2i map_tile_size;
extern struct vec2i map_map_size;

static enum render_layer layers_cleared;
static struct vec2i text_pos = { 0, 0 };
static struct vec2i cam_pos = { 0, 0 };


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

	SDL_RenderCopyEx(rend,
	                 tex_ts,
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

	const int y_tiles = map_map_size.y;
	const int x_tiles = map_map_size.x;
	const int ts_w = map_ts_size.x;
	const struct vec2i tile_size = {
		map_tile_size.x, map_tile_size.y
	};

	for (int y = 0; y < y_tiles; ++y) {
		for (int x = 0; x < x_tiles; ++x) {

			const int32_t gid = gids[(y * x_tiles) + x];
			if (gid == 0)
				continue;
			const int32_t id = (gid&TMX_FLIP_BITS_REMOVAL) - 1;

			src.x = (id * tile_size.x) % ts_w,
			src.y = (id / (ts_w / tile_size.x)) * tile_size.y;
			dst.x = x * tile_size.x;

			if ((gid&(~TMX_FLIP_BITS_REMOVAL)) == 0x00)
				SDL_RenderCopy(rend, tex_ts, &src, &dst);
			else
				draw_flipped_gid(gid, &src, &dst);
		}

		dst.y += tile_size.y;
	}
}


void render_init(const char* const identifier)
{
	int err;

	win = SDL_CreateWindow(identifier,
	                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                       GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT,
	                       SDL_WINDOW_RESIZABLE);
	assert(win != NULL);

	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
	assert(rend != NULL);

	tex_txt = SDL_CreateTexture(rend,
	                                SDL_PIXELFORMAT_RGB888,
	                                SDL_TEXTUREACCESS_TARGET,
	                                GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	assert(tex_txt != NULL);

	font = FC_CreateFont();
	assert(font != NULL);

	err = FC_LoadFont(font, rend, "8bit-madness.ttf", 16,
					  FC_MakeColor(0xFF,0xFF,0xFF,0xFF), TTF_STYLE_NORMAL);
	assert(err != 0);

	err = SDL_SetTextureBlendMode(tex_txt, SDL_BLENDMODE_BLEND);
	assert(err == 0);


	targets_arr[3] = tex_txt;

	render_clear(RENDER_LAYER_TXT);

	render_present();
}

void render_term()
{
	FC_FreeFont(font);

	if (tex_ts != NULL)
		SDL_DestroyTexture(tex_ts);
	if (tex_ss != NULL)
		SDL_DestroyTexture(tex_ss);
	if (tex_fg != NULL)
		SDL_DestroyTexture(tex_fg);
	if (tex_actors != NULL)
		SDL_DestroyTexture(tex_actors);
	if (tex_bg != NULL)
		SDL_DestroyTexture(tex_bg);
	if (tex_txt != NULL)
		SDL_DestroyTexture(tex_txt);
	if (rend != NULL)
		SDL_DestroyRenderer(rend);
	if (win != NULL)
		SDL_DestroyWindow(win);
}



void render_load_world(const char* ts_path,
                       struct vec2i* world_size)
{
	tex_bg = SDL_CreateTexture(rend,
	                        SDL_PIXELFORMAT_RGB888,
	                        SDL_TEXTUREACCESS_TARGET,
	                        world_size->x, world_size->y);
	assert(tex_bg != NULL);

	tex_actors = SDL_CreateTexture(rend,
	                        SDL_PIXELFORMAT_RGB888,
	                        SDL_TEXTUREACCESS_TARGET,
	                        world_size->x, world_size->y);
	assert(tex_actors != NULL);

	tex_fg = SDL_CreateTexture(rend,
	                        SDL_PIXELFORMAT_RGB888,
	                        SDL_TEXTUREACCESS_TARGET,
	                        world_size->x, world_size->y);
	assert(tex_fg != NULL);

	int err;
	err = SDL_SetTextureBlendMode(tex_actors, SDL_BLENDMODE_BLEND);
	assert(err == 0);
	err = SDL_SetTextureBlendMode(tex_fg, SDL_BLENDMODE_BLEND);
	assert(err == 0);

	if (tex_ts)
		SDL_DestroyTexture(tex_ts);

	LOG_DEBUG("LOADING TILESHEET: %s", ts_path);

	tex_ts = IMG_LoadTexture(rend, ts_path);

	assert(tex_ts != NULL);
	targets_arr[0] = tex_bg;
	targets_arr[1] = tex_actors;
	targets_arr[2] = tex_fg;

	render_clear(RENDER_LAYER_ALL);

	render_present();

}

void render_load_ss(const char* path)
{
	if (tex_ss)
		SDL_DestroyTexture(tex_ss);

	LOG_DEBUG("LOADING SPRITESHEET: %s", path);
	tex_ss = IMG_LoadTexture(rend, path);

	assert(tex_ss != NULL);
}


void render_clear(const enum render_layer layers)
{
	assert(layers != 0);

	for (size_t i = 0; i < ARRSZ(targets_flags); ++i) {
		if ((layers&targets_flags[i]) && !(targets_flags[i]&layers_cleared)) {
			SDL_SetRenderTarget(rend, targets_arr[i]);
			SDL_RenderClear(rend);
		}
	}

	layers_cleared |= layers;
}


void render_map(const int32_t* const gids)
{
	render_clear(RENDER_LAYER_BG|RENDER_LAYER_FG);
	SDL_SetRenderTarget(rend, tex_bg);
	draw_tile_layers(gids);
	SDL_SetRenderTarget(rend, tex_fg);
	draw_tile_layers(gids + MAP_LAYER_FG * map_map_size.x * map_map_size.y);
}


void render_actors(const struct recti* const ss_srcs,
                   const struct rectf* const scr_dsts,
                   const actor_anim_flag_t* const flags,
                   const int count)
{
	/* FIXME: clearing takes the most time for actors update */
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
		const int flip = flags[i]&ANIM_FLAG_FLIPH ? SDL_FLIP_HORIZONTAL : 0;
		SDL_RenderCopyEx(rend, tex_ss, &ss, &scr, 0, NULL, flip);
	}
}


void render_text(const char* const text, ...)
{
	va_list vargs;
	va_start(vargs, text);

	text_pos.x = 0;

	render_clear(RENDER_LAYER_TXT);
	SDL_SetRenderTarget(rend, tex_txt);
	const SDL_Rect dirty = FC_Draw_v(font, rend,
		                             text_pos.x, text_pos.y,
		                             text, vargs);
	//x += dirty.w;
	text_pos.y += dirty.h;

	va_end(vargs);
}


void render_set_camera(int x, int y)
{
	cam_pos.x = x;
	cam_pos.y = y;
}


void render_present(void)
{
	render_text("CAMERA POS => (%d, %d)", cam_pos.x, cam_pos.y);

	const SDL_Rect cam_rect = {
		.x = cam_pos.x,
		.y = cam_pos.y,
		.w = GPROJ_SCR_WIDTH,
		.h = GPROJ_SCR_HEIGHT
	};

	SDL_SetRenderTarget(rend, NULL);
	SDL_RenderCopy(rend, tex_bg, &cam_rect, NULL);
	SDL_RenderCopy(rend, tex_actors, &cam_rect, NULL);
	SDL_RenderCopy(rend, tex_fg, &cam_rect, NULL);
	SDL_RenderCopy(rend, tex_txt, NULL, NULL);
	SDL_RenderPresent(rend);

	text_pos.x = 0;
	text_pos.y = 0;

	layers_cleared = 0;
}




