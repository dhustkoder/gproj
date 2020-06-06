#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_FontCache.h>
#include "logger.h"
#include "render.h"


extern SDL_Window* sdl_window;
static SDL_Renderer* rend;
static SDL_Texture* layers[GPROJ_RENDER_NLAYERS];
static SDL_Texture* tex_txt;
static SDL_Texture* tex_ts;
static SDL_Texture* tex_ss;
static FC_Font* font;

static unsigned layers_cleared = 0;
static struct vec2i layers_size = { 0, 0 };
static struct vec2i text_pos = { 0, 0 };



static void prepare_target_layer(const int layer)
{
	assert(layer >= 0 && layer < GPROJ_RENDER_NLAYERS);
	SDL_SetRenderTarget(rend, layers[layer]);
	if (!(layers_cleared&(1<<layer))) {
		SDL_RenderClear(rend);
		layers_cleared |= (1<<layer);
	}
}



void sdl2_render_init(void)
{
	log_dbg("INITIALIZING SDL2 RENDER");
	int err;
	((void)err);

	assert(sdl_window != NULL);

	rend = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED|
	                                   SDL_RENDERER_TARGETTEXTURE);
	assert(rend != NULL);

	tex_txt = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888,
	                            SDL_TEXTUREACCESS_TARGET,
	                            GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	assert(tex_txt != NULL);


	font = FC_CreateFont();
	assert(font != NULL);

	err = FC_LoadFont(font, rend, "8bit-madness.ttf", 16,
	                  FC_MakeColor(0xFF,0xFF,0xFF,0xFF), TTF_STYLE_NORMAL);
	assert(err != 0);

	for (int i = 0; i < GPROJ_RENDER_NLAYERS; ++i) {
		layers[i] = SDL_CreateTexture(rend,
		                              SDL_PIXELFORMAT_RGBA8888,
		                              SDL_TEXTUREACCESS_TARGET,
		                              GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
		assert(layers[i] != NULL);
		err = SDL_SetTextureBlendMode(layers[i], SDL_BLENDMODE_BLEND);
		assert(err == 0);
	}

	err = (int) SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	if ((SDL_bool)err == SDL_FALSE)
		INVALID_CODE_PATH;

	SDL_SetTextureBlendMode(tex_txt, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(rend, NULL);
	SDL_RenderClear(rend);
	SDL_RenderPresent(rend);
}

void sdl2_render_term(void)
{
	log_dbg("TERMINATING SDL2 RENDER");
	FC_FreeFont(font);

	if (tex_ts != NULL)
		SDL_DestroyTexture(tex_ts);
	if (tex_ss != NULL)
		SDL_DestroyTexture(tex_ss);

	for (int i = 0; i < GPROJ_RENDER_NLAYERS; ++i)
		SDL_DestroyTexture(layers[i]);

	if (tex_txt != NULL)
		SDL_DestroyTexture(tex_txt);
	if (rend != NULL)
		SDL_DestroyRenderer(rend);
}

void sdl2_render_load_ts(const char* const path)
{
	log_dbg("LOADING TILESET: %s", path);
	if (tex_ts)
		SDL_DestroyTexture(tex_ts);

	assert(path != NULL);

	tex_ts = IMG_LoadTexture(rend, path);
	assert(tex_ts != NULL);
}


void sdl2_render_load_ss(const char* const path)
{
	log_dbg("LOADING SPRITESHEET: %s", path);
	if (tex_ss)
		SDL_DestroyTexture(tex_ss);

	assert(path != NULL);

	tex_ss = IMG_LoadTexture(rend, path);
	assert(tex_ss != NULL);
}

void sdl2_render_tilemap(const struct vec2f* const cam, const struct tilemap* const map)
{

	prepare_target_layer(0);
	const struct vec2i cnt = map->size;
	const struct vec2i pos = { .x = cam->x, .y = cam->y };
	const struct vec2i* const tiles = map->tiles;
#ifdef GPROJ_DEBUG
	const int tile_width = TILE_WIDTH * map->scale;
	const int tile_height = TILE_HEIGHT * map->scale;
#else
	const int tile_width = TILE_WIDTH;
	const int tile_height = TILE_HEIGHT;
#endif
	for (int y = 0; y < cnt.y; ++y) {
		for (int x = 0; x < cnt.x; ++x) {
			const int offset = y * cnt.x + x;
			const struct vec2i uv = tiles[offset];
			if (uv.x < 0) continue;
			const SDL_Rect ts_rect = {
				.x = uv.x,
				.y = uv.y,
				.w = TILE_WIDTH,
				.h = TILE_HEIGHT
			};
			const SDL_Rect scr_rect = {
				.x = (x * tile_width) + pos.x,
				.y = (y * tile_height) + pos.y,
				.w = tile_width,
				.h = tile_height
			};
			SDL_RenderCopy(rend, tex_ts, &ts_rect, &scr_rect);
		}
	}
}

void sdl2_render_ss(const int layer,
               const struct vec2f* restrict const wpos,
               const struct vec2i* restrict const wsize,
               const struct vec2i* restrict const spos,
               const struct vec2i* restrict const ssize,
               const render_flag_t* restrict const flags,
               const int cnt)
{
	prepare_target_layer(layer);

	for (int i = 0; i < cnt; ++i) {
		const SDL_Rect wr = (SDL_Rect) {
			.x = wpos[i].x,
			.y = wpos[i].y,
			.w = wsize[i].x,
			.h = wsize[i].y
		};

		const SDL_Rect sr = (SDL_Rect) {
			.x = spos[i].x,
			.y = spos[i].y,
			.w = ssize[i].x,
			.h = ssize[i].y
		};

		SDL_RenderCopyEx(rend, tex_ss, &sr, &wr, 0, NULL, 0);
	}

}


void sdl2_render_text(const char* const text, ...)
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

void sdl2_render_finish_frame(void)
{
	const SDL_Rect text_rect = {
		.x = 0,
		.y = 0,
		.w = text_pos.x,
		.h = text_pos.y
	};
	const SDL_Rect text_dst = {
		.x = 0,
		.y = 0,
		.w = text_pos.x * 2,
		.h = text_pos.y * 2
	};

	SDL_SetRenderTarget(rend, NULL);
	SDL_RenderClear(rend);

	for (int i = 0; i < GPROJ_RENDER_NLAYERS; ++i)
		SDL_RenderCopy(rend, layers[i], NULL, NULL);

	SDL_RenderCopy(rend, tex_txt, &text_rect, &text_dst);
	SDL_RenderPresent(rend);

	text_pos.x = 0;
	text_pos.y = 0;
	layers_cleared = 0;
}




