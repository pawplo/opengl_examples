#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

#define TEXTURE_WIDTH 4
#define TEXTURE_HEIGHT 4

const GLubyte texture_data[] = {
    255,0,0,255,   0,255,0,255,   255,255,0,255, 0,0,255,255,
    255,0,255,255, 255,0,0,255,   0,255,0,255,   255,255,0,255,
    0,0,255,255,   255,0,255,255, 255,0,0,255,   0,255,0,255,
    255,255,0,255, 0,0,255,255,   255,0,255,255, 255,0,0,255
};

const GLfloat position_data[] = {
    -0.5, -0.5,
    -0.5,  0.5,
     0.5, -0.5,
     0.5, -0.5,
    -0.5,  0.5,
     0.5,  0.5,
};

const GLfloat texcoord_data[] = {
    0.0, 0.0,
    1.0, 0.0,
    0.0, 1.0,
    0.0, 1.0,
    1.0, 0.0,
    1.0, 1.0,
};

const char *vertex_shader_str =
"#version 410 core\n"
"in vec2 vertex_position;\n"
"in vec2 vertex_texcoord;\n"
"out vec2 texcoord;\n"
"void main() {\n"
"   texcoord = vertex_texcoord;\n"
"   gl_Position = vec4(vertex_position, 0.0, 1.0);\n"
"}\n";

const char *fragment_shader_str =
"#version 410 core\n"
"in vec2 texcoord;\n"
"uniform sampler2D tex_sampler;\n"
"out vec4 color;\n"
"void main() {\n"
    "color = texture(tex_sampler, texcoord);\n"
"}\n";

GLuint program;
GLFWwindow* window;

void error_exit(const char *str)
{
    fprintf(stderr, "%s\n", str);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (!glfwInit())
        error_exit("Failed to initialize glfw.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(300, 300, "OpenGL 4.1 Texture", NULL, NULL);
    if (!window)
         error_exit("Failed to create window.");

    glfwMakeContextCurrent(window);

    if (gl3wInit())
        error_exit("Failed to initialize OpenGL.\n");

    if (!gl3wIsSupported(4, 1))
        error_exit("OpenGL 4.1 not supported.\n");

    printf("GL_VERSION=%s\n", glGetString(GL_VERSION));

    if(create_program((GLchar *)vertex_shader_str, (GLchar *)fragment_shader_str, (GLchar *)0, &program))
         error_exit("Failder to create program.\n");

    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint position;
    glGenBuffers(1, &position);
    glBindBuffer(GL_ARRAY_BUFFER, position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position_data), position_data, GL_STATIC_DRAW);

    GLuint pos_loc = glGetAttribLocation(program, "vertex_position");
    glEnableVertexAttribArray(pos_loc);
    glVertexAttribPointer(pos_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint texcoord;
    glGenBuffers(1, &texcoord);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord_data), texcoord_data, GL_STATIC_DRAW);

    GLuint tex_loc = glGetAttribLocation(program, "vertex_texcoord");
    glEnableVertexAttribArray(tex_loc);
    glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
