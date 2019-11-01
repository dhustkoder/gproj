#include <stdarg.h>
#include <GL/gl.h>
#include "logger.h"
#include "ogl_render.h"


glCreateShader_fn_t glCreateShader;
glShaderSource_fn_t glShaderSource;
glCompileShader_fn_t glCompileShader;
glShaderiv_fn_t glShaderiv;
glGetShaderInfoLog_fn_t glGetShaderInfoLog;



static GLchar* gl_proc_names[] = {
	"glCreateShader",
	"glShaderSource",
	"glCompileShader",
	"glShaderiv",
	"glGetShaderInfoLog"
};

static gl_void_proc_fn_t* gl_proc_ptrs[] = {
	&glCreateShader,
	&glShaderSource,
	&glCompileShader,
	&glShaderiv,
	&glGetShaderInfoLog
};

STATIC_ASSERT(
	GL_PROCS_ARRAYS,
	STATIC_ARRAY_SIZE(gl_proc_names) == STATIC_ARRAY_SIZE(gl_proc_ptrs)
);

static GLuint vs_id;
static GLuint fs_id;

#ifdef GPROJ_DEBUG
#define SHADER_COMPILATION_INFO_BUFFER_SIZE (512)
static GLchar shader_compilation_info_buffer[SHADER_COMPILATION_INFO_BUFFER_SIZE];
#endif

const GLchar* vs_source =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";



static void ogl_shaders_set_vs(const GLchar* source)
{
	vs_id = glCreateShader(GL_VERTEX_SHADER);
	assert(vs_id != 0);

	const GLchar* src[] = { source };
	const GLint length[] = { strlen(source) };
	glShaderSource(vs_id, 1, src, length);
	glCompileShader(vs_id);

#ifdef GPROJ_DEBUG
	GLint status;
	glGetShaderiv(vs_id, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderInfoLog(vs_id, SHADER_COMPILATION_INFO_BUFFER_SIZE,
		                   NULL, shader_compilation_info_buffer);
		LOG_DEBUG("FAILED TO COMPILE SHADER: %s", shader_compilation_info_buffer);
		assert(false && "FAILED SHADER COMPILATION");
	}
#endif

}

static void ogl_shaders_set_fs(const char* source)
{

}



void ogl_render_init(void)
{
	LOG_DEBUG("INITIALIZING OPENGL RENDER");
	for (int i = 0; i < STATIC_ARRAY_SIZE(gl_proc_names); ++i) {
		gl_void_proc_fn_t proc = OGL_GET_PROC_ADDR(gl_proc_names[i]);
		assert(proc != NULL);
		*gl_proc_ptrs[i] = proc;
	}

	ogl_shaders_set_vs(vs_source);
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

void ogl_render_finish_frame(void)
{
	glClearColor(0, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);


	#ifdef PLATFORM_SDL2
	extern SDL_GLContext* sdl_gl_context;
	extern SDL_Window* sdl_window;
	SDL_GL_SwapWindow(sdl_window);
	#else
	#error "NEED IMPLEMENTATION"
	#endif
}


