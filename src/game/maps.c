#include "render.h"
#include "maps.h"
#include "types.h"

struct game_map {
	const char* const ts_img_path;
	struct world_ts ts_info;
};


static int16_t level_1_mapping[] = {
	1,2,3,4,5,6,7,8,9,10,11,12,
	121,122,123,124,125,126,127,128,129,130,131,132,
	241,242,243,244,245,246,247,248,249,250,251,252,
	361,362,363,364,365,366,367,368,369,370,371,372,
	481,482,483,484,485,486,487,488,489,490,491,492,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0
};


static int16_t level_2_mapping[] = {
	1,2,3,4,5,6,7,8,9,10,11,12,1,2,3,4,5,6,7,8,9,10,11,12,
	121,122,123,124,125,126,127,128,129,130,131,132,121,122,123,124,125,126,127,128,129,130,131,132,
	241,242,243,244,245,246,247,248,249,250,251,252,241,242,243,244,245,246,247,248,249,250,251,252,
	361,362,363,364,365,366,367,368,369,370,371,372,361,362,363,364,365,366,367,368,369,370,371,372,
	481,482,483,484,485,486,487,488,489,490,491,492,481,482,483,484,485,486,487,488,489,490,491,492,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static struct game_map maps[] = {
	{
		.ts_img_path = "test-ts.png",
		.ts_info = { 
			.ts_img_size_in_pixels = { 3840, 1080 },
			.map_size = { 12, 8 },
			.map = level_1_mapping
		}
	},
	{
		.ts_img_path = "test-ts.png",
		.ts_info = {
			.ts_img_size_in_pixels = { 3840, 1080 },
			.map_size = { 24, 8 },
			.map = level_2_mapping
		}
	}
};

static int wid = 1;
#define WM_BUFF_SIZE (256 * 256)
static unsigned char world_map_buffer [
	sizeof(struct world_map) + 
	(sizeof(((struct world_map*)0)->map[0]) * WM_BUFF_SIZE)
];

static struct world_map* wi = (void*)&world_map_buffer[0];
static struct world_map_view wv;

void maps_init(void)
{
	render_load_ts(maps[wid].ts_img_path);
	world_map_fill(&maps[wid].ts_info, wi);
}

void maps_term(void)
{

}

void maps_update(const struct vec2i* campos,
                 timer_clk_t now,
                 float dt)
{
	((void)campos);
	((void)now);
	((void)dt);

	wv = (struct world_map_view) {
		.map = wi->map,
		.size = { 
			24,
			8
		}
	};

}

void maps_send_render(void)
{
	render_ts(0, wv.map, wv.size);
}


