#ifndef __SHADER_H
#define __SHADER_H

#include <GLFW/glfw3.h>

extern int create_shader(GLenum type, const GLchar *str, GLuint *_shader);
extern int create_program(const GLchar *vs_str, const GLchar *fs_str, const GLchar *gs_str, GLuint *_program);

#endif //__SHADER_H
