#ifndef GPROJ_SDL2_RENDER_H_
#define GPROJ_SDL2_RENDER_H_
#include <SDL.h>
#include "render.h"


extern void sdl2_render_init(void);
extern void sdl2_render_term(void);

extern void sdl2_render_load_ts(const char *path);
extern void sdl2_render_load_ss(const char *path);

extern void sdl2_render_tilemap(const struct vec2f* cam, const struct tilemap *map);

extern void sdl2_render_ss(
	int layer,
	const struct vec2f *restrict wpos,
	const struct vec2i *restrict wsize,
	const struct vec2i *restrict spos,
	const struct vec2i *restrict ssize,
	const render_flag_t *restrict flags,
	const int cnt
);

extern void sdl2_render_text(const char *text, ...);
extern void sdl2_render_finish_frame(void);

#endif
