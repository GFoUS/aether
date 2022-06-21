#include "aether.h"

void aether_init() {
    event_bus_init();
    window_system_init();
}

void aether_deinit() {
    window_system_deinit();
    event_bus_deinit();
}