#include <stdarg.h>
#include <stdio.h>
#include "stb_image.h"
#include "logger.h"
#include "ogl_render.h"


struct ts_vertex {
	struct vec2f world_pos;
	struct vec2f tex_pos;
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
glUniform2f_fn_t glUniform2f;
glUniform2i_fn_t glUniform2i;


#ifdef GPROJ_DEBUG
glGetShaderiv_fn_t glGetShaderiv;
glGetShaderInfoLog_fn_t glGetShaderInfoLog;
glGetProgramiv_fn_t glGetProgramiv;
glGetProgramInfoLog_fn_t glGetProgramInfoLog;
#endif



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
	"glGetUniformLocation",
	"glUniform2f",
	"glUniform2i"

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
	&glGetUniformLocation,
	&glUniform2f,
	&glUniform2i

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
#endif /* GL_VERSION_2_0 */

static GLuint vs_id;
static GLuint fs_id;
static GLuint shader_program_id;

#ifdef GPROJ_DEBUG
#define SHADER_COMPILATION_INFO_BUFFER_SIZE (512)
static GLchar shader_compilation_info_buffer[SHADER_COMPILATION_INFO_BUFFER_SIZE];
#endif

static const GLchar* const vs_source = OGL_SL(
	attribute vec2 world_pos;
	attribute vec2 tex_pos;

	varying vec2 fs_tex_pos;

	void main()
	{
		fs_tex_pos = tex_pos;
		gl_Position = gl_ProjectionMatrix *
		              gl_ModelViewMatrix *
		              vec4(world_pos, 1.0, 1.0);
	}

);

static const GLchar* const fs_source = OGL_SL(
	varying vec2 fs_tex_pos;


	uniform ivec2 ts_tex_size;
	uniform sampler2D textures;


	void main()
	{
		vec2 tspos = vec2(fs_tex_pos / ts_tex_size);
		gl_FragColor = texture2D(textures, tspos);
	}

);


static GLuint vbo_id;
static GLuint ts_tex_id;
static struct ts_vertex ts_verts[MAP_MAX_X_TILES * MAP_MAX_Y_TILES * 4];
static GLsizei ts_nverts = 0;

static GLfloat width_scale = 1.0f;
static GLfloat height_scale = 1.0f;


static void load_gl_procs(void)
{
	#ifndef GL_VERSION_2_0
	for (int i = 0; i < STATIC_ARRAY_SIZE(gl_proc_names); ++i) {
		gl_void_proc_fn_t proc = (gl_void_proc_fn_t) ogl_get_proc_addr(gl_proc_names[i]);
		assert(proc != NULL);
		*gl_proc_ptrs[i] = proc;
	}
	#endif
}

static GLuint create_and_compile_shader(
	const GLenum type,
	const GLchar* const source
)
{
	const GLuint id = glCreateShader(type);

	assert(id != 0);

	const GLchar* src[] = { source };
	const GLint length[] = { strlen(source) };

	glShaderSource(id, 1, src, length);
	glCompileShader(id);


#ifdef GPROJ_DEBUG
	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glGetShaderInfoLog(
			id,
			SHADER_COMPILATION_INFO_BUFFER_SIZE,
			NULL,
			shader_compilation_info_buffer
		);
		log_dbg(
			"FAILED TO COMPILE SHADER: %s\n"
			"ERROR: %s",
			source, shader_compilation_info_buffer
		);
		assert(false && "FAILED SHADER COMPILATION");
	}
#endif

	OGL_ASSERT_NO_ERROR;

	return id;
}

static void init_shader_program(void)
{
	vs_id = create_and_compile_shader(GL_VERTEX_SHADER, vs_source);
	fs_id = create_and_compile_shader(GL_FRAGMENT_SHADER, fs_source);

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
		log_dbg("FAILED TO LINK PROGRAM: %s", shader_compilation_info_buffer);
		assert(false && "FAILED PROGRAM LINK");
	}
#endif

	glUseProgram(shader_program_id);

	OGL_ASSERT_NO_ERROR;
}

static void term_shader_program(void)
{
	glUseProgram(0);
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
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(ts_verts),
		NULL,
		GL_DYNAMIC_DRAW
	);

	const GLint xy_loc = glGetAttribLocation(shader_program_id, "world_pos");
	const GLint uv_loc = glGetAttribLocation(shader_program_id, "tex_pos");

	glVertexAttribPointer(
		xy_loc,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(struct ts_vertex),
		(void*) OFFSETOF(struct ts_vertex, world_pos)
	);

	glVertexAttribPointer(
		uv_loc,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(struct ts_vertex),
		(void*) OFFSETOF(struct ts_vertex, tex_pos)
	);

	glEnableVertexAttribArray(xy_loc);
	glEnableVertexAttribArray(uv_loc);

	OGL_ASSERT_NO_ERROR;
}

static void term_buffers(void)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo_id);
}


static void init_textures(void)
{
	glGenTextures(1, &ts_tex_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ts_tex_id);

	OGL_ASSERT_NO_ERROR;
}

static void term_textures(void)
{
	glDeleteTextures(1, &ts_tex_id);
}


void ogl_render_init(void)
{
	log_dbg("INITIALIZING OPENGL RENDER");

	load_gl_procs();

	init_shader_program();
	init_buffers();
	init_textures();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
	ogl_render_resize(GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);

	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	log_dbg(
		"OpenGL INFO: \n"
		"VENDOR: %s\n"
		"RENDERER: %s\n"
		"VERSION: %s\n",
		vendor, renderer, version
	);


	OGL_ASSERT_NO_ERROR;
}

void ogl_render_term(void)
{
	log_dbg("TERMINATING OPENGL RENDER");

	term_textures();
	term_buffers();
	term_shader_program();
}

void ogl_render_load_ts(const char* const path)
{
	log_dbg("LOADING TS: %s", path);

	int x, y, nchan;
	void* const data = stbi_load(path, &x, &y, &nchan, 0);
	assert(data != NULL);
	assert(nchan == 4); // assert RGBA

	log_dbg(
		"LOADED TS FROM FILE %s\n"
		"WIDTH: %d\n"
		"HEIGHT: %d\n"
		"CHANNELS: %d",
		path, x, y, nchan
	);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ts_tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data
	);

	const GLint ts_tex_size_id = glGetUniformLocation(shader_program_id, "ts_tex_size");
	glUniform2i(ts_tex_size_id, x, y);

	OGL_ASSERT_NO_ERROR;

	stbi_image_free(data);
}

void ogl_render_load_ss(const char* const path)
{
}



void ogl_render_tilemap(
	const struct vec2f* const cam,
	const struct tilemap* const tm
)
{
	const struct vec2i size = tm->size;
	const struct vec2i* tile = tm->tiles;
	struct ts_vertex* dest = ts_verts;
	const GLfloat tilew = TILE_WIDTH * width_scale * tm->scale;
	const GLfloat tileh = TILE_HEIGHT * height_scale * tm->scale;
	for (int h = 0; h < size.y; ++h) {
		for (int w = 0; w < size.x; ++w) {
			const GLfloat u = tile->x;
			const GLfloat v = tile->y;
			++tile;
			if (u < 0)
				continue;

			const GLfloat x = (w * tilew) - cam->x;
			const GLfloat y = (h * tileh) - cam->y;

			dest[0].world_pos.x = x;
			dest[0].world_pos.y = y;
			dest[0].tex_pos.x   = u;
			dest[0].tex_pos.y   = v;

			dest[1].world_pos.x = x + tilew;
			dest[1].world_pos.y = y;
			dest[1].tex_pos.x   = u + TILE_WIDTH;
			dest[1].tex_pos.y   = v;

			dest[2].world_pos.x = x + tilew;
			dest[2].world_pos.y = y + tileh;
			dest[2].tex_pos.x   = u + TILE_WIDTH;
			dest[2].tex_pos.y   = v + TILE_HEIGHT;

			dest[3].world_pos.x = x;
			dest[3].world_pos.y = y + tileh;
			dest[3].tex_pos.x   = u;
			dest[3].tex_pos.y   = v + TILE_HEIGHT;
			
			dest += 4;
		}
	}

	ts_nverts = INDEX_OF(ts_verts, dest);
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


void ogl_render_text(const char* const fmt, ...)
{
	/*
	static char buffer[1024];
	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);
	log("%s", buffer);
	*/
}

void ogl_render_finish_frame(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(ts_verts[0]) * ts_nverts,
        	ts_verts,
		GL_DYNAMIC_DRAW
	);
	
	glDrawArrays(GL_QUADS, 0, ts_nverts);

	glFlush();

	OGL_ASSERT_NO_ERROR;
}

void ogl_render_resize(const int width, const int height)
{
	log_dbg("OpenGL resizing %dx%d", width, height);

	if (width == 0 || height == 0)
		return;

	if (width >= GPROJ_SCR_WIDTH) {
		width_scale = ((double)width) / ((double)GPROJ_SCR_WIDTH);
	} else {
		//INVALID_CODE_PATH;
	}
	
	if (height >= GPROJ_SCR_HEIGHT) {
		height_scale = ((double)height) / ((double)GPROJ_SCR_HEIGHT);
	} else {
		//INVALID_CODE_PATH;
	}

	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	OGL_ASSERT_NO_ERROR;
}

