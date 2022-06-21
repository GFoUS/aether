#include "window.h"

#include "GLFW/glfw3.h"

typedef struct {
    u32 numWindows;
    window_window** windows;
} window_system;
static window_system windowSystem;

void glfw_error_callback(i32 error, const char* description) {
    ERROR("GLFW error. Code: %d, Description: %s", error, description);
}

void window_system_init() {
    if (!glfwInit()) {
        FATAL("GLFW failed to initialise");
    }

    glfwSetErrorCallback(glfw_error_callback);

    windowSystem.numWindows = 0;
    windowSystem.windows = malloc(0);
}

void window_system_deinit() {
    for (u32 i = 0; i < windowSystem.numWindows; i++) {
        if (!windowSystem.windows[i]->isDestroyed) {
            glfwDestroyWindow(windowSystem.windows[i]->window);
            windowSystem.windows[i]->isDestroyed = true;
        }
    }
    free(windowSystem.windows);

    glfwTerminate();
}

void glfw_key_callback(GLFWwindow* w, i32 key, i32 scancode, i32 action, i32 mods) {
    window_window* window = (window_window*)glfwGetWindowUserPointer(w);
    if (action == GLFW_PRESS) {
        event_bus_submit(KEY_PRESS, (void*)&key);
    }
}

void glfw_close_callback(GLFWwindow* w) {
    window_window* window = (window_window*)glfwGetWindowUserPointer(w);
    event_bus_submit(WINDOW_CLOSE, NULL);
}

window_window* window_create(window_config* config) {
    window_window* window = malloc(sizeof(window_window));
    CLEAR_MEMORY(window);
    static nextID = 0;
    window->id = nextID++;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    if (config->isFullscreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        i32 width, height;
        glfwGetMonitorWorkarea(monitor, NULL, NULL, &width, &height);
        window->width = (u32)width;
        window->height = (u32)height;
        window->window = glfwCreateWindow(width, height, config->title, NULL, NULL);
    } else {
        window->width = config->width;
        window->height = config->height;
        window->window = glfwCreateWindow((i32)config->width, (i32)config->height, config->title, NULL, NULL);
    }

    glfwSetWindowUserPointer(window->window, window);
    glfwSetKeyCallback(window->window, glfw_key_callback);
    glfwSetWindowCloseCallback(window->window, glfw_close_callback);

    windowSystem.numWindows++;
    windowSystem.windows = realloc(windowSystem.windows, sizeof(window_window) * windowSystem.numWindows);
    windowSystem.windows[windowSystem.numWindows - 1] = window;

    return window;
}

void window_destroy(window_window* window) {
    glfwDestroyWindow(window->window);
    window->isDestroyed = true;
    free(window);
}


void window_poll(window_window* window) {
    glfwPollEvents();
}