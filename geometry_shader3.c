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

#include <GL/gl3w.h>
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
"layout (triangle_strip, max_vertices = 6) out;\n"
"uniform float u_x;\n"
"out vec2 pos_center;\n"
"out vec2 pos;\n"
"void main() {\n"
"    pos_center = gl_in[0].gl_Position.xy;\n"

"    gl_Position = gl_in[0].gl_Position + vec4(-u_x,  u_x, 0.0, 0.0);\n"
"    pos = gl_Position.xy;\n"
"    EmitVertex();\n"

"    gl_Position = gl_in[0].gl_Position + vec4(-u_x, -u_x, 0.0, 0.0);\n"
"    pos = gl_Position.xy;\n"
"    EmitVertex();\n"

"    gl_Position = gl_in[0].gl_Position + vec4( u_x, -u_x, 0.0, 0.0);\n"
"    pos = gl_Position.xy;\n"
"    EmitVertex();\n"

"    EndPrimitive();\n"

"    gl_Position = gl_in[0].gl_Position + vec4( -u_x, u_x, 0.0, 0.0);\n"
"    pos = gl_Position.xy;\n"
"    EmitVertex();\n"

"    gl_Position = gl_in[0].gl_Position + vec4( u_x, -u_x, 0.0, 0.0);\n"
"    pos = gl_Position.xy;\n"
"    EmitVertex();\n"

"    gl_Position = gl_in[0].gl_Position + vec4( u_x,  u_x, 0.0, 0.0);\n"
"    pos = gl_Position.xy;\n"
"    EmitVertex();\n"

"    EndPrimitive();\n"
"}\n";

static const char *fragment_shader_str =
"#version 410\n"
"in vec2 pos_center;\n"
"in vec2 pos;\n"
"out vec4 color;\n"
"uniform float u_x;\n"
"void main() {\n"
"    //color = vec4(1.0, 0.0, 0.0, 1.0);\n"
"    if (distance(pos_center, pos) < u_x) {\n"
"        color = vec4(1.0, 0.0, 0.0, 1.0);\n"
"    } else\n"
"        discard;\n"
"}\n";

int w, h;

#define POINTS_COUNT 10
float points[POINTS_COUNT][2];
int current_point = 0;

GLint u_x_loc;
float x = 0.1;
int x_up = 1;

GLFWwindow *window;

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void error_exit(const char *str)
{
    fprintf(stderr, "%s\n", str);
    exit(1);
}

void window_refresh_callback()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_POINTS, 0, POINTS_COUNT);
    glfwSwapBuffers(window);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
//    printf("window_size %d %d\n", width, height);
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
//    printf("cursor_pos %f %f\n", xpos, ypos);
    current_point++;
    if (current_point >= POINTS_COUNT)
        current_point = 0;

    points[current_point][0] = (xpos*2.0)/w - 1.0;
    points[current_point][1] = (-ypos*2.0)/h + 1.0;

    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);

    if (x_up) {
        x+=0.01;
        if (x > 0.3) {
            x_up = 0;
        }
    } else {
        x-=0.01;
        if (x < 0.1) {
            x_up = 1;
        }
    }
    glUniform1f(u_x_loc, x);

    window_refresh_callback();
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        printf("right_mouse_button_press\n");
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        printf("right_mouse_button_release\n");
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    printf("scroll %f %f\n", xoffset, yoffset);
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        error_exit("Failed to initialize glfw.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(300, 300, "OpenGL 4.1 Geometry Shader", NULL, NULL);
    if (!window)
         error_exit("Failed to create window.");

    glfwMakeContextCurrent(window);

    if (gl3wInit())
        error_exit("Failed to initialize OpenGL.");

    if (!gl3wIsSupported(4, 1))
        error_exit("OpenGL 4.1 not supported.");

    glfwSetWindowRefreshCallback(window, window_refresh_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);


    for(int i=0; i<10; i++) {
        points[i][0] = 0.0f;
        points[i][1] = 0.0f;
    }

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);

    GLuint program;
    if(create_program((GLchar *)vertex_shader_str, (GLchar *)fragment_shader_str, (GLchar *)geometry_shader_str, &program))
         error_exit("Failder to create program.");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLint v_pos_loc;
    v_pos_loc = glGetAttribLocation(program, "v_pos");
    glEnableVertexAttribArray(v_pos_loc);
    glVertexAttribPointer(v_pos_loc, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    u_x_loc = glGetUniformLocation(program, "u_x");
    glUniform1f(u_x_loc, x);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    while (!glfwWindowShouldClose(window))
        glfwPollEvents();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
