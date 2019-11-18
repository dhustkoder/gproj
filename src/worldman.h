#ifndef GPROJ_WORLDMAN_H_
#define GPROJ_WORLDMAN_H_
#include "timer.h"
#include "tilemap.h"
#include "actors.h"
#include "input.h"


#define GPROJ_MAX_WORLDS   (16)

#ifdef GPROJ_DEBUG
#define WORLD_DEBUG_CAM_VELOCITY (64.f)
#endif

struct world_meta {
	const char* name;
	struct tilemap_meta tm_meta;
};

struct world {
	struct vec2f cam;
	struct tilemap tm;
};


extern void worldman_init(const struct world_meta* metas,
                          int cnt);
extern void worldman_load_world(const char* name);
extern void worldman_update_world(input_t input, timer_clk_t now, float dt);
extern void worldman_send_render(void);
extern void worldman_term(void);







#endif

