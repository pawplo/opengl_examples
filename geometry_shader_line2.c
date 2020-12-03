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
"layout (lines) in;\n"
"layout (triangle_strip, max_vertices = 6) out;\n"
"uniform float u_x;\n"

"void main() {\n"
"    vec2 vvv = normalize(gl_in[1].gl_Position.xy - gl_in[0].gl_Position.xy ) * u_x;\n"

"    gl_Position = gl_in[0].gl_Position + vec4( -vvv.y, vvv.x, 0.0, 0.0);\n"
"    EmitVertex();\n"

"    gl_Position = gl_in[1].gl_Position + vec4( -vvv.y, vvv.x, 0.0, 0.0);\n"
"    EmitVertex();\n"

"    gl_Position = gl_in[0].gl_Position + vec4( vvv.y, -vvv.x, 0.0, 0.0);\n"
"    EmitVertex();\n"

"    EndPrimitive();\n"

"    gl_Position = gl_in[0].gl_Position + vec4( vvv.y, -vvv.x, 0.0, 0.0);\n"
"    EmitVertex();\n"

"    gl_Position = gl_in[1].gl_Position + vec4( -vvv.y, vvv.x, 0.0, 0.0);\n"
"    EmitVertex();\n"

"    gl_Position = gl_in[1].gl_Position + vec4( vvv.y, -vvv.x, 0.0, 0.0);\n"
"    EmitVertex();\n"

"    EndPrimitive();\n"
"}\n";

static const char *fragment_shader_str =
"#version 410\n"
"out vec4 color;\n"
"void main() {\n"
"    color = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\n";

static const char *geometry_shader_circle_str =
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

static const char *fragment_shader_circle_str =
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

#define POINTS_COUNT 30
float points[POINTS_COUNT][2];
int current_point = 0;

GLint u_x_loc;
GLint u_x_loc_circle;
float x = 0.1;
int x_up = 1;

GLFWwindow *window;

GLuint program;
GLuint program_circle;

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
//    printf("window_refresh\n");
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glUniform1f(u_x_loc, x);

//    glDrawArrays(GL_LINE_STRIP, 0, POINTS_COUNT);
    glDrawArrays(GL_LINE_STRIP, 0, current_point+1);
    glDrawArrays(GL_LINE_STRIP, current_point+1, POINTS_COUNT-current_point-1);

    glUseProgram(program_circle);
    glUniform1f(u_x_loc_circle, x);

    glDrawArrays(GL_POINTS, 0, current_point+1);
    glDrawArrays(GL_POINTS, current_point+1, POINTS_COUNT-current_point-1);

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

static void refresh(GLFWwindow* window, double x_point, double y_point)
{
//    printf("refresh %f %f\n", xpos, ypos);
    current_point++;
    if (current_point == POINTS_COUNT) {
        points[0][0] = points[POINTS_COUNT-1][0];
        points[0][1] = points[POINTS_COUNT-1][1];
        current_point = 1;
    }

    points[current_point][0] = x_point;
    points[current_point][1] = y_point;

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

    window_refresh_callback();
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
//    printf("cursor_pos %f %f\n", xpos, ypos);
    refresh(window,  (xpos*2.0)/w - 1.0, (-ypos*2.0)/h + 1.0);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT ) {

        if (action == GLFW_PRESS)
            printf("right_mouse_button_press\n");
        else if (action == GLFW_RELEASE)
            printf("right_mouse_button_release\n");

    } else if (button == GLFW_MOUSE_BUTTON_LEFT ) {

        if (action == GLFW_PRESS)
            printf("left_mouse_button_press\n");
        else if (action == GLFW_RELEASE)
            printf("left_mouse_button_release\n");

    } else {

        if (action == GLFW_PRESS)
            printf("unknown_mouse_button_press\n");
        else if (action == GLFW_RELEASE)
            printf("unknown_mouse_button_release\n");
    }

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

//    if (!gl3wIsSupported(4, 1))
//        error_exit("OpenGL 4.1 not supported.");

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

    if(create_program((GLchar *)vertex_shader_str, (GLchar *)fragment_shader_str, (GLchar *)geometry_shader_str, &program)) {
//    if(create_program((GLchar *)vertex_shader_str, (GLchar *)fragment_shader_str, (GLchar *)0, &program)) {
         error_exit("Failder to create program.");
    }

    if(create_program((GLchar *)vertex_shader_str, (GLchar *)fragment_shader_circle_str, (GLchar *)geometry_shader_circle_str, &program_circle)) {
//    if(create_program((GLchar *)vertex_shader_str, (GLchar *)fragment_shader_str, (GLchar *)0, &program)) {
         error_exit("Failder to create program.");
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glUseProgram(program);

    GLint v_pos_loc;
    v_pos_loc = glGetAttribLocation(program, "v_pos");
    glEnableVertexAttribArray(v_pos_loc);
    glVertexAttribPointer(v_pos_loc, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    u_x_loc = glGetUniformLocation(program, "u_x");
//    glUniform1f(u_x_loc, x);

    glUseProgram(program_circle);

    GLint v_pos_loc_circle;
    v_pos_loc_circle = glGetAttribLocation(program_circle, "v_pos");
    glEnableVertexAttribArray(v_pos_loc_circle);
    glVertexAttribPointer(v_pos_loc_circle, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    u_x_loc_circle = glGetUniformLocation(program_circle, "u_x");
//    glUniform1f(u_x_loc_circle, x);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        refresh(window, points[current_point][0], points[current_point][1]);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
