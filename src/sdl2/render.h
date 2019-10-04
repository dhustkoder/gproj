#ifndef GPROJ_RENDER_H_
#define GPROJ_RENDER_H_
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include "types.h"


enum render_flag {
	RENDER_FLAG_FLIPH = SDL_FLIP_HORIZONTAL,
	RENDER_FLAG_FLIPV = SDL_FLIP_VERTICAL
};

typedef int render_flag_t;


extern void render_init(const char* identifier);
extern void render_term();

extern void render_layers_setup(struct vec2i size, int cnt);

extern void render_load_ts(const char* path);
extern void render_load_ss(const char* path);

extern void render_ss(int layer,
                      const struct vec2f* wpos,
                      const struct vec2i* wsize,
                      const struct vec2i* spos,
                      const struct vec2i* ssize,
                      const render_flag_t* flags,
                      const int cnt);

extern void render_text(const char* text, ...);
extern void render_present(void);


#endif
