#ifndef GPROJ_RENDER_H_
#define GPROJ_RENDER_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include "types.h"
#include "world.h"

#define GPROJ_SCR_WIDTH      (360)
#define GPROJ_SCR_HEIGHT     (240)
#define GPROJ_RENDER_NLAYERS (1)
#define GPROJ_TS_MAX_WIDTH   (S16_MAX - 1)
#define GPROJ_TS_MAX_HEIGHT  (S16_MAX - 1)


#ifndef GPROJ_RENDER_NLAYERS
#error "Need GPROJ_RENDER_NLAYERS definition"
#endif

#if GPROJ_RENDER_NLAYERS <= 0 || GPROJ_RENDER_NLAYERS > 8
#error "GPROJ_RENDER_NLAYERS out of range"
#endif 



enum render_flag {
	RENDER_FLAG_FLIPH = SDL_FLIP_HORIZONTAL,
	RENDER_FLAG_FLIPV = SDL_FLIP_VERTICAL
};

typedef int render_flag_t;


extern void render_init(const char* identifier);
extern void render_term();

extern void render_layers_setup(int w, int h);

extern void render_load_ts(const char* path);
extern void render_load_ss(const char* path);

extern void render_world(const struct world_map_view* view);

extern void render_ss(int layer,
                      const struct vec2f* restrict wpos,
                      const struct vec2i* restrict wsize,
                      const struct vec2i* restrict spos,
                      const struct vec2i* restrict ssize,
                      const render_flag_t* restrict flags,
                      const int cnt);

extern void render_text(const char* text, ...);
extern void render_present(void);


#endif
