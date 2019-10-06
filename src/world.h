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



struct world_info {
	struct vec2i size;    // world size in tiles
	struct vec2i tsmap[]; // world tiles positions in the tilesheet 
};

struct world_view {
	struct vec2i* tspos; // pointer to tsmap right position relative world camera
	struct vec2i size;   // size in tiles that fills the screen 
};


static inline struct world_info* world_create(const int16_t* const restrict tsids, 
                                              const struct vec2i wsize,
                                              const struct vec2i ts_pix_size)
{
	struct world_info* const wi = 
		malloc(sizeof(*wi) + (sizeof(wi->tsmap[0]) * wsize.x * wsize.y));

	assert(wi != NULL);
	wi->size = wsize;

	for (int y = 0; y < wsize.y; ++y) {
		for (int x = 0; x < wsize.x; ++x) {
			const int offset = y * wsize.x + x;
			const int id = tsids[offset] - 1;
			if (id < 0) {
				wi->tsmap[offset].x = -1;
				continue;
			}
			wi->tsmap[offset] = (struct vec2i) {
				.x = (id * GPROJ_TILE_WIDTH) % ts_pix_size.x,
				.y = ((id * GPROJ_TILE_WIDTH) / ts_pix_size.x) * GPROJ_TILE_HEIGHT
			};
		}
	}

	return wi;
}

static inline void world_destroy(struct world_info* w)
{
	free(w);
}


#endif

