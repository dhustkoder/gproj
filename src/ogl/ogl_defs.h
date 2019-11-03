#ifndef GPROJ_OGL_DEFS_H_
#define GPROJ_OGL_DEFS_H_
#include "types.h"


#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

typedef void   (OGLAPI *gl_void_proc_fn_t)();

#ifndef GL_VERSION_2_0
#define GL_FRAGMENT_SHADER ((GLenum)0x8B30)
#define GL_VERTEX_SHADER   ((GLenum)0x8B31)
#define GL_COMPILE_STATUS  ((GLenum)0x8B81)
#define GL_LINK_STATUS     ((GLenum)0x8B82)

typedef char GLchar;

typedef GLuint (OGLAPI *glCreateShader_fn_t)(GLenum shaderType);
typedef void   (OGLAPI *glShaderSource_fn_t)(GLuint shader, GLsizei count,
                                             const GLchar **string,
                                             const GLint *length);
typedef void  (OGLAPI *glCompileShader_fn_t)(GLuint shader);
typedef GLuint  (OGLAPI *glCreateProgram_fn_t)(void);
typedef void  (OGLAPI *glAttachShader_fn_t)(GLuint programId, GLuint shaderId);
typedef void  (OGLAPI *glLinkProgram_fn_t)(GLuint programId);
typedef void  (OGLAPI *glUseProgram_fn_t)(GLuint programId);

#ifdef GPROJ_DEBUG
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

#ifdef GPROJ_DEBUG
extern glGetShaderiv_fn_t glGetShaderiv;
extern glGetShaderInfoLog_fn_t glGetShaderInfoLog;
extern glGetProgramiv_fn_t glGetProgramiv;
extern glGetProgramInfoLog_fn_t glGetProgramInfoLog;
#endif



#endif



#endif


