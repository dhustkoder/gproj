#ifndef GPROJ_MAP_H_
#define GPROJ_MAP_H_
#include "gmath.h"

/*
	A map is N matrixes of tiles,
	N being the number of layers
	map begins at pos 0,0
*/


#define TILE_WIDTH      (32)
#define TILE_HEIGHT     (32)
#define MAP_MAX_X_TILES (512)
#define MAP_MAX_Y_TILES (512)

#ifdef GPROJ_DEBUG
#define MAP_DEFAULT_SCALE (1.0f)
#define MAP_MAX_SCALE     (5.0f)
#define MAP_MIN_SCALE     (0.1f)
#define MAP_SCALE_MOD     (0.1f)
#endif

struct tilemap_meta {
	// ts file path
	const char* ts_path;
	// tile IDS in the tilesheet
	s16* ts_ids;
	// map size in tiles
	struct vec2i map_size;
	// tilesheet image pixel size
	struct vec2i ts_img_size;
};

struct tilemap {
#ifdef GPROJ_DEBUG
	// map tile size scale factor
	f32 scale;
#endif
	// size in tiles
	struct vec2i size;
	// positions on tilesheet
	struct vec2i tiles[MAP_MAX_X_TILES * MAP_MAX_Y_TILES];
};



extern void tilemap_init(
	const struct tilemap_meta* const meta,
	struct tilemap* const map
);


#endif

