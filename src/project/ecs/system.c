#include "system.h"

#include "world.h"

ecs_system* ecs_system_create(u32 numRequirements, ecs_component_type* requirements, void(*updateCallback)(ecs_entity*, void*), void(*destroyCallback)(void*), void* data) {    
    ecs_system* system = malloc(sizeof(ecs_system));
    CLEAR_MEMORY(system);

    system->numRequirements = numRequirements;
    system->requirements = requirements;
    system->updateCallback = updateCallback;
    system->destroyCallback = destroyCallback;
    system->data = data;

    ecs_world* world = ecs_world_get();
    world->numSystems++;
    world->systems = realloc(world->systems, sizeof(ecs_system*) * world->numSystems);
    world->systems[world->numSystems - 1] = system;

    return system;
}

void ecs_system_call(ecs_system* system, ecs_entity* entity) {
    system->updateCallback(entity, system->data);
}

void ecs_system_set_data(ecs_system* system, void* data) {
    system->data = data;
}

void ecs_system_destroy(ecs_system* system) {
    ecs_world* world = ecs_world_get();

    system->destroyCallback(system->data);

    for (u32 i = 0; i < world->numSystems; i++) {
        if (world->systems[i] == system) {
            memcpy(world->systems[i],  world->systems[i+1], sizeof(ecs_system*) * (world->numSystems - (i + 1)));
            world->numSystems--;
            break;
        }
    }

    free(system);
}