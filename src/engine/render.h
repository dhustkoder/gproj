#ifndef GPROJ_RENDER_H_
#define GPROJ_RENDER_H_
#include <stdarg.h>
#include "gmath.h"
#include "tilemap.h"


#define GPROJ_SCR_WIDTH      (480)
#define GPROJ_SCR_HEIGHT     (360)
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
	RENDER_FLAG_FLIP_H,
	RENDER_FLAG_FLIP_V
};
typedef int render_flag_t;



typedef void(*render_load_ts_fn_t)(const char *path);
typedef void(*render_load_ss_fn_t)(const char *path);
typedef void(*render_tilemap_fn_t)(const struct vec2f* cam, const struct tilemap *tm);
typedef void(*render_ss_fn_t)(int layer,
			  const struct vec2f *restrict wpos,
			  const struct vec2i *restrict wsize,
			  const struct vec2i *restrict spos,
			  const struct vec2i *restrict ssize,
			  const render_flag_t *restrict flags,
			  const int cnt);
typedef void(*render_text_fn_t)(const char *text, ...);
typedef void(*render_finish_frame_fn_t)(void);
typedef void(*render_resize_fn_t)(int widht, int height);


extern void render_init(const char* winname);
extern void render_term(void);



extern render_load_ts_fn_t render_load_ts;
extern render_load_ss_fn_t render_load_ss;
extern render_tilemap_fn_t render_tilemap;
extern render_ss_fn_t render_ss;
extern render_text_fn_t render_text;
extern render_finish_frame_fn_t render_finish_frame;
extern render_resize_fn_t render_resize;



#endif

