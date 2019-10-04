#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_FontCache.h>
#include <tmx.h>
#include "events.h"
#include "logger.h"
#include "map.h"
#include "render.h"

static SDL_Window* win = NULL;
static SDL_Renderer* rend;
static SDL_Texture* tex_bg;
static SDL_Texture* tex_actors;
static SDL_Texture* tex_fg;
static SDL_Texture* tex_txt;
static SDL_Texture* tex_ts;
static SDL_Texture* tex_ss;
static FC_Font* font;

static struct vec2i ts_size = { 0, 0 };
static struct vec2i fb_size = { 0, 0 };
static struct vec2i text_pos = { 0, 0 };
static struct vec2i cam_pos = { 0, 0 };
static struct recti view_area = { {0, 0}, {GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT}};


static void fb_free(void)
{
	if (tex_fg != NULL)
		SDL_DestroyTexture(tex_fg);
	if (tex_actors != NULL)
		SDL_DestroyTexture(tex_actors);
	if (tex_bg != NULL)
		SDL_DestroyTexture(tex_bg);
}


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

	SDL_RenderCopyEx(rend, tex_ts, src, dst, degrees, NULL, flips);
}

static void draw_map_layer(const int32_t* const gids,
		const struct vec2i map_size,
		const struct vec2i tile_size)
{
	SDL_Rect src = {
		.w = tile_size.x,
		.h = tile_size.y
	};

	SDL_Rect dst = {
		.y = 0,
		.w = tile_size.x,
		.h = tile_size.y
	};

	const int y_tiles = map_size.y;
	const int x_tiles = map_size.x;
	const int ts_w = ts_size.x;


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
	LOG_DEBUG("INITIALIZING RENDER");
	int err;
	((void)err);

	const char* const winname = identifier;
	assert(winname != NULL);

	win = SDL_CreateWindow(winname,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT,
			SDL_WINDOW_RESIZABLE);
	assert(win != NULL);

	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED|
	                                   SDL_RENDERER_TARGETTEXTURE);
	assert(rend != NULL);

	tex_txt = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGB888,
	                            SDL_TEXTUREACCESS_TARGET,
	                            GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	assert(tex_txt != NULL);


	font = FC_CreateFont();
	assert(font != NULL);

	err = FC_LoadFont(font, rend, "8bit-madness.ttf", 16,
	                  FC_MakeColor(0xFF,0xFF,0xFF,0xFF), TTF_STYLE_NORMAL);
	assert(err != 0);

	SDL_SetRenderTarget(rend, NULL);	
	SDL_RenderPresent(rend);
}

void render_term()
{
	LOG_DEBUG("TERMINATING RENDER");
	FC_FreeFont(font);

	if (tex_ts != NULL)
		SDL_DestroyTexture(tex_ts);
	if (tex_ss != NULL)
		SDL_DestroyTexture(tex_ss);

	fb_free();
	
	if (tex_txt != NULL)
		SDL_DestroyTexture(tex_txt);
	if (rend != NULL)
		SDL_DestroyRenderer(rend);
	if (win != NULL)
		SDL_DestroyWindow(win);
}

void render_fb_setup(const struct vec2i* const size)
{
	LOG_DEBUG("TRYING TO SETUP FB");
	fb_size = *size;
	int err;
	((void)err);

	fb_free();
	tex_bg = SDL_CreateTexture(rend,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			fb_size.x, fb_size.y);
	assert(tex_bg != NULL);

	tex_actors = SDL_CreateTexture(rend,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			fb_size.x, fb_size.y);
	assert(tex_actors != NULL);

	tex_fg = SDL_CreateTexture(rend,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			fb_size.x, fb_size.y);
	assert(tex_fg != NULL);

	err = SDL_SetTextureBlendMode(tex_bg, SDL_BLENDMODE_BLEND);
	assert(err == 0);
	err = SDL_SetTextureBlendMode(tex_actors, SDL_BLENDMODE_BLEND);
	assert(err == 0);
	err = SDL_SetTextureBlendMode(tex_fg, SDL_BLENDMODE_BLEND);
	assert(err == 0);
}


void render_load_ts(const char* const path)
{
	LOG_DEBUG("LOADING TILESET: %s", path);
	if (tex_ts)
		SDL_DestroyTexture(tex_ts);

	assert(path != NULL);

	tex_ts = IMG_LoadTexture(rend, path);
	assert(tex_ts != NULL);
	SDL_QueryTexture(tex_ts, NULL, NULL, &ts_size.x, &ts_size.y);

}


void render_load_ss(const char* const path)
{
	LOG_DEBUG("LOADING SPRITESHEET: %s", path);
	if (tex_ss)
		SDL_DestroyTexture(tex_ss);

	assert(path != NULL);

	tex_ss = IMG_LoadTexture(rend, path);
	assert(tex_ss != NULL);
}


void render_map(const int32_t* const gids,
                const struct vec2i* const map_size,
                const struct vec2i* const tile_size) 
{
	SDL_SetRenderTarget(rend, tex_bg);
	draw_map_layer(gids, *map_size, *tile_size);
	SDL_SetRenderTarget(rend, tex_fg);
	draw_map_layer(gids + MAP_LAYER_FG *
			map_size->x * map_size->y,
			*map_size, *tile_size);
}


void render_ss(const struct recti* const ss_srcs,
               const struct rectf* const scr_dsts,
               const actor_flag_t* const flags,
               const int cnt)
{
	SDL_SetRenderTarget(rend, tex_actors);
	SDL_RenderClear(rend);
	
	const struct recti view = view_area;
	for (int i = 0; i < cnt; ++i) {
		const SDL_Rect scr = (SDL_Rect) {
			.x = scr_dsts[i].pos.x,
			.y = scr_dsts[i].pos.y,
			.w = scr_dsts[i].size.x,
			.h = scr_dsts[i].size.y
		};

		if      ((scr.x + scr.w) < view.pos.x) continue;
		else if ((scr.y + scr.h) < view.pos.y) continue;
		else if (scr.x > (view.pos.x + view.size.x)) continue;
		else if (scr.y > (view.pos.y + view.size.y)) continue;

		const SDL_Rect ss = (SDL_Rect) {
			.x = ss_srcs[i].pos.x,
			.y = ss_srcs[i].pos.y,
			.w = ss_srcs[i].size.x,
			.h = ss_srcs[i].size.y
		};

		const int flip = flags[i]&ACTOR_FLAG_FLIPH ? SDL_FLIP_HORIZONTAL : 0;
		SDL_RenderCopyEx(rend, tex_ss, &ss, &scr, 0, NULL, flip);
	}
			
}


void render_text(const char* const text, ...)
{
	static char buffer[256];
	va_list vargs;
	va_start(vargs, text);

	vsnprintf(buffer, 256, text, vargs);
	
	SDL_SetRenderTarget(rend, tex_txt);
	
	if (text_pos.x == 0)
		SDL_RenderClear(rend);

	const SDL_Rect dirty = FC_Draw(font, rend, 0, text_pos.y, buffer);
  
	if (dirty.w > text_pos.x)
		text_pos.x = dirty.w;
	
	text_pos.y += dirty.h;


	va_end(vargs);
}


void render_set_camera(int x, int y)
{
	if (x > fb_size.x - GPROJ_SCR_WIDTH)
		x = fb_size.x - GPROJ_SCR_WIDTH;

	if (y > fb_size.y - GPROJ_SCR_HEIGHT)
		y = fb_size.y - GPROJ_SCR_HEIGHT;

	cam_pos.x = x < 0 ? 0 : x;
	cam_pos.y = y < 0 ? 0 : y;

	view_area.pos.x = cam_pos.x;
	view_area.pos.y = cam_pos.y;
}


void render_present(void)
{
	const SDL_Rect cam_rect = {
		.x = cam_pos.x,
		.y = cam_pos.y,
		.w = GPROJ_SCR_WIDTH,
		.h = GPROJ_SCR_HEIGHT
	};

	const SDL_Rect text_rect = {
		.x = 0,
		.y = 0,
		.w = text_pos.x,
		.h = text_pos.y
	};

	SDL_SetRenderTarget(rend, NULL);
	SDL_RenderCopy(rend, tex_bg, &cam_rect, NULL);
	SDL_RenderCopy(rend, tex_actors, &cam_rect, NULL);
	SDL_RenderCopy(rend, tex_fg, &cam_rect, NULL);
	SDL_RenderCopy(rend, tex_txt, &text_rect, &text_rect);
	SDL_RenderPresent(rend);

	text_pos.x = 0;
	text_pos.y = 0;
}




