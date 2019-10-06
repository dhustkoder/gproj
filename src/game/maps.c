#include "render.h"
#include "maps.h"
#include "types.h"

struct map_info {
	const char* ts_img_path;
	const struct vec2i ts_img_size;
	const struct vec2i map_size;
	int16_t* mapping;
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

static struct map_info maps[] = {
	{
		.ts_img_path = "test-ts.png",
		.ts_img_size = { 3840, 1080 },
	        .map_size = { 12, 8 },
		.mapping = level_1_mapping
	}
};

static int wid = 0;
static struct world_info* wi;

void maps_init(void)
{
	render_load_ts(maps[wid].ts_img_path);
	wi = world_create(maps[wid].mapping, maps[wid].map_size, maps[wid].ts_img_size);
}

void maps_update(const struct vec2i* campos,
                 timer_clk_t now,
                 float dt)
{
	//world_make_view(campos, &worlds[wid], &world_view[0]);
}

void maps_send_render(void)
{
	render_ts(0, &wi->tsmap[0], wi->size);
}


