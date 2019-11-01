#include "logger.h"
#include "render.h"
#include "sdl2_render.h"
#include "ogl_render.h"


render_layers_setup_fn_t render_layers_setup;
render_load_ts_fn_t render_load_ts;
render_load_ss_fn_t render_load_ss;
render_map_fn_t render_map;
render_ss_fn_t render_ss;
render_text_fn_t render_text;
//render_start_frame_fn_t render_start_frame;
render_finish_frame_fn_t render_finish_frame;

SDL_Window* sdl_window = NULL;
SDL_GLContext* sdl_gl_context = NULL;


static void (*target_render_init)(void);
static void (*target_render_term)(void);



void render_init(const char* const name,
                 const render_mode_t mode)
{
	LOG_DEBUG("INITIALIZING WINDOW");
	int err;
	((void)err);

	assert(name != NULL);

	int window_flags = SDL_WINDOW_RESIZABLE;

	switch (mode) {
	case RENDER_MODE_DEFAULT:
		window_flags = 0;
		target_render_init = sdl2_render_init;
		target_render_term = sdl2_render_term;
		render_layers_setup = sdl2_render_layers_setup;
		render_load_ts = sdl2_render_load_ts;
		render_load_ss = sdl2_render_load_ss;
		render_map = sdl2_render_map;
		render_ss = sdl2_render_ss;
		render_text = sdl2_render_text;
		//render_start_frame = sdl2_render_start_frame;
		render_finish_frame = sdl2_render_finish_frame;
		break;
	case RENDER_MODE_OPENGL:
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		window_flags |= SDL_WINDOW_OPENGL;
		target_render_init = ogl_render_init;
		target_render_term = ogl_render_term;
		render_layers_setup = ogl_render_layers_setup;
		render_load_ts = ogl_render_load_ts;
		render_load_ss = ogl_render_load_ss;
		render_map = ogl_render_map;
		render_ss = ogl_render_ss;
		render_text = ogl_render_text;
		//render_start_frame = ogl_render_start_frame;
		render_finish_frame = ogl_render_finish_frame;
		break;
	default:
		assert(false && "INVALID PATH");
		break;
	};

	sdl_window = SDL_CreateWindow(name,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT,
			window_flags);
	assert(sdl_window != NULL);

	sdl_gl_context = SDL_GL_CreateContext(sdl_window);
	assert(sdl_gl_context != NULL);

	target_render_init();
}

void render_term(void)
{
	target_render_term();

	if (sdl_gl_context != NULL)
		SDL_GL_DeleteContext(sdl_gl_context);

	if (sdl_window != NULL)
		SDL_DestroyWindow(sdl_window);

	LOG_DEBUG("TERMINATING WINDOW");
}

