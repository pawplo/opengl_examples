//#include <GL/gl3w.h>
#include "gl.h"
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

GLFWcursor *cursor_ew;
GLFWcursor *cursor_ns;

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
GLFWwindow *w;
GLFWwindow *w2;

volatile _Bool w2_show = 0;

void error_exit(const char *str)
{
    fprintf(stderr, "%s\n", str);
    exit(1);
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    printf("key %d %d %d %d\n", key, scancode, action, mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (window == w && w2_show) return;
    printf("cursor[%d] %f %f\n", window == w, xpos, ypos);
    if (xpos > 150 && xpos < 300 && ypos > 150 && ypos < 300) {
        glfwSetCursor(window, cursor_ew);
    } else {
        glfwSetCursor(window, NULL);
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    printf("scroll %f %f\n", xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    printf("mouse_button %d %d %d\n", button, action, mods);
    if (w2_show == 1) return;
    w2_show = 1;
    w2 = glfwCreateWindow(200, 200, "🍏", NULL, NULL);
    if (!w2) error_exit("Failed to create window.");
    glfwShowWindow(w2);
//    glfwMakeContextCurrent(w2);
    glfwSetCursorPosCallback(w2, cursor_position_callback);
}

void char_mods_callback(GLFWwindow *window, unsigned int codepoint, int mods)
{
    printf("char_mods %d %d\n", codepoint, mods);
}

void char_callback(GLFWwindow *window, unsigned int codepoint)
{
    printf("char %d\n", codepoint);
}

void drop_callback(GLFWwindow *window, int path_count, const char* paths[])
{
    for (int i = 0; i < path_count; i++) {
        printf("drop[%d] %s\n", i, paths[i]);
    }
}

void cursor_enter_callback(GLFWwindow *window, int entered)
{
    printf("cursor_enter %d\n", entered);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    printf("framebuffer_size %d %d\n", width, height);
//    glViewport(0, 0, width, height);
}

void window_pos_callback(GLFWwindow *window, int xpos, int ypos)
{
    printf("window_pos %d %d\n", xpos, ypos);
}

void window_focus_callback(GLFWwindow* window, int focused)
{
    printf("focus[%d] %d\n", window == w, focused);
    if (window == w && focused) {
        if (w2_show) glfwFocusWindow(w2);
        glfwRequestWindowAttention(w2);
    }
}

int main(int argc, char *argv[])
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        error_exit("Failed to initialize glfw.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
//    glfwWindowHint(GLFW_FLOATING, GL_TRUE);

    w = glfwCreateWindow(600, 600, "🍏🍎🍐🍊🍋🍋🍌🍉🍇🍓🫐🍈🍒🍑🥭🍍🥥", NULL, NULL);
    if (!w) error_exit("Failed to create window.");

    glfwWindowHint(GLFW_FLOATING, GL_TRUE);

    //glfwMakeContextCurrent(w2);

    glfwMakeContextCurrent(w);

    cursor_ew = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
    cursor_ns = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);

//    if (gl3wInit())
//        error_exit("Failed to initialize OpenGL.\n");

//    if (!gl3wIsSupported(4, 1))
//        error_exit("OpenGL 4.1 not supported.\n");

//    printf("GL_VERSION=%s\n", glGetString(GL_VERSION));

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

    if (glfwRawMouseMotionSupported()) {
        printf("glfwRawMouseMotionSupported()");
        glfwSetInputMode(w, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    glfwSetKeyCallback(w, key_callback);
    glfwSetCursorPosCallback(w, cursor_position_callback);
    glfwSetScrollCallback(w, scroll_callback);
    glfwSetMouseButtonCallback(w, mouse_button_callback);
    glfwSetCharModsCallback(w, char_mods_callback);
    glfwSetCharCallback(w, char_callback);
    glfwSetDropCallback(w, drop_callback);
    glfwSetCursorEnterCallback(w, cursor_enter_callback);
    glfwSetFramebufferSizeCallback(w, framebuffer_size_callback);
    glfwSetWindowPosCallback(w, window_pos_callback);
    glfwSetWindowFocusCallback(w, window_focus_callback);

//    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
//    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
//    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

 //   glfwSetInputMode(w, GLFW_LOCK_KEY_MODS, 1);


    while (!glfwWindowShouldClose(w)) {
        if (w2_show) {
            while (!glfwWindowShouldClose(w2)) {
                glfwPollEvents();
            }
            w2_show = 0;
            //glfwFocusWindow(w);
            glfwDestroyWindow(w2);
            //glfwHideWindow(w2);
//            glfwMakeContextCurrent(w);

        } else {

            glClear(GL_COLOR_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glfwSwapBuffers(w);
            glfwPollEvents();
        }
    }

    printf("terminate\n");
    glfwTerminate();
    return 0;
}
