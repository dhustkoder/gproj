#include "logger.h"
#include "render.h"
#include "maps.h"
#include "timer.h"

struct game_map {
	const char* const ts_img_path;
	struct world_ts ts_info;
};


static s16 level_1_mapping[] = {
	0,
	1
};

static s16 level_2_mapping[] = {
	0, 1
};

static s16 level_3_mapping[] = {
	0,
	1,
	2
};

static s16 level_4_mapping[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};


static s16 level_5_mapping[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,	
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};



static struct game_map maps[] = {
	{
		.ts_img_path = "world-ts.png",
		.ts_info = { 
			.ts_img_size_in_pixels = { 512, 512 },
			.world_size = { 1, 2 },
			.tile_ids = level_1_mapping
		}
	},
	{
		.ts_img_path = "world-ts.png",
		.ts_info = { 
			.ts_img_size_in_pixels = { 512, 512 },
			.world_size = { 2, 1 },
			.tile_ids = level_2_mapping
		}
	},
	{
		.ts_img_path = "world-ts.png",
		.ts_info = { 
			.ts_img_size_in_pixels = { 512, 512 },
			.world_size = { 1, 3 },
			.tile_ids = level_3_mapping
		}	
	},
	{
		.ts_img_path = "world-ts.png",
		.ts_info = { 
			.ts_img_size_in_pixels = { 512, 512 },
			.world_size = { 12, 3 },
			.tile_ids = level_4_mapping
		}	
	},
	{
		.ts_img_path = "world-ts.png",
		.ts_info = { 
			.ts_img_size_in_pixels = { 512, 512 },
			.world_size = { 24, 10 },
			.tile_ids = level_5_mapping
		}	
	},
};

static int mapid = 0;

static struct world_map wm;
static struct world_map_view wmv;

void maps_init(void)
{
	render_load_ts(maps[mapid].ts_img_path);
	world_map_init(&maps[mapid].ts_info, &wm);
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
	extern struct events gproj_events;

	if (gproj_events.input.new_state) {
		if (gproj_events.input.buttons&INPUT_BUTTON_ACTION) {
			mapid = (mapid + 1) % STATIC_ARRAY_SIZE(maps);
			maps_init();
		}
	}
	
	world_view_update(cam, &wm, &wmv);
}

void maps_send_render(void)
{
	render_text("WORLD VIEW SCRPOS => %.3d, %.3d",
	            wmv.scrpos.x, wmv.scrpos.y);
	render_text("WORLD VIEW SIZE => %.3d, %.3d",
	            wmv.size.x, wmv.size.y);
	render_ts(0, &wmv.map[0], &wmv.size, &wmv.scrpos);
}


