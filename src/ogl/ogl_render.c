#include <stdarg.h>
#include <stdio.h>
#include "logger.h"
#include "ogl_render.h"


#define GLSL(...) "#version 120\n" #__VA_ARGS__

#ifndef GL_VERSION_2_0
glCreateShader_fn_t glCreateShader;
glShaderSource_fn_t glShaderSource;
glCompileShader_fn_t glCompileShader;
glCreateProgram_fn_t glCreateProgram;
glAttachShader_fn_t glAttachShader;
glLinkProgram_fn_t glLinkProgram;
glUseProgram_fn_t glUseProgram;
glDetachShader_fn_t glDetachShader;
glDeleteShader_fn_t glDeleteShader;
glDeleteProgram_fn_t glDeleteProgram;
glDeleteBuffers_fn_t glDeleteBuffers;
glGenBuffers_fn_t glGenBuffers;
glBindBuffer_fn_t glBindBuffer;
glBufferData_fn_t glBufferData;
glGetAttribLocation_fn_t glGetAttribLocation;
glVertexAttribPointer_fn_t glVertexAttribPointer;
glEnableVertexAttribArray_fn_t glEnableVertexAttribArray;


glGetShaderiv_fn_t glGetShaderiv;
glGetShaderInfoLog_fn_t glGetShaderInfoLog;
glGetProgramiv_fn_t glGetProgramiv;
glGetProgramInfoLog_fn_t glGetProgramInfoLog;
#endif


#ifndef GL_VERSION_2_0
static GLchar* gl_proc_names[] = {
	"glCreateShader",
	"glShaderSource",
	"glCompileShader",
	"glCreateProgram",
	"glAttachShader",
	"glLinkProgram",
	"glUseProgram",
	"glDetachShader",
	"glDeleteShader",
	"glDeleteProgram",
	"glDeleteBuffers",
	"glGenBuffers",
	"glBindBuffer",
	"glBufferData",
	"glGetAttribLocation",
	"glVertexAttribPointer",
	"glEnableVertexAttribArray"
	#ifdef GPROJ_DEBUG
	,"glGetShaderiv",
	"glGetShaderInfoLog",
	"glGetProgramiv",
	"glGetProgramInfoLog"
	#endif

};

static gl_void_proc_fn_t* gl_proc_ptrs[] = {
	&glCreateShader,
	&glShaderSource,
	&glCompileShader,
	&glCreateProgram,
	&glAttachShader,
	&glLinkProgram,
	&glUseProgram,
	&glDetachShader,
	&glDeleteShader,
	&glDeleteProgram,
	&glDeleteBuffers,
	&glGenBuffers,
	&glBindBuffer,
	&glBufferData,
	&glGetAttribLocation,
	&glVertexAttribPointer,
	&glEnableVertexAttribArray

	#ifdef GPROJ_DEBUG
	,&glGetShaderiv,
	&glGetShaderInfoLog,
	&glGetProgramiv,
	&glGetProgramInfoLog,
	#endif

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

static const GLchar* const vs_source = GLSL(
	attribute vec2 dep_position;
	attribute vec2 position;

	void main()
	{
		gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix *
		              vec4(dep_position, 0.0, 1.0);
	}
);

static const GLchar* const fs_source = GLSL(
	void main()
	{
		gl_FragColor = vec4(0.0f, 1.0f, 0.0f, 0.5f);
	}
);


static GLuint vbo_id;



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

static void init_shader_program(void)
{
	compile_shader(&vs_id, GL_VERTEX_SHADER, vs_source);
	compile_shader(&fs_id, GL_FRAGMENT_SHADER, fs_source);

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

	glUseProgram(shader_program_id);

}

static void term_shader_program(void)
{
	glDetachShader(shader_program_id, vs_id);
	glDetachShader(shader_program_id, fs_id);
	glDeleteShader(vs_id);
	glDeleteShader(fs_id);
	glDeleteProgram(shader_program_id);
}

static void init_buffers(void)
{
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(struct vec2f) * (MAP_MAX_X_TILES * 2 * MAP_MAX_Y_TILES),
				 NULL,
				 GL_DYNAMIC_DRAW);

	GLint pos_loc = glGetAttribLocation(shader_program_id, "dep_position");
	glVertexAttribPointer(pos_loc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), NULL);
	glEnableVertexAttribArray(pos_loc);
}

static void term_buffers(void)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo_id);
}


void ogl_render_init(void)
{
	LOG_DEBUG("INITIALIZING OPENGL RENDER");
	#ifndef GL_VERSION_2_0
	for (int i = 0; i < STATIC_ARRAY_SIZE(gl_proc_names); ++i) {
		gl_void_proc_fn_t proc = (gl_void_proc_fn_t) OGL_GET_PROC_ADDR(gl_proc_names[i]);
		assert(proc != NULL);
		*gl_proc_ptrs[i] = proc;
	}
	#endif
	init_shader_program();
	init_buffers();

	glViewport(0, 0, GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	glClearColor(0, 0, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT, 0, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void ogl_render_term(void)
{
	LOG_DEBUG("TERMINATING OPENGL RENDER");

	term_shader_program();
	term_buffers();
}

void ogl_render_load_ts(const char* const path)
{
}

void ogl_render_load_ss(const char* const path)
{

}

static struct vec2f map_verts[MAP_MAX_X_TILES * MAP_MAX_Y_TILES * 4];
static GLsizei map_verts_size = 0;


void ogl_render_map(const struct map_view* const view)
{
	const struct vec2i size = view->size;
	const struct vec2i pos = view->scrpos;
	const struct vec2i* src = view->data;
	struct vec2f* dst = map_verts;

	map_verts_size = 0;

	for (int h = 0; h < size.y; ++h) {
		for (int w = 0; w < size.x; ++w) {
			const GLfloat tsx = src->x;
			++src;
			if (tsx < 0)
				continue;

			const GLfloat x = (w * TILE_WIDTH) + pos.x;
			const GLfloat y = (h * TILE_HEIGHT) + pos.y;
			dst->x = x;
			dst->y = y;
			++dst;

			dst->x = x + TILE_WIDTH;
			dst->y = y;
			++dst;

			dst->x = x + TILE_WIDTH;
			dst->y = y + TILE_HEIGHT;
			++dst;

			dst->x = x;
			dst->y = y + TILE_HEIGHT;
			++dst;

			map_verts_size += 4;
		}
	}

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
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	/*
	glBegin(GL_TRIANGLES);

	glColor4f(1, 0, 0, 1);
	glVertex3f(-1, -1, 0);
	glVertex3f(1, -1, 0);
	glVertex3f(1,  1, 0);

	glEnd();
	*/
/*
	struct vec2f test_verts[] = {
		{  0, 0   },
		{  32, 0  },
		{  32, 32 },
		{  0, 32  }
	};

	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(test_verts[0]) * 4,
                 test_verts,
	             GL_DYNAMIC_DRAW);

	glDrawArrays(GL_QUADS, 0, 4);
*/

	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(map_verts[0]) * map_verts_size,
                 map_verts,
	             GL_DYNAMIC_DRAW);

	glDrawArrays(GL_QUADS, 0, map_verts_size);

	#ifdef GPROJ_PLATFORM_SDL2
	extern SDL_Window* sdl_window;

	SDL_GL_SwapWindow(sdl_window);
	#else
	#error "NEED IMPLEMENTATION"
	#endif
}


#ifdef GPROJ_PLATFORM_SDL2
void ogl_window_resize(int w, int h)
{
	glViewport(0, 0, w, h);
}
#endif
