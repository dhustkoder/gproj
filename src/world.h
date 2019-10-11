#ifndef GPROJ_WORLD_H_
#define GPROJ_WORLD_H_
#include "render.h"
#include "types.h"

/*
	A world is N matrixes of tiles,
	N being the number of layers
	world begins at pos 0,0 going to 512 * 32, 512 * 32
*/


#define TILE_WIDTH  (32)
#define TILE_HEIGHT (32)
#define WORLD_MAX_X_TILES (512)
#define WORLD_MAX_Y_TILES (512)


struct world_ts {
	// tile IDS in the tilesheet
	s16* tile_ids;
	// world size in tiles
	struct vec2i world_size;
	// tilesheet image pixel size
	struct vec2i ts_img_size_in_pixels;
};

struct world_map {
	// size in tiles
	struct vec2i size;	
	// positions on tilesheet
	struct vec2i map[WORLD_MAX_X_TILES * WORLD_MAX_Y_TILES]; 
};

struct world_map_view {
	// pointer to tsmap right position relative world camera
	struct vec2i map[WORLD_MAX_X_TILES * WORLD_MAX_Y_TILES];
	// size of the view area
	struct vec2i size; 
	// screen begin position
	struct vec2i scrpos;
};


extern void world_map_init(const struct world_ts* const tsmap,
                           struct world_map* const wi);

extern void world_view_update(const struct vec2f* cam,
                              const struct world_map* map,
                              struct world_map_view* view);


#endif

