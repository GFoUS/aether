#pragma once

#include "types.h"

typedef void(*event_listener_fn)(void* data, void* userPtr);
typedef event_listener_fn event_listener_generic[1024];

typedef enum {
    WINDOW_CLOSE = 0,
    KEY_PRESS,
    LAST_EVENT_TYPE
} aether_event_type;

typedef struct {
    void(*windowClose)(void*, void*);
    void(*keyPress)(void*, void*);
} event_listener;

event_listener* event_listener_create();

typedef struct {
    u64 numListeners;
    event_listener_fn** listeners;
    void** userPtrs;
} event_bus;

void event_bus_init();
void event_bus_deinit();

void event_bus_submit(u64 type, void* data);
void event_bus_listen(event_listener_generic listener, void* userPtr);