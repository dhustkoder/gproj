#include <stdint.h>
#include <string.h>
#include "render.h"
#include "video.h"

extern uint32_t map_gfx[GPROJ_FB_HEIGHT][GPROJ_FB_WIDTH];

static uint32_t sprites[GPROJ_FB_HEIGHT][GPROJ_FB_WIDTH];
static uint32_t framebuffer[GPROJ_FB_HEIGHT][GPROJ_FB_WIDTH];


void render_clear(const bool clear_map, const bool clear_sprites)
{
	if (clear_map)
		memset(map_gfx, 0, sizeof(map_gfx));
	if (clear_sprites)
		memset(sprites, 0, sizeof(sprites));
}

void render_draw_sprites(const struct sprite* const spr, const int count)
{
	for (int i = 0; i < count; ++i) {
		const int ybeg = spr[i].pos.y - spr[i].size.y / 2;
		const int xbeg = spr[i].pos.x - spr[i].size.x / 2;
		const int yend = spr[i].pos.y + spr[i].size.y / 2;
		const int xend = spr[i].pos.x + spr[i].size.x / 2;
		for (int y = ybeg; y < yend; ++y)
			for (int x = xbeg; x < xend; ++x)
				sprites[y][x] = spr[i].color.raw;
	}
}

void render_present(void)
{
	memcpy(framebuffer, map_gfx, sizeof(framebuffer));
	
	for (int i = 0; i < GPROJ_FB_HEIGHT; ++i) {
		for (int j = 0; j < GPROJ_FB_WIDTH; ++j) {
			if ((sprites[i][j]&0xFF) != 0) {
				framebuffer[i][j] = sprites[i][j];
			}
		}
	}

	video_present_framebuffer((void*)framebuffer);
}
