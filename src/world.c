#include "world.h"


void world_make_view(const struct vec2i* restrict const cam_pos,
                     const struct vec2i* restrict const world_size,
                     const int16_t* restrict const world_ts_ids,
                     int16_t* restrict const view)
{
	assert(ts_ids != NULL && view != NULL && ts_ids != view);

	const int xstart = cam_pos.x / GPROJ_TILE_WIDTH;
	const int ystart = cam_pos.y / GPROJ_TILE_HEIGHT;
	const int xtiles = (GPROJ_SCR_WIDTH / GPROJ_TILE_WIDTH);
	const int ytiles = (GPROJ_SCR_HEIGHT / GPROJ_TILE_HEIGHT);
	const uintptr_t offset = ystart * world_size.x + xstart;
	
	assert((offset + (xtyles * ytiles)) < world_size.x * world_size.y);
	
	memcpy(view, sizeof(*view) * xtiles * ytiles);
}





