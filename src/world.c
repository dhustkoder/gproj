#include <assert.h>
#include <string.h>
#include "render.h"
#include "world.h"


void world_map_init(const struct world_ts* const tsmap,
                    struct world_map* const wi)
{
	const struct vec2i wsize = tsmap->world_size;
	const struct vec2i pixsz = tsmap->img_size;
	const s16* const tsids = tsmap->tile_ids;

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

#ifdef GPROJ_DEBUG
	wi->scale = 1.0;
#endif
}


void world_view_update(const struct vec2f* const cam,
                       const struct world_map* const map,
                       struct world_map_view* const view)
{
	const int camx = cam->x;
	const int camy = cam->y;

#ifdef GPROJ_DEBUG
	assert(map->scale >= 0.1 && map->scale <= 1.1);
	const int tile_width = TILE_WIDTH * map->scale;
	const int tile_height = TILE_WIDTH * map->scale;
	view->scale = map->scale;
#else
	const int tile_width = TILE_WIDTH;
	const int tile_height = TILE_HEIGHT;
#endif

	const int scr_x_tiles = (GPROJ_SCR_WIDTH / tile_width) + 2;
	const int scr_y_tiles = (GPROJ_SCR_HEIGHT / tile_height) + 2;
	const struct vec2i mapsz = map->size;

	if (camx <= -(GPROJ_SCR_WIDTH)       ||
	    camy <= -(GPROJ_SCR_HEIGHT)      ||
	    camx >=  (mapsz.x * TILE_WIDTH)  ||
	    camy >=  (mapsz.y * TILE_HEIGHT)) {
		view->size = (struct vec2i) { 0, 0 };
		return;
	}

	const int xdiv = camx / tile_width;
	const int xmod = camx % tile_width;
	const int ydiv = camy / tile_height;
	const int ymod = camy % tile_height;

	int xtiles = scr_x_tiles;
	int ytiles = scr_y_tiles;
	const int xfirst = xdiv > 0 ? xdiv : 0;
	const int yfirst = ydiv > 0 ? ydiv : 0;

	if (xfirst + xtiles > map->size.x)
		xtiles = map->size.x - xfirst;
	if (yfirst + ytiles > map->size.y)
		ytiles = map->size.y - yfirst;

	const struct vec2i* const src = 
		&map->map[yfirst * mapsz.x + xfirst];
	for (int y = 0; y < ytiles; ++y) {
		for (int x = 0; x < xtiles; ++x) {
			view->map[y * xtiles + x] =
				src[y * mapsz.x + x];
		}
	}

	view->size = (struct vec2i) { xtiles, ytiles };
	view->scrpos = (struct vec2i) {
		camx < 0 ? -camx : -xmod,
		camy < 0 ? -camy : -ymod 
	};
}




