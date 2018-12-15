#ifndef GPROJ_MAP_H_
#define GPROJ_MAP_H_
#include <tmx.h>
#include "types.h"
#include "timer.h"


enum map_layer {
	MAP_LAYER_BG  = 0,
	MAP_LAYER_BG2 = 1,
	MAP_LAYER_FG  = 2,
	MAP_LAYER_FG2 = 3,
	MAP_NLAYERS   = 4
};

struct animated_tile {
	const tmx_tile* tmx_tile;
	int32_t* gid_ptr;
	unsigned int current_frame_idx;
	uint32_t frame_clk;
};


extern void map_load(const char* path);
extern void map_update(uint32_t now);




#endif
