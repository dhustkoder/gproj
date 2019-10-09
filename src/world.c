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
	const s32 camx = cam->x;
	const s32 camy = cam->y;
	const s32 scr_x_tiles = (GPROJ_SCR_WIDTH / TILE_WIDTH) + 2;
	const s32 scr_y_tiles = (GPROJ_SCR_HEIGHT / TILE_HEIGHT) + 2;

	if (camx <= -(GPROJ_SCR_WIDTH)  ||
	    camy <= -(GPROJ_SCR_HEIGHT) ||
	    camx >=  (map->size.x * TILE_WIDTH)  ||
	    camy >=  (map->size.y * TILE_HEIGHT)) {
		view->size = (struct vec2i) { 0, 0 };
		return;
	}

	const s32 xdiv = camx / TILE_WIDTH;
	const s32 xmod = camx % TILE_WIDTH;
	const s32 ydiv = camy / TILE_HEIGHT;
	const s32 ymod = camy % TILE_HEIGHT;

	s32 xtiles = scr_x_tiles;
	s32 ytiles = scr_y_tiles;
	s32 xfirst = xdiv > 0 ? xdiv : 0;
	s32 yfirst = ydiv > 0 ? ydiv : 0;

	if (xfirst + xtiles > map->size.x)
		xtiles = map->size.x - xfirst;
	if (yfirst + ytiles > map->size.y)
		ytiles = map->size.y - yfirst;

	const struct vec2i* const src = &map->map[yfirst * map->size.x + xfirst];
	for (s32 y = 0; y < ytiles; ++y) {
		for (s32 x = 0; x < xtiles; ++x) {
			view->map[y * xtiles + x] = src[y * map->size.x + x];
		}
	}

	view->size = (struct vec2i) { xtiles, ytiles };
	view->scrpos = (struct vec2i) {
		camx < 0 ? -camx : -xmod,
		camy < 0 ? -camy : -ymod 
	};
}




