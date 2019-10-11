#include "logger.h"
#include "render.h"
#include "maps.h"
#include "timer.h"


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



static struct world_ts maps[] = {
	{
		.path = "world-ts.png",
		.img_size = { 512, 512 },
		.world_size = { 1, 2 },
		.tile_ids = level_1_mapping
	},
	{
		.path = "world-ts.png",
		.img_size = { 512, 512 },
		.world_size = { 2, 1 },
		.tile_ids = level_2_mapping
	},
	{
		.path = "world-ts.png",
		.img_size = { 512, 512 },
		.world_size = { 1, 3 },
		.tile_ids = level_3_mapping
	},
	{
		.path = "world-ts.png",
		.img_size = { 512, 512 },
		.world_size = { 12, 3 },
		.tile_ids = level_4_mapping
	},
	{
		.path = "world-ts.png",
		.img_size = { 512, 512 },
		.world_size = { 24, 10 },
		.tile_ids = level_5_mapping
	}
};

static s8 mapid = 0;
static struct world_map wm;
static struct world_map_view wmv;

void maps_init(void)
{
	render_load_ts(maps[mapid].path);
	world_map_init(&maps[mapid], &wm);
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

#ifdef GPROJ_DEBUG	
	extern struct events gproj_events;
	if (gproj_events.input.new_state) {
		const input_button_t buttons = gproj_events.input.buttons;

		if (buttons&INPUT_BUTTON_ACTION) {
			mapid = (mapid + 1) % STATIC_ARRAY_SIZE(maps);
			maps_init();
		}

		if (buttons&INPUT_BUTTON_WORLD_SCALE_DOWN && wm.scale >= 0.2)
			wm.scale -= 0.1;
		
		if (buttons&INPUT_BUTTON_WORLD_SCALE_UP && wm.scale <= 0.9)
			wm.scale += 0.1;
	}
#endif

	world_view_update(cam, &wm, &wmv);
}

void maps_send_render(void)
{
	render_text("WORLD VIEW SCRPOS => %.5d, %.5d",
	            wmv.scrpos.x, wmv.scrpos.y);
	render_text("WORLD VIEW SIZE => %.5d, %.5d",
	            wmv.size.x, wmv.size.y);
	render_world(&wmv);
}


