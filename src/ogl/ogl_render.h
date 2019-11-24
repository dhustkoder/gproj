#ifndef GPROJ_OGL_RENDER_H_
#define GPROJ_OGL_RENDER_H_
#include "ogl_defs.h"
#include "render.h"


extern void ogl_render_init(void);
extern void ogl_render_term(void);

extern void ogl_render_load_ts(const char *path);
extern void ogl_render_load_ss(const char *path);

extern void ogl_render_tilemap(const struct vec2f* cam, const struct tilemap *map);

extern void ogl_render_ss(int layer,
					  const struct vec2f *restrict wpos,
					  const struct vec2i *restrict wsize,
					  const struct vec2i *restrict spos,
					  const struct vec2i *restrict ssize,
					  const render_flag_t *restrict flags,
					  const int cnt);

extern void ogl_render_text(const char *text, ...);
extern void ogl_render_finish_frame(void);



#endif
