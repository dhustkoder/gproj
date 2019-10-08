#ifndef GPROJ_WORLD_H_
#define GPROJ_WORLD_H_
#include <stdlib.h>
#include <assert.h>
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
#define SCR_MAX_X_TILES ((GPROJ_SCR_WIDTH / TILE_WIDTH) + 2)
#define SCR_MAX_Y_TILES ((GPROJ_SCR_HEIGHT / TILE_HEIGHT) + 2)


struct world_ts {
	// tile IDS in the tilesheet
	int16_t* tile_ids;
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
	struct vec2i map[SCR_MAX_X_TILES * SCR_MAX_Y_TILES]; 
	// size of the view area
	struct vec2i size; 
	// border tiles pixels to scroll	
	struct vec2i shift;
};


extern void world_map_fill(const struct world_ts* restrict const tsmap,
                           struct world_map* restrict const wi);

extern void world_view_fill(const struct vec2f* restrict cam,
                            const struct world_map* restrict map,
			    struct world_map_view* restrict view);

#endif

