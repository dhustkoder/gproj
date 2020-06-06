#include <assert.h>
#include <string.h>
#include "render.h"
#include "tilemap.h"


void tilemap_init(
	const struct tilemap_meta* const meta, 
	struct tilemap* const map
)
{
	const struct vec2i wsize = meta->map_size;
	const struct vec2i pixsz = meta->ts_img_size;
	const s16* const tsids = meta->ts_ids;

	assert(pixsz.x <= S16_MAX && pixsz.y <= S16_MAX);

	for (int y = 0; y < wsize.y; ++y) {
		for (int x = 0; x < wsize.x; ++x) {
			const int offset = y * wsize.x + x;
			const int id = tsids[offset];
			if (id < 0) {
				map->tiles[offset].x = -1;
			} else {
				map->tiles[offset] = (struct vec2i) {
					.x = (id * TILE_WIDTH) % pixsz.x,
					.y = (((id * TILE_WIDTH) / pixsz.x) * TILE_HEIGHT)
				};	
			}
		}
	}

	map->size = wsize;

#ifdef GPROJ_DEBUG
	map->scale = MAP_DEFAULT_SCALE;
#endif
}




