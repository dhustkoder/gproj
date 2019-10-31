#ifndef GPROJ_OGL_RENDER_H_
#define GPROJ_OGL_RENDER_H_
#include "render.h"


extern void ogl_render_init(void);
extern void ogl_render_term(void);

extern void ogl_render_layers_setup(int w, int h);

extern void ogl_render_load_ts(const char *path);
extern void ogl_render_load_ss(const char *path);

extern void ogl_render_map(const struct map_view *view);

extern void ogl_render_ss(int layer,
					  const struct vec2f *restrict wpos,
					  const struct vec2i *restrict wsize,
					  const struct vec2i *restrict spos,
					  const struct vec2i *restrict ssize,
					  const render_flag_t *restrict flags,
					  const int cnt);

extern void ogl_render_text(const char *text, ...);
extern void ogl_render_present(void);

#endif
