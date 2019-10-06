#ifndef GPROJ_WORLD_H_
#define GPROJ_WORLD_H_
#include <stdlib.h>
#include <assert.h>
#include "types.h"

/*
	A world is N matrixes of tiles,
	N being the number of layers
*/


#ifndef GPROJ_TILE_WIDTH
#error "Need GPROJ_TILE_WIDTH definition"
#endif

#ifndef GPROJ_TILE_HEIGHT
#error "Need GPROJ_TILE_HEIGHT definition"
#endif

#if ((GPROJ_TILE_WIDTH % 2) != 0) || ((GPROJ_TILE_HEIGHT % 2) != 0)
#error "Tile dimensions must be 2 multiples"
#endif

#define GPROJ_X_TILES (GPROJ_SCR_WIDTH / GPROJ_TILE_WIDTH)
#define GPROJ_Y_TILES (GPROJ_SCR_HEIGHT / GPROJ_TILE_HEIGHT)


struct world_ts {
	int16_t* map;
	struct vec2i map_size;
	struct vec2i ts_img_size_in_pixels;
};

struct world_map {
	struct vec2i size;  // world size in tiles
	struct vec2i map[]; // world tiles positions in the tilesheet 
};

struct world_map_view {
	struct vec2i* tspos; // pointer to tsmap right position relative world camera
	struct vec2i size;   // size in tiles that fills the screen 
};


extern void world_map_fill(const struct world_ts* restrict const tsmap,
                           struct world_map* restrict const wi);


#endif

