//https://gist.github.com/kawa-yoiko/cb2af1b67bd299b4672131a994ab6029
#include "gl.h"
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define N 15
static const char *btn_abbr[N] = {
    "A/x",
    "B/o",
    "X/[]",
    "Y/<>",
    "LBUM",
    "RBUM",
    "BACK",
    "STRT",
    "GUID",
    "LTHM",
    "RTHM",
    "DPUP",
    "DPRG",
    "DPDN",
    "DPLF",
};

#define M 6
static const char *axis_abbr[M] = {
    "LEFTX",
    "LEFTY",
    "RGHTX",
    "RGHTY",
    "LTRIG",
    "RTRIG"
};

int main()
{
    glfwInit();

    int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
    printf("%d\n", present);

    while (1) {
        printf("Joystick: ");
        puts(glfwGetJoystickName(GLFW_JOYSTICK_1));

        printf(" Buttons: ");
        int button_count;
        const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &button_count);
        for (int i = 0; i < button_count; i++)
            printf("%d%c", (int)buttons[i], i == button_count - 1 ? '\n' : ' ');

        printf("    Axes: ");
        int axis_count;
        const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axis_count);
        for (int i = 0; i < axis_count; i++)
            printf("%6.2lf%c", axes[i], i == axis_count - 1 ? '\n' : ' ');

        printf("    Hats: ");
        int hat_count;
        const unsigned char *hats = glfwGetJoystickHats(GLFW_JOYSTICK_1, &hat_count);
        for (int i = 0; i < hat_count; i++)
            printf("%d%c", (int)hats[i], i == hat_count - 1 ? '\n' : ' ');

        putchar('\n');

        /********/

        if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1)) {
            printf(" Gamepad: ");
            puts(glfwGetGamepadName(GLFW_JOYSTICK_1));

            GLFWgamepadstate state;
            glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
            for (int i = 0; i < N; i++) printf("%5s", btn_abbr[i]);
            putchar('\n');
            for (int i = 0; i < N; i++) printf("%5d", state.buttons[i]);
            putchar('\n');
            for (int i = 0; i < M; i++) printf("%7s", axis_abbr[i]);
            putchar('\n');
            for (int i = 0; i < M; i++) printf("%7.3f", state.axes[i]);
            putchar('\n');
        }

        puts("\n======\n");

        usleep(100000);
    }

    glfwTerminate();
    return 0;
}
