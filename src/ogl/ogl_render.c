#include <stdarg.h>
#include "logger.h"
#include "ogl_render.h"



void ogl_render_init(void)
{
	LOG_DEBUG("INITIALIZING OPENGL RENDER");

}

void ogl_render_term(void)
{
	LOG_DEBUG("TERMINATING OPENGL RENDER");
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
	va_list args;
	va_start(args, text);
	vprintf(text, args);
	printf("\n");
	va_end(args);
}


void ogl_render_present(void)
{

}




