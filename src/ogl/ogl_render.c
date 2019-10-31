#include "logger.h"
#include "render.h"


void ogl_render_init(void)
{

}

void ogl_render_term(void)
{

}

void ogl_render_layers_setup(int w, int h)
{

}

void ogl_render_load_ts(const char* const path)
{
}


void ogl_render_load_ss(const char* const path)
{
}

void ogl_render_map(const struct map_view* const view)
{

}

void ogl_render_ss(const int layer,
               const struct vec2f* restrict const wpos,
               const struct vec2i* restrict const wsize,
               const struct vec2i* restrict const spos,
               const struct vec2i* restrict const ssize,
               const render_flag_t* restrict const flags,
               const int cnt)
{
}


void ogl_render_text(const char* const text, ...)
{

}


void ogl_render_present(void)
{

}




