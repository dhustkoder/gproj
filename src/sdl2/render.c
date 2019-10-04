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
#include "render.h"

static SDL_Window* win = NULL;
static SDL_Renderer* rend;
static SDL_Texture** layers;
static SDL_Texture* tex_txt;
static SDL_Texture* tex_ts;
static SDL_Texture* tex_ss;
static FC_Font* font;

static int layers_cnt = 0;
static struct vec2i layers_size = { 0, 0 };
static struct vec2i ts_size = { 0, 0 };
static struct vec2i text_pos = { 0, 0 };


static void fb_free(void)
{
	for (int i = 0; i < layers_cnt; ++i)
		SDL_DestroyTexture(layers[i]);
	free(layers);
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
	SDL_RenderClear(rend);
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

void render_layers_setup(const struct vec2i _layers_size, const int _layers_cnt)
{
	LOG_DEBUG("RENDER LAYERS SETUP");

	fb_free();

	layers_size = _layers_size;
	layers_cnt = _layers_cnt;
	layers = malloc(sizeof(*layers) * layers_cnt);
	assert(layers != NULL);	

	int err;
	((void)err);
	for (int i = 0; i < layers_cnt; ++i) {
		layers[i] = SDL_CreateTexture(rend,
		                              SDL_PIXELFORMAT_UNKNOWN,
		                              SDL_TEXTUREACCESS_TARGET,
		                              layers_size.x, layers_size.y);
		assert(layers[i] != NULL);
		err = SDL_SetTextureBlendMode(layers[i], SDL_BLENDMODE_BLEND);
		assert(err == 0);
	}
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



void render_ss(const int layer,
               const struct vec2f* const wpos,
               const struct vec2i* const wsize,
               const struct vec2i* const spos,
               const struct vec2i* const ssize,
               const render_flag_t* const flags,
               const int cnt)
{
	SDL_SetRenderTarget(rend, layers[layer]);
	SDL_RenderClear(rend);
	
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

		SDL_RenderCopyEx(rend, tex_ss, &sr, &wr, 0, NULL, flags[i]);
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


void render_present(void)
{
	const SDL_Rect text_rect = {
		.x = 0,
		.y = 0,
		.w = text_pos.x,
		.h = text_pos.y
	};

	SDL_SetRenderTarget(rend, NULL);
	SDL_RenderClear(rend);
	
	for (int i = 0; i < layers_cnt; ++i)
		SDL_RenderCopy(rend, layers[i], NULL, NULL);

	SDL_RenderCopy(rend, tex_txt, &text_rect, &text_rect);
	SDL_RenderPresent(rend);

	text_pos.x = 0;
	text_pos.y = 0;
}




