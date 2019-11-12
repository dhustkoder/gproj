#ifndef GPROJ_OGL_DEFS_H_
#define GPROJ_OGL_DEFS_H_
#include "platform_defs.h"

#ifdef _MSC_VER
#include <windows.h>
#define OGLAPI APIENTRY
#else
#define OGLAPI
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


typedef void (OGLAPI *gl_void_proc_fn_t)();



#ifdef GPROJ_PLATFORM_SDL2
#define ogl_get_proc_addr(proc_name) (gl_void_proc_fn_t)SDL_GL_GetProcAddress(proc_name)
#elif defined(GPROJ_PLATFORM_WIN32)
#define ogl_get_proc_addr(proc_name) (gl_void_proc_fn_t)wglGetProcAddress(proc_name)
#else
#error "Unknown Platform"
#endif


#define OGL_SL(...) "#version 120\n" #__VA_ARGS__


#ifdef GPROJ_DEBUG
#define OGL_ASSERT_NO_ERROR() { \
	const GLenum gl_error_code = glGetError(); \
	if (gl_error_code != GL_NO_ERROR) { \
		LOG_DEBUG("GL ERROR: %d", gl_error_code); \
		assert(false && "OGL ERROR"); \
	} \
}
#else
#define OGL_ASSERT_NO_ERROR() ((void)0)
#endif


#ifndef GL_VERSION_2_0
#define GL_FRAGMENT_SHADER ((GLenum)0x8B30)
#define GL_VERTEX_SHADER   ((GLenum)0x8B31)
#define GL_COMPILE_STATUS  ((GLenum)0x8B81)
#define GL_LINK_STATUS     ((GLenum)0x8B82)
#define GL_ARRAY_BUFFER    ((GLenum)0x8892)
#define GL_DYNAMIC_DRAW    ((GLenum)0x88E8)
#define GL_TEXTURE0        ((GLenum)0x84C0)
#define GL_TEXTURE1        ((GLenum)0x84C1)
#define GL_TEXTURE2        ((GLenum)0x84C2)
#define GL_TEXTURE3        ((GLenum)0x84C3)
#define GL_TEXTURE4        ((GLenum)0x84C4)
#define GL_TEXTURE5        ((GLenum)0x84C5)
#define GL_TEXTURE6        ((GLenum)0x84C6)
#define GL_TEXTURE7        ((GLenum)0x84C7)
#define GL_TEXTURE8        ((GLenum)0x84C8)
#define GL_TEXTURE9        ((GLenum)0x84C9)
#define GL_TEXTURE10       ((GLenum)0x84CA)
#define GL_TEXTURE11       ((GLenum)0x84CB)
#define GL_TEXTURE12       ((GLenum)0x84CC)
#define GL_TEXTURE13       ((GLenum)0x84CD)
#define GL_TEXTURE14       ((GLenum)0x84CE)
#define GL_TEXTURE15       ((GLenum)0x84CF)
#define GL_TEXTURE16       ((GLenum)0x84D0)
#define GL_TEXTURE17       ((GLenum)0x84D1)
#define GL_TEXTURE18       ((GLenum)0x84D2)
#define GL_TEXTURE19       ((GLenum)0x84D3)
#define GL_TEXTURE20       ((GLenum)0x84D4)
#define GL_TEXTURE21       ((GLenum)0x84D5)
#define GL_TEXTURE22       ((GLenum)0x84D6)
#define GL_TEXTURE23       ((GLenum)0x84D7)
#define GL_TEXTURE24       ((GLenum)0x84D8)
#define GL_TEXTURE25       ((GLenum)0x84D9)
#define GL_TEXTURE26       ((GLenum)0x84DA)
#define GL_TEXTURE27       ((GLenum)0x84DB)
#define GL_TEXTURE28       ((GLenum)0x84DC)
#define GL_TEXTURE29       ((GLenum)0x84DD)
#define GL_TEXTURE30       ((GLenum)0x84DE)
#define GL_TEXTURE31       ((GLenum)0x84DF)

typedef char GLchar;
typedef sptr GLsizeiptr;

/* shaders */
typedef GLuint (OGLAPI *glCreateShader_fn_t)(GLenum shaderType);
typedef void   (OGLAPI *glShaderSource_fn_t)(GLuint shader, GLsizei count,
                                             const GLchar **string,
                                             const GLint *length);
typedef void  (OGLAPI *glCompileShader_fn_t)(GLuint shader);
typedef GLuint  (OGLAPI *glCreateProgram_fn_t)(void);
typedef void  (OGLAPI *glAttachShader_fn_t)(GLuint programId, GLuint shaderId);
typedef void  (OGLAPI *glLinkProgram_fn_t)(GLuint programId);
typedef void  (OGLAPI *glUseProgram_fn_t)(GLuint programId);
typedef void  (OGLAPI *glDetachShader_fn_t)(GLuint programId, GLuint shaderId);
typedef void  (OGLAPI *glDeleteShader_fn_t)(GLuint shaderId);
typedef void  (OGLAPI *glDeleteProgram_fn_t)(GLuint programId);

/* buffers */
typedef void  (OGLAPI *glGenBuffers_fn_t)(GLsizei n, GLuint* buffers);
typedef void  (OGLAPI *glDeleteBuffers_fn_t)(GLsizei n, const GLuint* buffers);
typedef void  (OGLAPI *glBindBuffer_fn_t)(GLenum target, GLuint bufferId);
typedef void  (OGLAPI *glBufferData_fn_t)(GLenum target,
                                          GLsizeiptr size,
                                          const void* data,
                                          GLenum usage);
typedef GLint (OGLAPI *glGetAttribLocation_fn_t)(GLuint programId, const GLchar* attribName);
typedef void (OGLAPI *glVertexAttribPointer_fn_t)(GLuint index,
                                                  GLint size,
                                                  GLenum type,
                                                  GLboolean normalized,
                                                  GLsizei stride,
                                                  const void * pointer);
typedef void (OGLAPI *glEnableVertexAttribArray_fn_t)(GLuint index);


/* 
 * textures
typedef void (OGLAPI *glGenTextures_fn_t)(GLsizei n, GLuint *textures);
typedef void (OGLAPI *glDeleteTextures_fn_t)(GLsizei n, const GLuint* textures);
typedef void (OGLAPI *glBindTexture_fn_t)(GLenum target, GLuint textureId);
typedef void (OGLAPI *glTexParameterf_fn_t)(GLenum target, GLenum pname, GLfloat param);
typedef void (OGLAPI *glTexParameterfv_fn_t)(GLenum target, GLenum pname, const GLfloat *params);
typedef void (OGLAPI *glTexParameteri_fn_t)(GLenum target, GLenum pname, GLint param);
typedef void (OGLAPI *glTexParameteriv_fn_t)(GLenum target, GLenum pname, const GLint *params);
*/
typedef void (OGLAPI *glActiveTexture_fn_t)(GLenum texture);
typedef void (OGLAPI *glUniform1i_fn_t)(GLint location, GLint v0);
typedef GLint (OGLAPI *glGetUniformLocation_fn_t)(GLuint program, const GLchar* name);
typedef void (OGLAPI *glUniform2f_fn_t)(GLint location, GLfloat v0, GLfloat v1);
#ifdef GPROJ_DEBUG
/* shaders error check */
typedef void  (OGLAPI *glGetShaderiv_fn_t)(GLuint shader, GLenum pname, GLint *params);
typedef void  (OGLAPI *glGetShaderInfoLog_fn_t)(GLuint shader, GLsizei maxLength,
                                                GLsizei* length, GLchar* infoLog);
typedef void  (OGLAPI *glGetProgramiv_fn_t)(GLuint programId, GLenum pname, GLint *params);
typedef void  (OGLAPI *glGetProgramInfoLog_fn_t)(GLuint programId,
                                                 GLsizei maxLength,
                                                 GLsizei *length,
                                                 GLchar  *infoLog);
#endif



extern glCreateShader_fn_t glCreateShader;
extern glShaderSource_fn_t glShaderSource;
extern glCompileShader_fn_t glCompileShader;
extern glCreateProgram_fn_t glCreateProgram;
extern glAttachShader_fn_t glAttachShader;
extern glLinkProgram_fn_t glLinkProgram;
extern glUseProgram_fn_t glUseProgram;
extern glDetachShader_fn_t glDetachShader;
extern glDeleteShader_fn_t glDeleteShader;
extern glDeleteProgram_fn_t glDeleteProgram;

extern glGenBuffers_fn_t glGenBuffers;
extern glDeleteBuffers_fn_t glDeleteBuffers;
extern glBindBuffer_fn_t glBindBuffer;
extern glBufferData_fn_t glBufferData;
extern glGetAttribLocation_fn_t glGetAttribLocation;
extern glVertexAttribPointer_fn_t glVertexAttribPointer;
extern glEnableVertexAttribArray_fn_t glEnableVertexAttribArray;

/*
extern glGenTextures_fn_t glGenTextures;
extern glDeleteTextures_fn_t glDeleteTextures;
extern glBindTexture_fn_t glBindTexture;
extern glTexParameterf_fn_t glTexParameterf;
extern glTexParameterfv_fn_t glTexParameterfv;
extern glTexParameteri_fn_t glTexParameteri;
extern glTexParameteriv_fn_t glTexParameteriv;
*/
extern glActiveTexture_fn_t glActiveTexture;
extern glUniform1i_fn_t glUniform1i;
extern glGetUniformLocation_fn_t glGetUniformLocation;
extern glUniform2f_fn_t glUniform2f;

#ifdef GPROJ_DEBUG
extern glGetShaderiv_fn_t glGetShaderiv;
extern glGetShaderInfoLog_fn_t glGetShaderInfoLog;
extern glGetProgramiv_fn_t glGetProgramiv;
extern glGetProgramInfoLog_fn_t glGetProgramInfoLog;
#endif



#endif



#endif


