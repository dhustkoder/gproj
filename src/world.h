#ifndef GPROJ_WORLD_H_
#define GPROJ_WORLD_H_
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




struct world {
	int16_t* ts_ids;
	struct vec2i size;
};


extern void world_make_view(const struct vec2i* restrict const cam_pos,
                            const struct world* world,
                            int16_t* restrict const view);



#endif

