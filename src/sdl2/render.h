#ifndef GPROJ_RENDER_H_
#define GPROJ_RENDER_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include "types.h"
#include "actors.h"


#define GPROJ_X_TILES (8)
#define GPROJ_Y_TILES (5)

#define GPROJ_SCR_WIDTH      (256)
#define GPROJ_SCR_HEIGHT     (160)

#define GPROJ_FB_WIDTH       (24*32)
#define GPROJ_FB_HEIGHT      (160)


enum render_layer {
	RENDER_LAYER_BG      = (0x01),
	RENDER_LAYER_FG      = (0x02),
	RENDER_LAYER_ACTORS  = (0x04),
	RENDER_LAYER_NLAYERS = 3,
};


extern void render_init();

extern void render_load_ts(const char* const path);

extern void render_load_ss(const char* path);

extern void render_clear(enum render_layer layers);

extern void render_map(const int32_t* gids);

extern void render_actors(const struct recti* const ss_srcs,
                          const struct rectf* const scr_dsts,
                          const struct actor_anim* anims,
                          const int count);


extern void render_text(enum render_layer layer,
                        const struct vec2f* scrdst,
                        const char* text, ...);

extern void render_present(void);


#endif
