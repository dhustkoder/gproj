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
	const tmx_anim_frame* frames;
	int32_t* gid_ptr;
	uint32_t clk;
	unsigned int idx;
	int cnt;
};


extern void map_load(const char* path);
extern void map_free(void);
extern bool map_is_blocking(const struct rectf* origin);
extern void map_update(uint32_t now);




#endif
