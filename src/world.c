#include <assert.h>
#include <string.h>
#include "world.h"


void world_map_fill(const struct world_ts* restrict const tsmap,
                    struct world_map* restrict const wi)
{
	const struct vec2i wsize = tsmap->world_size;
	const struct vec2i pixsz = tsmap->ts_img_size_in_pixels;
	const int16_t* const tsids = tsmap->tile_ids;

	for (int y = 0; y < wsize.y; ++y) {
		for (int x = 0; x < wsize.x; ++x) {
			const int offset = y * wsize.x + x;
			const int id = tsids[offset];
			if (id < 0) {
				wi->map[offset].x = -1;
				continue;
			}
			wi->map[offset] = (struct vec2i) {
				.x = (id * TILE_WIDTH) % pixsz.x,
				.y = (((id * TILE_WIDTH) / pixsz.x) 
				     * TILE_HEIGHT)
			};
		}
	}

	wi->size = wsize;
}
  


void world_view_fill(const struct vec2f* restrict const cam,
                     const struct world_map* restrict const map,
                     struct world_map_view* restrict const view)
{
	memset(view->map, 0xFF, sizeof(view->map[0]) * SCR_MAX_X_TILES * SCR_MAX_Y_TILES);
	int xshift, yshift;

	const int camx = cam->x;
	const int camy = cam->y;
	
	if (camx < 0) {
		xshift = - ((-camx) % TILE_WIDTH);
	} else {
		xshift = camx % TILE_WIDTH;
	}

	if (camy < 0) {
		yshift = -((-camy) % TILE_HEIGHT);
	} else {
		yshift = camy % TILE_WIDTH;
	}


	memcpy(view->map, map->map, sizeof(view->map[0]) * 1 * 2);
	view->size = (struct vec2i) { 1, 2 };
	view->shift = (struct vec2i) { xshift, yshift };
}




