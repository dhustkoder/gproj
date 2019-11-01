#ifndef GPROJ_OGL_DEFS_H_
#define GPROJ_OGL_DEFS_H_
#include <GL/gl.h>
#include "types.h"


typedef void (GLAPI *gl_void_proc_fn_t)();
typedef GLuint (GLAPI *glCreateShader_fn_t)(GLenum shaderType);
typedef void   (GLAPI *glShaderSource_fn_t)(GLuint shader, GLsizei count,
                                      const GLchar **string,
									  const GLint *length);

typedef void  (GLAPI *glCompileShader_fn_t)(GLuint shader);
typedef void  (GLAPI *glShaderiv_fn_t)(GLuint shader, GLenum pname, GLint *params);
typedef void  (GLAPI *glGetShaderInfoLog_fn_t)(GLuint shader, GLsizei maxLength,
                                         GLsizei* length, GLchar* infoLog);


extern glCreateShader_fn_t glCreateShader;
extern glShaderSource_fn_t glShaderSource;
extern glCompileShader_fn_t glCompileShader;
extern glShaderiv_fn_t glShaderiv;
extern glGetShaderInfoLog_fn_t glGetShaderInfoLog;




#endif

