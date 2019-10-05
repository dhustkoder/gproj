#ifndef GPROJ_TILES_H_
#define GPROJ_TILES_H_
#include "types.h"

#ifndef GPROJ_TILE_WIDTH
#error "Need GPROJ_TILE_WIDTH definition"
#endif

#ifndef GPROJ_TILE_HEIGHT
#error "Need GPROJ_TILE_HEIGHT definition"
#endif

#if ((GPROJ_TILE_WIDTH%2)!=0) || ((GPROJ_TILE_HEIGHT%2)!=0)
#error "Tile dimensions must be 2 multiples"
#endif


extern void world_make_view(const struct vec2i* restrict const cam_pos,
                            const struct vec2i* restrict const world_size,
                            const int16_t* restrict const world_ts_ids,
                            int16_t* restrict const view);


#endif
