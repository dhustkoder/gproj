#include "types.h"
#include "render.h"
#include "map.h"


uint32_t map_gfx[GPROJ_FB_HEIGHT][GPROJ_FB_WIDTH];

static struct {
	const char* buf;
	struct vec2i player_init_pos;
} map_info;



void map_load(const char* data)
{
	struct vec2i pos = {0, 0};
	struct vec2i size = { GPROJ_FB_WIDTH / 64, GPROJ_FB_HEIGHT / 32 };
	struct color color;
	for (int i = 0; data[i] != '\0'; ++i) {
		switch (data[i]) {
		case 'p':
			map_info.player_init_pos = (struct vec2i) {
				pos.x + size.x / 2,
				pos.y + size.y / 2
			};
		case '.':
			color.raw = 0xABABABFF;
			break;
		case 'x':
			color.raw = 0x000000FF;
			break;
		
		}

		const int ybeg = pos.y;
		const int xbeg = pos.x;
		const int yend = pos.y + size.y;
		const int xend = pos.x + size.x;
		for (int y = ybeg; y < yend; ++y)
			for (int x = xbeg; x < xend; ++x)
				map_gfx[y][x] = color.raw;

		pos.x += size.x;
		
		if (pos.x >= GPROJ_FB_WIDTH) {
			pos.y += size.y;
			pos.x = 0;
		}
		if (pos.y >= GPROJ_FB_HEIGHT) {
			pos.y = 0;
		}
	}
}

struct vec2i map_get_player_init_pos(void)
{
	return map_info.player_init_pos;
}

