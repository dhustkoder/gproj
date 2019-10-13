#ifndef GPROJ_WORLD_H_
#define GPROJ_WORLD_H_
#include "types.h"
#include "timer.h"
#include "map.h"
#include "actors.h"


#define GPROJ_MAX_WORLDS   (16)

#ifdef GPROJ_DEBUG
#define WORLD_DEBUG_CAM_VELOCITY (64.f)
#endif

struct world_meta {
	const char* name;
	struct map_meta map_meta;
};

struct world {
	struct vec2f cam;
	struct map map;
	struct map_view map_view;
};


extern void worldman_init(const struct world_meta* metas,
                          int cnt);
extern void worldman_load_world(const char* name);
extern void worldman_update_world(timer_clk_t now, float dt);
extern void worldman_send_render(void);
extern void worldman_term(void);







#endif

