#include <stdint.h>
#include <string.h>
#include "render.h"
#include "video.h"

static uint32_t background[GPROJ_FB_HEIGHT][GPROJ_FB_WIDTH];
static uint32_t sprites[GPROJ_FB_HEIGHT][GPROJ_FB_WIDTH];
static uint32_t framebuffer[GPROJ_FB_HEIGHT][GPROJ_FB_WIDTH];


void render_clear(const bool clear_bkg, const bool clear_sprites)
{
	if (clear_bkg)
		memset(background, 0, sizeof(background));
	if (clear_sprites)
		memset(sprites, 0, sizeof(sprites));
}

void render_draw_map(const char* const map)
{
	struct sprite spr = {
		.x = 0,
		.y = 0,
		.w = GPROJ_FB_WIDTH / 64,
		.h = GPROJ_FB_HEIGHT / 32,
		.rgba = 0x00
	};

	for (int i = 0; map[i] != '\0'; ++i) {
		switch (map[i]) {
		case '.':
			spr.rgba = 0xABABABFF;
			break;
		case 'x':
			spr.rgba = 0x000000FF;
			break;
		}

		const int ybeg = spr.y;
		const int xbeg = spr.x;
		const int yend = spr.y + spr.h;
		const int xend = spr.x + spr.w;
		for (int y = ybeg; y < yend; ++y)
			for (int x = xbeg; x < xend; ++x)
				background[y][x] = spr.rgba;

		spr.x += spr.w;
		
		if (spr.x >= GPROJ_FB_WIDTH) {
			spr.y += spr.h;
			spr.x = 0;
		}
		if (spr.y >= GPROJ_FB_HEIGHT) {
			spr.y = 0;
		}
	}
}

void render_draw_sprites(const struct sprite* const spr, const int count)
{
	for (int i = 0; i < count; ++i) {
		const int ybeg = spr[i].y - spr[i].h;
		const int xbeg = spr[i].x - spr[i].w;
		const int yend = spr[i].y + spr[i].h;
		const int xend = spr[i].x + spr[i].w;
		for (int y = ybeg; y < yend; ++y)
			for (int x = xbeg; x < xend; ++x)
				sprites[y][x] = spr[i].rgba;
	}
}

void render_present(void)
{
	memcpy(framebuffer, background, sizeof(framebuffer));
	
	for (int i = 0; i < GPROJ_FB_HEIGHT; ++i) {
		for (int j = 0; j < GPROJ_FB_WIDTH; ++j) {
			if ((sprites[i][j]&0xFF) != 0) {
				framebuffer[i][j] = sprites[i][j];
			}
		}
	}

	video_present_framebuffer((void*)framebuffer);
}
