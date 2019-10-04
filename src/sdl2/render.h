#ifndef GPROJ_RENDER_H_
#define GPROJ_RENDER_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include "types.h"
#include "actors.h"


extern void render_init(const char* identifier);
extern void render_term();

extern void render_fb_setup(const struct vec2i* size);

extern void render_load_ts(const char* path);
extern void render_load_ss(const char* path);


extern void render_map(const int32_t* gids,
                       const struct vec2i* map_size,
                       const struct vec2i* tile_size);

extern void render_ss(const struct recti* const srects,
                      const struct rectf* const wrects,
                      const actor_flag_t* flags,
                      const int count);


extern void render_text(const char* text, ...);

extern void render_set_camera(int x, int y);

extern void render_present(void);


#endif
