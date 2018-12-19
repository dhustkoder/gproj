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


extern void map_load(const char* path);
extern void map_free(void);
extern bool map_is_blocking(const struct rectf* origin);
extern void map_update(uint32_t now);




#endif
