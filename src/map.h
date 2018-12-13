#ifndef GPROJ_MAP_H_
#define GPROJ_MAP_H_
#include <tmx.h>
#include "types.h"
#include "timer.h"


struct animated_tile {
	const tmx_tile* tmx_tile;
	int32_t* map_gids_ptr;
	unsigned int current_frame_idx;
	uint32_t frame_clk;
};


extern void map_load(const char* path);
extern void map_update(void);




#endif
