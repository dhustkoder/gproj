#include <assert.h>
#include <string.h>
#include "world.h"


void world_make_view(const struct vec2i* restrict const cam_pos,
                     const struct world* const world,
                     int16_t* restrict const view)
{
	assert(world->ts_ids != NULL &&
	       view != NULL &&
	       world->ts_ids != view);

	const int xstart = cam_pos->x / GPROJ_TILE_WIDTH;
	const int ystart = cam_pos->y / GPROJ_TILE_HEIGHT;
	const int xtiles = (GPROJ_SCR_WIDTH / GPROJ_TILE_WIDTH);
	const int ytiles = (GPROJ_SCR_HEIGHT / GPROJ_TILE_HEIGHT);
	const int offset = ystart * world->size.x + xstart;
	
	assert((offset + (xtiles * ytiles)) < world->size.x * world->size.y);
	
	memcpy(view, world->ts_ids + offset, sizeof(*view) * xtiles * ytiles);
}





