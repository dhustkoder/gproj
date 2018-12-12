#include "types.h"
#include "render.h"
#include "map.h"


static uint32_t map_gfx[GPROJ_FB_HEIGHT][GPROJ_FB_WIDTH];


static struct {
	const char* buf;
	struct vec2i player_init_pos;
} map_info;



void map_load(const char* data)
{

}

struct vec2i map_get_player_init_pos(void)
{
	return map_info.player_init_pos;
}

