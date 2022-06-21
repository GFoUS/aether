#pragma once

#include "types.h"

typedef void(*event_listener_fn)(void* data);
typedef event_listener_fn event_listener_generic[1000];

typedef enum {
    WINDOW_CLOSE,
    KEY_PRESS
} aether_event_type;

typedef struct {
    void(*windowClose)(void* data);
    void(*keyPress)(void* data);
} event_listener;

typedef struct {
    u64 numListeners;
    event_listener_fn** listeners;
} event_bus;

void event_bus_init();
void event_bus_deinit();

void event_bus_submit(u64 type, void* data);
void event_bus_listen(event_listener_generic listener);