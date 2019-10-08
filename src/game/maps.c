#include "render.h"
#include "maps.h"
#include "types.h"

struct game_map {
	const char* const ts_img_path;
	struct world_ts ts_info;
};


static int16_t level_1_mapping[] = {
	0,
	1
};


static struct game_map maps[] = {
	{
		.ts_img_path = "world-ts.png",
		.ts_info = { 
			.ts_img_size_in_pixels = { 512, 512 },
			.world_size = { 1, 2 },
			.tile_ids = level_1_mapping
		}
	}
};

static int mapid = 0;

static struct world_map wm;
static struct world_map_view wmv;

void maps_init(void)
{
	render_load_ts(maps[mapid].ts_img_path);
	world_map_fill(&maps[mapid].ts_info, &wm);
}

void maps_term(void)
{

}

void maps_update(const struct vec2f* restrict const cam,
                 timer_clk_t now,
                 float dt)
{
	((void)cam);
	((void)now);
	((void)dt);
	world_view_fill(cam, &wm, &wmv);
}

void maps_send_render(void)
{
	render_ts(0, &wmv.map[0], &wmv.size, &wmv.shift);
}


