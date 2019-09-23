#ifndef GPROJ_MAP_H_
#define GPROJ_MAP_H_
#include <tmx.h>
#include "types.h"


enum map_layer {
	MAP_LAYER_BG  = 0,
	MAP_LAYER_FG  = 1,
	MAP_NLAYERS   = 2
};


extern void map_load(const char* path);
extern void map_free(void);
extern void map_update(timer_clk_t now, float dt);




#endif
