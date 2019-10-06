#include <assert.h>
#include <string.h>
#include "world.h"


void world_map_fill(const struct world_ts* restrict const tsmap,
                    struct world_map* restrict const wi)
{
	const struct vec2i wsize = tsmap->map_size;
	const struct vec2i pixsz = tsmap->ts_img_size_in_pixels;
	const int16_t* const tsids = tsmap->map;

	for (int y = 0; y < wsize.y; ++y) {
		for (int x = 0; x < wsize.x; ++x) {
			const int offset = y * wsize.x + x;
			const int id = tsids[offset] - 1;
			if (id < 0) {
				wi->map[offset].x = -1;
				continue;
			}
			wi->map[offset] = (struct vec2i) {
				.x = (id * GPROJ_TILE_WIDTH) % pixsz.x,
				.y = (((id * GPROJ_TILE_WIDTH) / pixsz.x) 
				     * GPROJ_TILE_HEIGHT)
			};
		}
	}

	wi->size = wsize;
}
  
