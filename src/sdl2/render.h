#ifndef GPROJ_RENDER_H_
#define GPROJ_RENDER_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include "types.h"
#include "actors.h"

typedef uint8_t render_layer_flags_t;
enum render_layer_flags {
	RENDER_LAYER_BG      = (0x01),
	RENDER_LAYER_ACTORS  = (0x02),
	RENDER_LAYER_FG      = (0x04),
	RENDER_LAYER_TXT     = (0x08),
	#ifdef GPROJ_PROFILING
	RENDER_LAYER_PROF    = (0x10),
	RENDER_LAYER_ALL     = (0x1F),
	RENDER_LAYER_NLAYERS = 5
	#else
	RENDER_LAYER_ALL     = (0x0F),
	RENDER_LAYER_NLAYERS = 4
	#endif
};

extern void render_init(const char* identifier);
extern void render_term();

extern void render_fb_setup(const struct vec2i* size);

extern void render_load_ts(const char* path);
extern void render_load_ss(const char* path);

extern void render_clear(render_layer_flags_t layers);

extern void render_map(const int32_t* gids,
                       const struct vec2i* map_size,
		       const struct vec2i* tile_size);

extern void render_actors(const struct recti* const ss_srcs,
                          const struct rectf* const scr_dsts,
                          const actor_anim_flag_t* flags,
                          const int count);


extern void render_text(const char* text, ...);

extern void render_set_camera(int x, int y);

extern void render_present(void);


#endif
