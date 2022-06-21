#pragma once

#include "core/core.h"

typedef struct GLFWwindow GLFWwindow;

void window_system_init();
void window_system_deinit();

typedef struct {
    u32 width;
    u32 height;
    bool isFullscreen;
    const char* title;
} window_config;

typedef struct {
    u32 id;
    GLFWwindow* window;
    u32 width;
    u32 height;
    bool isDestroyed;
} window_window;

window_window* window_create(window_config* config);
void window_destroy(window_window* window);

void window_poll(window_window* window);