#include "aether.h"

#include "graphics/window.h"
#include "nfd.h"

void aether_init() {
    // internal
    event_bus_init();
    window_system_init();

    // external
    NFD_Init();
}

void aether_deinit() {
    // internal
    window_system_deinit();
    event_bus_deinit();

    // external
    NFD_Quit();
}