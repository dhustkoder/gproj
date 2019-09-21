#ifndef GPROJ_RENDER_H_
#define GPROJ_RENDER_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include "types.h"
#include "actors.h"


#define GPROJ_X_TILES        (8)
#define GPROJ_Y_TILES        (5)

#define GPROJ_TILE_WIDTH     (32)
#define GPROJ_TILE_HEIGHT    (32)

#define GPROJ_SCR_WIDTH      (GPROJ_TILE_WIDTH * GPROJ_X_TILES)
#define GPROJ_SCR_HEIGHT     (GPROJ_TILE_HEIGHT * GPROJ_Y_TILES)

#define GPROJ_TILESET_WIDTH  (1920*2)
#define GPROJ_TILESET_HEIGHT (1080)

enum render_layer {
	RENDER_LAYER_BG      = (0x01),
	RENDER_LAYER_FG      = (0x02),
	RENDER_LAYER_ACTORS  = (0x04),
	RENDER_LAYER_NLAYERS = 3,
};


extern void render_init();

extern void render_set_ts(const char* path);
extern void render_set_ss(const char* path);

extern void render_clear(enum render_layer layers);

extern void render_tile_layers(const int32_t* gids);

extern void render_update_tile_layers(const int32_t* gids,
                                      const int32_t** gids_to_update,
                                      int update_len);

extern void render_actors(const struct recti* const ss_srcs,
                          const struct rectf* const scr_dsts,
                          const struct actor_anim* anims,
                          const int count);


extern void render_text(enum render_layer layer,
                        const struct vec2f* scrdst,
                        const char* text, ...);

extern void render_present(void);


#endif
