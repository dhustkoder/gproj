#include <stdarg.h>
#include <stdio.h>
#include "stb_image.h"
#include "logger.h"
#include "ogl_render.h"


struct ts_vertex {
	struct vec2f pos;
	struct vec2f tex;
};


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

glGenBuffers_fn_t glGenBuffers;
glDeleteBuffers_fn_t glDeleteBuffers;
glBindBuffer_fn_t glBindBuffer;
glBufferData_fn_t glBufferData;
glGetAttribLocation_fn_t glGetAttribLocation;
glVertexAttribPointer_fn_t glVertexAttribPointer;
glEnableVertexAttribArray_fn_t glEnableVertexAttribArray;

/*
glGenTextures_fn_t glGenTextures;
glDeleteTextures_fn_t glDeleteTextures;
glBindTexture_fn_t glBindTexture;
glTexParameterf_fn_t glTexParameterf;
glTexParameterfv_fn_t glTexParameterfv;
glTexParameteri_fn_t glTexParameteri;
glTexParameteriv_fn_t glTexParameteriv;
*/
glActiveTexture_fn_t glActiveTexture;
glUniform1i_fn_t glUniform1i;
glGetUniformLocation_fn_t glGetUniformLocation;


#ifdef GPROJ_DEBUG
glGetShaderiv_fn_t glGetShaderiv;
glGetShaderInfoLog_fn_t glGetShaderInfoLog;
glGetProgramiv_fn_t glGetProgramiv;
glGetProgramInfoLog_fn_t glGetProgramInfoLog;
#endif


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
	
	"glGenBuffers",
	"glDeleteBuffers",
	"glBindBuffer",
	"glBufferData",
	"glGetAttribLocation",
	"glVertexAttribPointer",
	"glEnableVertexAttribArray"
/*
	,"glGenTextures",
	"glDeleteTextures",
	"glBindTexture",
	"glTexParameterf",
	"glTexParameterfv",
	"glTexParameteri",
	"glTexParameteriv"
*/
	,"glActiveTexture",
	"glUniform1i",
	"glGetUniformLocation"
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
	
	&glGenBuffers,
	&glDeleteBuffers,
	&glBindBuffer,
	&glBufferData,
	&glGetAttribLocation,
	&glVertexAttribPointer,
	&glEnableVertexAttribArray
	
/*
	,&glGenTextures,
	&glDeleteTextures,
	&glBindTexture,
	&glTexParameterf,
	&glTexParameterfv,
	&glTexParameteri,
	&glTexParameteriv
*/
	,&glActiveTexture,
	&glUniform1i,
	&glGetUniformLocation

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

static const GLchar* const vs_source = OGL_SL(
	attribute vec2 pos;
	attribute vec2 tex;
	out vec2 TexCoord;
	void main()
	{
		TexCoord = vec2(tex.x / 512, tex.y / 512);
		gl_Position = gl_ProjectionMatrix *
		              gl_ModelViewMatrix *
		              vec4(pos, 0.0, 1.0);
	}

);

static const GLchar* const fs_source = OGL_SL(
	in vec2 TexCoord;
	uniform sampler2D ts_texture;
	void main()
	{
		gl_FragColor = texture2D(ts_texture, TexCoord);
	}
);


static GLuint vbo_id;
static GLuint ts_tex_id;
static struct ts_vertex ts_verts[MAP_MAX_X_TILES * MAP_MAX_Y_TILES * 4];
static GLsizei ts_nverts = 0;



static void load_gl_procs(void)
{
	#ifndef GL_VERSION_2_0
	for (int i = 0; i < STATIC_ARRAY_SIZE(gl_proc_names); ++i) {
		gl_void_proc_fn_t proc = (gl_void_proc_fn_t) OGL_GET_PROC_ADDR(gl_proc_names[i]);
		assert(proc != NULL);
		*gl_proc_ptrs[i] = proc;
	}
	#endif
}


static void compile_shader(GLuint* const id, GLenum type, const GLchar* source)
{
	*id = glCreateShader(type);

	assert(*id != 0);

	const GLchar* src[] = { source };
	const GLint length[] = { strlen(source) };

	glShaderSource(*id, 1, src, length);
	OGL_ASSERT_NO_ERROR();
	glCompileShader(*id);
	OGL_ASSERT_NO_ERROR();

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
	OGL_ASSERT_NO_ERROR();
	glAttachShader(shader_program_id, fs_id);
	OGL_ASSERT_NO_ERROR();
	glLinkProgram(shader_program_id);
	OGL_ASSERT_NO_ERROR();

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
	OGL_ASSERT_NO_ERROR();
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
	OGL_ASSERT_NO_ERROR();
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	OGL_ASSERT_NO_ERROR();
	glBufferData(
		GL_ARRAY_BUFFER,

		(sizeof(struct ts_vertex) * 
		MAP_MAX_X_TILES * 
		MAP_MAX_Y_TILES * 4),

		NULL,
		GL_DYNAMIC_DRAW
	);
	OGL_ASSERT_NO_ERROR();

	const GLint xy_loc = glGetAttribLocation(shader_program_id, "pos");
	OGL_ASSERT_NO_ERROR();
	const GLint uv_loc = glGetAttribLocation(shader_program_id, "tex");
	OGL_ASSERT_NO_ERROR();

	glVertexAttribPointer(
		xy_loc,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(struct ts_vertex),
		(void*) OFFSETOF(struct ts_vertex, pos)	
	);
	OGL_ASSERT_NO_ERROR();

	glVertexAttribPointer(
		uv_loc,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(struct ts_vertex),
		(void*) OFFSETOF(struct ts_vertex, tex)
	);
	OGL_ASSERT_NO_ERROR();

	glEnableVertexAttribArray(xy_loc);
	OGL_ASSERT_NO_ERROR();
	glEnableVertexAttribArray(uv_loc);
	OGL_ASSERT_NO_ERROR();
}

static void term_buffers(void)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	OGL_ASSERT_NO_ERROR();
	glDeleteBuffers(1, &vbo_id);
	OGL_ASSERT_NO_ERROR();
}


static void init_textures(void)
{
	glGenTextures(1, &ts_tex_id);
	OGL_ASSERT_NO_ERROR();
}

static void term_textures(void)
{
	glDeleteTextures(1, &ts_tex_id);
}


void ogl_render_init(void)
{
	LOG_DEBUG("INITIALIZING OPENGL RENDER");

	load_gl_procs();

	init_shader_program();
	init_buffers();
	init_textures();

	glViewport(0, 0, GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	OGL_ASSERT_NO_ERROR();
	glClearColor(0, 0, 1, 1);
	OGL_ASSERT_NO_ERROR();

	glMatrixMode(GL_PROJECTION);
	OGL_ASSERT_NO_ERROR();
	glLoadIdentity();
	OGL_ASSERT_NO_ERROR();
	glOrtho(0, GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT, 0, -1.0f, 1.0f);
	OGL_ASSERT_NO_ERROR();
	
	glMatrixMode(GL_MODELVIEW);
	OGL_ASSERT_NO_ERROR();
	glLoadIdentity();
	OGL_ASSERT_NO_ERROR();
}

void ogl_render_term(void)
{
	LOG_DEBUG("TERMINATING OPENGL RENDER");

	term_textures();
	term_buffers();
	term_shader_program();
}

void ogl_render_load_ts(const char* const path)
{
	LOG_DEBUG("LOADING TS: %s", path);

	int x, y, nchan;
	u8* data = stbi_load(path, &x, &y, &nchan, 0);
	assert(data != NULL);

	LOG_DEBUG(
		"LOADED TS FROM FILE %s\n"
	        "WIDTH: %d\n"
		"HEIGHT: %d\n"
		"CHANNELS: %d",
		path, x, y, nchan
	);

	
	glActiveTexture(GL_TEXTURE0);
	OGL_ASSERT_NO_ERROR();
	glBindTexture(GL_TEXTURE_2D, ts_tex_id);
	OGL_ASSERT_NO_ERROR();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	OGL_ASSERT_NO_ERROR();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	OGL_ASSERT_NO_ERROR();

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		x,
		y,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		data
	);
	OGL_ASSERT_NO_ERROR();

	glUniform1i(glGetUniformLocation(shader_program_id, "ts_texture"), 0);
	OGL_ASSERT_NO_ERROR();


	stbi_image_free(data);
}

void ogl_render_load_ss(const char* const path)
{
}



void ogl_render_map(const struct map_view* const view)
{
	const struct vec2i size = view->size;
	const struct vec2i pos = view->scrpos;
	const struct vec2i* src = view->data;
	struct ts_vertex* dst = ts_verts;

	for (int h = 0; h < size.y; ++h) {
		for (int w = 0; w < size.x; ++w) {
			const GLfloat u = src->x;
			const GLfloat v = src->y;
			++src;
			if (u < 0)
				continue;

			const GLfloat x = (w * TILE_WIDTH) + pos.x;
			const GLfloat y = (h * TILE_HEIGHT) + pos.y;
			dst->pos.x = x;
			dst->pos.y = y;
			dst->tex.x = u;
			dst->tex.y = v;
			++dst;

			dst->pos.x = x + TILE_WIDTH;
			dst->pos.y = y;
			dst->tex.x = u + TILE_WIDTH;
			dst->tex.y = v;
			++dst;

			dst->pos.x = x + TILE_WIDTH;
			dst->pos.y = y + TILE_HEIGHT;
			dst->tex.x = u + TILE_WIDTH;
			dst->tex.y = v + TILE_HEIGHT;
			++dst;

			dst->pos.x = x;
			dst->pos.y = y + TILE_HEIGHT;
			dst->tex.x = u;
			dst->tex.y = v + TILE_HEIGHT;
			++dst;
		}
	}

	ts_nverts = INDEX_OF(ts_verts, dst);
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
	OGL_ASSERT_NO_ERROR();

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

	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(ts_verts[0]) * ts_nverts,
        	ts_verts,
		GL_DYNAMIC_DRAW
	);

	OGL_ASSERT_NO_ERROR();

	glDrawArrays(GL_QUADS, 0, ts_nverts);
	OGL_ASSERT_NO_ERROR();

	#ifdef GPROJ_PLATFORM_SDL2
	extern SDL_Window* sdl_window;

	SDL_GL_SwapWindow(sdl_window);
	#elif defined(GPROJ_PLATFORM_WIN32)
	// ...
	#else
	#error "Unknown Platform"
	#endif
}


