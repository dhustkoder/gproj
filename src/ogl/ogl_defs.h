#ifndef GPROJ_OGL_DEFS_H_
#define GPROJ_OGL_DEFS_H_
#include "platform_defs.h"


#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


#ifdef _MSC_VER
#define OGLAPI APIENTRY
#else
#define OGLAPI 
#endif


typedef void (OGLAPI *gl_void_proc_fn_t)();

#ifndef GL_VERSION_2_0
#define GL_FRAGMENT_SHADER ((GLenum)0x8B30)
#define GL_VERTEX_SHADER   ((GLenum)0x8B31)
#define GL_COMPILE_STATUS  ((GLenum)0x8B81)
#define GL_LINK_STATUS     ((GLenum)0x8B82)
#define GL_ARRAY_BUFFER    ((GLenum)0x8892)
#define GL_DYNAMIC_DRAW    ((GLenum)0x88E8)

typedef char GLchar;
typedef sptr GLsizeiptr;

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
typedef void  (OGLAPI *glDeleteBuffers_fn_t)(GLsizei n, const GLuint* buffers);
typedef void  (OGLAPI *glGenBuffers_fn_t)(GLsizei n, GLuint* buffers);
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
extern glDetachShader_fn_t glDetachShader;
extern glDeleteShader_fn_t glDeleteShader;
extern glDeleteProgram_fn_t glDeleteProgram;
extern glDeleteBuffers_fn_t glDeleteBuffers;
extern glGenBuffers_fn_t glGenBuffers;
extern glBindBuffer_fn_t glBindBuffer;
extern glBufferData_fn_t glBufferData;
extern glGetAttribLocation_fn_t glGetAttribLocation;
extern glVertexAttribPointer_fn_t glVertexAttribPointer;
extern glEnableVertexAttribArray_fn_t glEnableVertexAttribArray;


#ifdef GPROJ_DEBUG
extern glGetShaderiv_fn_t glGetShaderiv;
extern glGetShaderInfoLog_fn_t glGetShaderInfoLog;
extern glGetProgramiv_fn_t glGetProgramiv;
extern glGetProgramInfoLog_fn_t glGetProgramInfoLog;
#endif



#endif



#endif


