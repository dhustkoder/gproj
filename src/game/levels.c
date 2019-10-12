#include "worldman.h"


static s16 level_1_mapping[] = {
	0,
	1
};
/*
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
*/


struct world_meta levels[1] = {
	{
		.name = "first",
		.map_meta = {
			.ts_path     = "world-ts.png",
			.ts_img_size = { 512, 512 },
			.map_size  = { 1, 2 },
			.ts_ids    = level_1_mapping
		}
	},
	/*
	{
		.ts_path = "world-ts.png",
		.ts_img_size = { 512, 512 },
		.map_size = { 2, 1 },
		.ts_ids = level_2_mapping
	},
	{
		.ts_path = "world-ts.png",
		.ts_img_size = { 512, 512 },
		.map_size = { 1, 3 },
		.ts_ids = level_3_mapping
	},
	{
		.ts_path = "world-ts.png",
		.ts_img_size = { 512, 512 },
		.map_size = { 12, 3 },
		.ts_ids = level_4_mapping
	},
	{
		.ts_path = "world-ts.png",
		.ts_img_size = { 512, 512 },
		.map_size = { 24, 10 },
		.ts_ids = level_5_mapping
	}
	*/
};

