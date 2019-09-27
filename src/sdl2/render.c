#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_FontCache.h>
#include <tmx.h>
#include "events.h"
#include "threads.h"
#include "logger.h"
#include "map.h"
#include "render.h"


#define THR_ACTIONS_MAX (2048)

typedef uint8_t render_thr_action_t;
enum render_thr_actions {
	RENDER_THR_DRAW_TXT,
	RENDER_THR_DRAW_ACTORS,
	RENDER_THR_DRAW_MAP,
	RENDER_THR_FETCH,
	RENDER_THR_PRESENT,
	RENDER_THR_SETUP_FB,
	RENDER_THR_INIT,
	RENDER_THR_TERMINATE
};

static struct render_actors_pack {
	const struct recti* ss_srcs;
	const struct rectf* scr_dsts;
	const actor_anim_flag_t* flags;
	int cnt;
} actors_packs[256];

static struct render_text_pack {
	char buffer[256];
} txt_packs[256];

static struct render_map_pack {
	const int32_t* gids;
	const struct vec2i* map_size;
	const struct vec2i* tile_size;
} map_pack;

static render_thr_action_t thr_action_queue[THR_ACTIONS_MAX];
static SDL_atomic_t thr_action_cnt;
static SDL_atomic_t thr_action_idx;

static volatile int actors_packs_cnt;
static volatile int txt_packs_cnt;
static volatile int map_pack_cnt;

static int thr_actors_packs_drawed = 0;
static int thr_txt_packs_drawed = 0;

static void* thr_winname = NULL;
static void* thr_ts_path = NULL;
static void* thr_ss_path = NULL;

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


static void thr_init()
{
	int err;
	((void)err);

	const char* const winname = (char*)SDL_AtomicGetPtr(&thr_winname);
	assert(winname != NULL);

	win = SDL_CreateWindow(winname,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT,
			SDL_WINDOW_RESIZABLE);
	assert(win != NULL);

	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
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

	err = SDL_SetTextureBlendMode(tex_txt, 0);
	assert(err == 0);

	SDL_SetRenderTarget(rend, tex_txt);
	SDL_RenderClear(rend);
	SDL_SetRenderTarget(rend, NULL);
	SDL_RenderPresent(rend);
}


static void thr_fb_free(void)
{
	if (tex_fg != NULL)
		SDL_DestroyTexture(tex_fg);
	if (tex_actors != NULL)
		SDL_DestroyTexture(tex_actors);
	if (tex_bg != NULL)
		SDL_DestroyTexture(tex_bg);
}

static void thr_term()
{
	FC_FreeFont(font);

	if (tex_ts != NULL)
		SDL_DestroyTexture(tex_ts);
	if (tex_ss != NULL)
		SDL_DestroyTexture(tex_ss);

	thr_fb_free();
	
	if (tex_txt != NULL)
		SDL_DestroyTexture(tex_txt);
	if (rend != NULL)
		SDL_DestroyRenderer(rend);
	if (win != NULL)
		SDL_DestroyWindow(win);
}

static void thr_fb_setup()
{
	int err;
	((void)err);

	if (tex_ts)
		SDL_DestroyTexture(tex_ts);

	const char* const ts_path = (char*)SDL_AtomicGetPtr(&thr_ts_path);
	assert(ts_path != NULL);

	tex_ts = IMG_LoadTexture(rend, ts_path);
	assert(tex_ts != NULL);
	SDL_QueryTexture(tex_ts, NULL, NULL, &ts_size.x, &ts_size.y);


	if (tex_ss)
		SDL_DestroyTexture(tex_ss);

	const char* const ss_path = (char*)SDL_AtomicGetPtr(&thr_ss_path);
	assert(ss_path != NULL);

	tex_ss = IMG_LoadTexture(rend, ss_path);
	assert(tex_ss != NULL);


	thr_fb_free();
	tex_bg = SDL_CreateTexture(rend,
			SDL_PIXELFORMAT_RGB888,
			SDL_TEXTUREACCESS_TARGET,
			fb_size.x, fb_size.y);
	assert(tex_bg != NULL);

	tex_actors = SDL_CreateTexture(rend,
			SDL_PIXELFORMAT_RGB888,
			SDL_TEXTUREACCESS_TARGET,
			fb_size.x, fb_size.y);
	assert(tex_actors != NULL);

	tex_fg = SDL_CreateTexture(rend,
			SDL_PIXELFORMAT_RGB888,
			SDL_TEXTUREACCESS_TARGET,
			fb_size.x, fb_size.y);
	assert(tex_fg != NULL);

	err = SDL_SetTextureBlendMode(tex_actors, SDL_BLENDMODE_BLEND);
	assert(err == 0);
	err = SDL_SetTextureBlendMode(tex_fg, SDL_BLENDMODE_BLEND);
	assert(err == 0);
}

static void thr_draw_flipped_gid(const int32_t gid,
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

static void thr_draw_map_layer(const int32_t* const gids,
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
				thr_draw_flipped_gid(gid, &src, &dst);
		}

		dst.y += tile_size.y;
	}
}


static void thr_draw_actors()
{
	const int apc = actors_packs_cnt;
	if (apc > thr_actors_packs_drawed) {
		SDL_SetRenderTarget(rend, tex_actors);
		if (thr_actors_packs_drawed == 0)
			SDL_RenderClear(rend);
		const struct render_actors_pack* const pack = &actors_packs[thr_actors_packs_drawed++];
		const struct recti* const ss_srcs = pack->ss_srcs;
		const struct rectf* const scr_dsts = pack->scr_dsts;
		const actor_anim_flag_t* const flags  = pack->flags;
		const int cnt = pack->cnt;



		SDL_Rect scr, ss;
		for (int i = 0; i < cnt; ++i) {
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
}

static void thr_draw_txt()
{
	const int tpc = txt_packs_cnt;
	if (tpc > thr_txt_packs_drawed) {
		SDL_SetRenderTarget(rend, tex_txt);
		if (thr_txt_packs_drawed == 0)
			SDL_RenderClear(rend);
		for (int i = thr_txt_packs_drawed; i < tpc; ++i) {
			const struct render_text_pack* const pack = &txt_packs[thr_txt_packs_drawed++];
			const SDL_Rect dirty = FC_Draw(font, rend, 0, text_pos.y, pack->buffer);
			if (dirty.w > text_pos.x)
				text_pos.x = dirty.w;
			text_pos.y += dirty.h;
		}
	}
}

static void thr_draw_map()
{
	if (map_pack.gids != NULL) {
		SDL_SetRenderTarget(rend, tex_bg);
		thr_draw_map_layer(map_pack.gids, *map_pack.map_size, *map_pack.tile_size);
		SDL_SetRenderTarget(rend, tex_fg);
		thr_draw_map_layer(map_pack.gids + MAP_LAYER_FG *
				map_pack.map_size->x * map_pack.map_size->y,
				*map_pack.map_size, *map_pack.tile_size);
		map_pack.gids = NULL;
	}
}

static void thr_present()
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
	thr_actors_packs_drawed = 0;
	thr_txt_packs_drawed = 0;
}



void render_thr_main(void)
{
	extern void thr_events_update();
	for (;;) {
		thr_events_update();
		const int idx = SDL_AtomicGet(&thr_action_idx);
		const int cnt = SDL_AtomicGet(&thr_action_cnt);
		if (cnt > idx) {
			for (int i = idx; i < cnt; ++i) {
				const render_thr_action_t action = thr_action_queue[i];
				switch (action) {
				case RENDER_THR_DRAW_TXT:
					thr_draw_txt();
					break;
				case RENDER_THR_DRAW_ACTORS:
					thr_draw_actors();
					break;
				case RENDER_THR_DRAW_MAP:
					thr_draw_map();
					break;
				case RENDER_THR_PRESENT:
					thr_present();
					break;
				case RENDER_THR_SETUP_FB:
					thr_fb_setup();
					break;
				case RENDER_THR_INIT:
					thr_init();
					break;
				case RENDER_THR_TERMINATE:
					goto Lterminate;
					break;
				default:
					LOG_DEBUG("UNKNOWN ACTION: %d", (int)action);
					assert(false && "UNKNOWN ACTION");
					break;
				}		
			}
			SDL_AtomicSet(&thr_action_idx, cnt);
		} else {
			timer_sleep(1);
		}
	}

Lterminate:
	thr_term();
	return;
}


static void render_actions_push(const render_thr_action_t action)
{
	const int cnt = SDL_AtomicGet(&thr_action_cnt);
	assert(cnt <= THR_ACTIONS_MAX);
	thr_action_queue[cnt] = action;
	SDL_AtomicIncRef(&thr_action_cnt);
}

static void render_actions_wait(void)
{
	const int cnt = SDL_AtomicGet(&thr_action_cnt);
	while (SDL_AtomicGet(&thr_action_idx) < cnt)
		;
}

static void render_actions_reset(void)
{
	map_pack_cnt = 0;
	actors_packs_cnt = 0;
	txt_packs_cnt = 0;
	SDL_AtomicSet(&thr_action_cnt, 0);
	SDL_AtomicSet(&thr_action_idx, 0);
}



void render_init(const char* const identifier)
{
	LOG_DEBUG("INITIALIZING RENDER");
	SDL_AtomicSetPtr(&thr_winname, (void*)identifier);
	render_actions_reset();
	render_actions_push(RENDER_THR_INIT);
	render_actions_wait();
}

void render_term()
{
	LOG_DEBUG("TERMINATING RENDER");
	render_actions_push(RENDER_THR_TERMINATE);
	render_actions_wait();
}

void render_fb_setup(const struct vec2i* const size)
{
	LOG_DEBUG("TRYING TO SETUP FB");
	fb_size = *size;
	render_actions_push(RENDER_THR_SETUP_FB);
	render_actions_wait();
}


void render_load_ts(const char* const path)
{
	LOG_DEBUG("LOADING TILESET: %s", path);
	SDL_AtomicSetPtr(&thr_ts_path, (void*)path);
}


void render_load_ss(const char* const path)
{
	LOG_DEBUG("LOADING SPRITESHEET: %s", path);
	SDL_AtomicSetPtr(&thr_ss_path, (void*)path);
}


void render_map(const int32_t* const gids,
                const struct vec2i* const map_size,
                const struct vec2i* const tile_size) 
{
	map_pack = (struct render_map_pack) {
		.gids = gids,
		.map_size = map_size,
		.tile_size = tile_size
	};
	++map_pack_cnt;
	render_actions_push(RENDER_THR_DRAW_MAP);
}


void render_actors(const struct recti* const ss_srcs,
		const struct rectf* const scr_dsts,
		const actor_anim_flag_t* const flags,
		const int cnt)
{
	const int packs_cnt = actors_packs_cnt;
	assert(packs_cnt < 256);
	actors_packs[packs_cnt] = (struct render_actors_pack) {
		.ss_srcs = ss_srcs,
		.scr_dsts = scr_dsts,
		.flags = flags,
		.cnt = cnt
	};
	++actors_packs_cnt;
	render_actions_push(RENDER_THR_DRAW_ACTORS);
}


void render_text(const char* const text, ...)
{
	va_list vargs;
	va_start(vargs, text);
	const int packs_cnt = txt_packs_cnt;
	assert(packs_cnt < 256);
	struct render_text_pack* const pack = &txt_packs[packs_cnt];
	vsnprintf(pack->buffer, 256, text, vargs);
	++txt_packs_cnt;
	render_actions_push(RENDER_THR_DRAW_TXT);
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
}


void render_present(void)
{
	render_actions_push(RENDER_THR_PRESENT);
	render_actions_wait();
	render_actions_reset();
}




