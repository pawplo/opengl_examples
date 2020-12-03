//========================================================================
// Simple GLFW example
// Copyright (c) Camilla Löwy <elmindreda@glfw.org>
// Copyright (c) 2018 Paweł Płóciennik (github.com/pawplo)
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

//#include <GL/gl3w.h>
#include "gl.h"
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

static const char *vertex_shader_str =
"#version 410\n"
"in vec2 v_pos;\n"
"void main() {\n"
"    gl_Position = vec4(v_pos, 0.0, 1.0);\n"
"}\n";

static const char *geometry_shader_str =
"#version 410 core\n"
"layout (points) in;\n"
"layout (triangle_strip, max_vertices = 3) out;\n"
"void main() {\n"
"    gl_Position = gl_in[0].gl_Position + vec4( 0.0, 0.5, 0.0, 0.0);\n"
"    EmitVertex();\n"
"    gl_Position = gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.0, 0.0);\n"
"    EmitVertex();\n"
"    gl_Position = gl_in[0].gl_Position + vec4( 0.5, -0.5, 0.0, 0.0);\n"
"    EmitVertex();\n"
"    EndPrimitive();\n"
"}\n";

static const char *fragment_shader_str =
"#version 410\n"
"out vec4 color;\n"
"void main() {\n"
"    color = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\n";

GLFWwindow *window;

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void refresh_callback()
{
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_POINTS, 0, 1);
    glfwSwapBuffers(window);
}

void error_exit(const char *str)
{
    fprintf(stderr, "%s\n", str);
    exit(1);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        error_exit("Failed to initialize glfw.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(300, 300, "OpenGL 4.1 Geometry Shader", NULL, NULL);
    if (!window)
         error_exit("Failed to create window.");

    glfwMakeContextCurrent(window);

//    if (gl3wInit())
//        error_exit("Failed to initialize OpenGL.");

//    if (!gl3wIsSupported(3, 3))
//        error_exit("OpenGL 4.1 not supported.");

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowRefreshCallback(window, refresh_callback);

    static float point[] = { 0.0f,  0.0f };
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

    GLuint program;
    if(create_program((GLchar *)vertex_shader_str, (GLchar *)fragment_shader_str, (GLchar *)geometry_shader_str, &program))
         error_exit("Failder to create program.");

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLint v_pos_loc;
    v_pos_loc = glGetAttribLocation(program, "v_pos");

    glEnableVertexAttribArray(v_pos_loc);
    glVertexAttribPointer(v_pos_loc, 2, GL_FLOAT, GL_FALSE, 1, (GLvoid*)0);

    glUseProgram(program);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    while (!glfwWindowShouldClose(window))
        glfwPollEvents();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
