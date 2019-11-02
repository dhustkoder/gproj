#include <stdarg.h>
#include <stdio.h>
#include "logger.h"
#include "ogl_render.h"

#ifndef GL_VERSION_2_0
glCreateShader_fn_t glCreateShader;
glShaderSource_fn_t glShaderSource;
glCompileShader_fn_t glCompileShader;
glGetShaderiv_fn_t glGetShaderiv;
glGetShaderInfoLog_fn_t glGetShaderInfoLog;
glCreateProgram_fn_t glCreateProgram;
glAttachShader_fn_t glAttachShader;
glLinkProgram_fn_t glLinkProgram;
glGetProgramiv_fn_t glGetProgramiv;
glGetProgramInfoLog_fn_t glGetProgramInfoLog;

#endif


#ifndef GL_VERSION_2_0
static GLchar* gl_proc_names[] = {
	"glCreateShader",
	"glShaderSource",
	"glCompileShader",
	"glGetShaderiv",
	"glGetShaderInfoLog",
	"glCreateProgram",
	"glAttachShader",
	"glLinkProgram",
	"glGetProgramiv",
	"glGetProgramInfoLog"
};

static gl_void_proc_fn_t* gl_proc_ptrs[] = {
	&glCreateShader,
	&glShaderSource,
	&glCompileShader,
	&glGetShaderiv,
	&glGetShaderInfoLog,
	&glCreateProgram,
	&glAttachShader,
	&glLinkProgram,
	&glGetProgramiv,
	&glGetProgramInfoLog
};

STATIC_ASSERT(
	GL_PROCS_ARRAYS,
	STATIC_ARRAY_SIZE(gl_proc_names) == STATIC_ARRAY_SIZE(gl_proc_ptrs)
);
#endif

static GLuint vs_id;
static GLuint fs_id;
static GLuint shader_program_id;

#ifdef GPROJ_DEBUG
#define SHADER_COMPILATION_INFO_BUFFER_SIZE (512)
static GLchar shader_compilation_info_buffer[SHADER_COMPILATION_INFO_BUFFER_SIZE];
#endif

static const GLchar* vs_source =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

static const GLchar* fs_source =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";



static void compile_shader(GLuint* const id, GLenum type, const GLchar* source)

{
	*id = glCreateShader(type);

	assert(*id != 0);

	const GLchar* src[] = { source };
	const GLint length[] = { strlen(source) };

	glShaderSource(*id, 1, src, length);
	glCompileShader(*id);

#ifdef GPROJ_DEBUG
	GLint status;
	glGetShaderiv(*id, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderInfoLog(*id, SHADER_COMPILATION_INFO_BUFFER_SIZE,
		                   NULL, shader_compilation_info_buffer);
		LOG_DEBUG("FAILED TO COMPILE SHADER: %s", shader_compilation_info_buffer);
		assert(false && "FAILED SHADER COMPILATION");
	}
#endif

}

static void create_shader_program(const char* vs_src,
                                  const char* fs_src)
{
	compile_shader(&vs_id, GL_VERTEX_SHADER, vs_src);
	compile_shader(&fs_id, GL_FRAGMENT_SHADER, fs_src);
	
	shader_program_id = glCreateProgram();
	assert(shader_program_id != 0);

	glAttachShader(shader_program_id, vs_id);
	glAttachShader(shader_program_id, fs_id);
	glLinkProgram(shader_program_id);	

#ifdef GPROJ_DEBUG
	GLint status;
	glGetProgramiv(shader_program_id, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		glGetProgramInfoLog(shader_program_id,
		                    SHADER_COMPILATION_INFO_BUFFER_SIZE,
		                    NULL,
		                    shader_compilation_info_buffer);
		LOG_DEBUG("FAILED TO LINK PROGRAM: %s", shader_compilation_info_buffer);
		assert(false && "FAILED PROGRAM LINK");
	}
#endif

}



void ogl_render_init(void)
{
	LOG_DEBUG("INITIALIZING OPENGL RENDER");
	#ifndef GL_VERSION_2_0
	for (int i = 0; i < STATIC_ARRAY_SIZE(gl_proc_names); ++i) {
		gl_void_proc_fn_t proc = OGL_GET_PROC_ADDR(gl_proc_names[i]);
		assert(proc != NULL);
		*gl_proc_ptrs[i] = proc;
	}
	#endif

	//glViewport(0, 0, GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	//create_shader_program(vs_source, fs_source);	
	
	glShadeModel(GL_SMOOTH);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glClearColor(0, 0, 1, 0);
	glViewport(0, 0, GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT, 0, 0, 1);


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


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);

	glColor3f(1, 0, 0);
	glVertex3f(10, 10, 0.0);

	glColor3f(1, 0, 0);
	glVertex3f(25, 10, 0.0);

	glColor3f(1, 0, 0);
	glVertex3f(0.0, 25, 0.0);
	glEnd();




	#ifdef PLATFORM_SDL2
	extern SDL_GLContext* sdl_gl_context;
	extern SDL_Window* sdl_window;
	SDL_GL_SwapWindow(sdl_window);
	#else
	#error "NEED IMPLEMENTATION"
	#endif

}


