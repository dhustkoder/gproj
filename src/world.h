#ifndef GPROJ_WORLD_H_
#define GPROJ_WORLD_H_
#include "types.h"

/*
	A world is N matrixes of tiles,
	N being the number of layers
	world begins at pos 0,0
*/


#define TILE_WIDTH        (32)
#define TILE_HEIGHT       (32)
#define WORLD_MAX_X_TILES (512)
#define WORLD_MAX_Y_TILES (512)


struct world_ts {
	// ts file path
	const char* path;
	// tile IDS in the tilesheet
	s16* tile_ids;
	// world size in tiles
	struct vec2i world_size;
	// tilesheet image pixel size
	struct vec2i img_size;
};

struct world_map {
#ifdef GPROJ_DEBUG
	// world tile size scale factor
	f32 scale;
#endif
	// size in tiles
	struct vec2i size;	
	// positions on tilesheet
	struct vec2i map[WORLD_MAX_X_TILES * WORLD_MAX_Y_TILES]; 
};

struct world_map_view {
#ifdef GPROJ_DEBUG
	// world tile size scale factor
	f32 scale;
#endif
	// size of the view area
	struct vec2i size; 
	// screen begin position
	struct vec2i scrpos;
	// pointer to tsmap right position relative world camera
	struct vec2i map[WORLD_MAX_X_TILES * WORLD_MAX_Y_TILES];
};


extern void world_map_init(const struct world_ts* const tsmap,
                           struct world_map* const wi);

extern void world_view_update(const struct vec2f* cam,
                              const struct world_map* map,
                              struct world_map_view* view);


#endif

