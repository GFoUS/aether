#include "event.h"

#include <stdlib.h>
#include <memory.h>

event_listener* event_listener_create() {
    event_listener_generic* listener = malloc(sizeof(event_listener_generic));
    memset(listener, 0, sizeof(event_listener_fn) * 1024);

    return (event_listener*)*listener;
}

static event_bus bus;

void event_bus_init() {
    bus.numListeners = 0;
    bus.listeners = malloc(sizeof(event_listener_generic));
}

void event_bus_deinit() {
    free(bus.listeners);
}

void event_bus_submit(u64 type, void* data) {
    for (u32 i = 0; i < bus.numListeners; i++) {
        if (bus.listeners[i][type]) bus.listeners[i][type](data);
    }
}

void event_bus_listen(event_listener_generic listener) {
    bus.numListeners++;
    bus.listeners = realloc(bus.listeners, sizeof(event_listener_generic) * bus.numListeners);
    bus.listeners[bus.numListeners - 1] = listener;
}