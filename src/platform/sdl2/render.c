#include "logger.h"
#include "render.h"
#include "events.h"
#include "sdl2_render.h"
#include "ogl_render.h"


render_load_ts_fn_t render_load_ts;
render_load_ss_fn_t render_load_ss;
render_map_fn_t render_map;
render_ss_fn_t render_ss;
render_text_fn_t render_text;
render_finish_frame_fn_t render_finish_frame;

SDL_Window* sdl_window = NULL;
SDL_GLContext* sdl_gl_context = NULL;


enum render_modes {
	RENDER_MODE_SDL2,
	RENDER_MODE_OPENGL,
	RENDER_MODE_DEFAULT = RENDER_MODE_OPENGL,
	RENDER_NMODES = 2 
};


static void (*target_render_init)(void);
static void (*target_render_term)(void);



static void init_sdl2_window(const char* winname, const Uint32 flags)
{
	sdl_window = SDL_CreateWindow(winname,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT,
			flags);
	assert(sdl_window != NULL);
}

static void init_opengl_mode(const char* winname)
{

	target_render_init = ogl_render_init;
	target_render_term = ogl_render_term;
	render_load_ts = ogl_render_load_ts;
	render_load_ss = ogl_render_load_ss;
	render_map = ogl_render_map;
	render_ss = ogl_render_ss;
	render_text = ogl_render_text;
	render_finish_frame = ogl_render_finish_frame;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	init_sdl2_window(winname, SDL_WINDOW_OPENGL);
	sdl_gl_context = SDL_GL_CreateContext(sdl_window);
	SDL_GL_MakeCurrent(sdl_window, sdl_gl_context);
	assert(sdl_gl_context != NULL);
	SDL_GL_SetSwapInterval(0);
}

static void init_sdl2_mode(const char* winname)
{
	target_render_init = sdl2_render_init;
	target_render_term = sdl2_render_term;
	render_load_ts = sdl2_render_load_ts;
	render_load_ss = sdl2_render_load_ss;
	render_map = sdl2_render_map;
	render_ss = sdl2_render_ss;
	render_text = sdl2_render_text;
	render_finish_frame = sdl2_render_finish_frame;


	init_sdl2_window(winname, 0);
}




void render_init(const char* winname)
{
	log_dbg("INITIALIZING WINDOW");
	int err;
	((void)err);

	assert(winname != NULL);

	const enum render_modes mode = RENDER_MODE_OPENGL;
	
	switch (mode) {
	default:
	case RENDER_MODE_SDL2:
		init_sdl2_mode(winname);
		break;
	case RENDER_MODE_OPENGL:
		init_opengl_mode(winname);
		break;
	};

	target_render_init();
}

void render_term(void)
{
	target_render_term();

	if (sdl_gl_context != NULL)
		SDL_GL_DeleteContext(sdl_gl_context);

	if (sdl_window != NULL)
		SDL_DestroyWindow(sdl_window);

	log_dbg("TERMINATING WINDOW");
}


