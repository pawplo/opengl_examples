#include <stdio.h>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#define INFO_LOG_BUF_SIZE 512
GLchar info_log_buf[INFO_LOG_BUF_SIZE+1];
GLsizei size;

int create_shader(GLenum type, const GLchar *str, GLuint *shader_)
{
		GLint status;
		*shader_ = glCreateShader(type);

		if (*shader_) {
				glShaderSource(*shader_, 1, &str, NULL);
				glCompileShader(*shader_);
				glGetShaderiv(*shader_, GL_COMPILE_STATUS, &status);
				if (!status) {
						glGetShaderInfoLog(*shader_, INFO_LOG_BUF_SIZE, &size, info_log_buf);
						if (size >= INFO_LOG_BUF_SIZE)
							size--;
						info_log_buf[size] = '\0';
						printf("GL_COMPILE_STATUS: %s\n", info_log_buf);
						glDeleteShader(*shader_);
						return -1;
				}
		}
		return 0;
}

int create_program(const GLchar *vs_str, const GLchar *fs_str, const GLchar *gs_str, GLuint *program_)
{
		GLint status;
		GLuint vs;
		GLuint fs;
		GLuint gs;

		*program_ = glCreateProgram();

//		printf("VERTEX_SHADER\n");
		if (create_shader(GL_VERTEX_SHADER, vs_str, &vs))
		    return -3;
		
//		printf("FRAGMENT_SHADER\n");
		if (create_shader(GL_FRAGMENT_SHADER, fs_str, &fs))
		    return -4;

                if (gs_str) {
//			printf("GEOMETRY_SHADER\n");
			if (create_shader(GL_GEOMETRY_SHADER, gs_str, &gs))
			    return -5;
		}
		glAttachShader(*program_, vs);
		glAttachShader(*program_, fs);
		if (gs_str)
			glAttachShader(*program_, gs);
		glLinkProgram(*program_);
 
		glGetProgramiv(*program_, GL_LINK_STATUS, &status);
		if (!status) {
				glGetProgramInfoLog(*program_, INFO_LOG_BUF_SIZE, &size, info_log_buf);
				if (size >= INFO_LOG_BUF_SIZE)
					size--;
				info_log_buf[size] = '\0';
				printf("GL_LINK_STATUS: %s\n", info_log_buf);
				return -5;
    }
		return 0;
}
