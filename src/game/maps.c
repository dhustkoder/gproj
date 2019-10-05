#include "render.h"
#include "maps.h"
#include "types.h"


static int16_t level_1[] = {
	1,2,3,4,5,6,7,8,9,10,11,12,
	121,122,123,124,125,126,127,128,129,130,131,132,
	241,242,243,244,245,246,247,248,249,250,251,252,
	361,362,363,364,365,366,367,368,369,370,371,372,
	481,482,483,484,485,486,487,488,489,490,491,492,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0
};

static int16_t level_2[] = {
	1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
	121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,
	241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,
	361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,381,382,383,384,
	481,482,483,484,485,486,487,488,489,490,491,492,493,494,495,496,497,498,499,500,501,502,503,504
};


static const char* worlds_ts[GAME_NWORLDS] = {
	"test-ts.png"
};

static struct world worlds[GAME_NWORLDS] = {
	{
		.size = { .x = 12, .y = 8 },
		.ts_ids = level_1
	}	
};


static int wid = 0;


static int16_t world_view[GPROJ_X_TILES * GPROJ_Y_TILES];


void maps_init(void)
{
	render_load_ts(worlds_ts[wid]);
}

void maps_update(const struct vec2i* campos,
                 timer_clk_t now,
                 float dt)
{
	world_make_view(campos, &worlds[wid], &world_view[0]);
}

void maps_send_render(void)
{
	render_ts(0, &level_1[0]);
}


