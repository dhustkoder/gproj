#ifndef GPROJ_OGL_DEFS_H_
#define GPROJ_OGL_DEFS_H_
#include "types.h"
#include <GL/gl.h>
#include <GL/glu.h>


typedef void   (OGLAPI *gl_void_proc_fn_t)();
typedef GLuint (OGLAPI *glCreateShader_fn_t)(GLenum shaderType);
typedef void   (OGLAPI *glShaderSource_fn_t)(GLuint shader, GLsizei count,
                                             const GLchar **string,
                                             const GLint *length);

typedef void  (OGLAPI *glCompileShader_fn_t)(GLuint shader);
typedef void  (OGLAPI *glGetShaderiv_fn_t)(GLuint shader, GLenum pname, GLint *params);
typedef void  (OGLAPI *glGetShaderInfoLog_fn_t)(GLuint shader, GLsizei maxLength,
                                         GLsizei* length, GLchar* infoLog);
#ifndef GL_VERTEX_SHADER
extern GLenum GL_VERTEX_SHADER;
#endif
#ifndef GL_COMPILE_STATUS
extern GLenum GL_COMPILE_STATUS;
#endif
extern glCreateShader_fn_t glCreateShader;
extern glShaderSource_fn_t glShaderSource;
extern glCompileShader_fn_t glCompileShader;
extern glGetShaderiv_fn_t glGetShaderiv;
extern glGetShaderInfoLog_fn_t glGetShaderInfoLog;




#endif

