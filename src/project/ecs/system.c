#include "system.h"

#include "world.h"

ecs_system* ecs_system_create(u32 numRequirements, ecs_component_type* requirements, void(*callback)(ecs_entity*, void*)) {    
    ecs_system* system = malloc(sizeof(ecs_system));
    CLEAR_MEMORY(system);

    system->numRequirements = 0;
    system->requirements = requirements;
    system->callback = callback;

    ecs_world* world = ecs_world_get();
    world->numSystems++;
    world->systems = realloc(world->systems, sizeof(ecs_system*) * world->numSystems);
    world->systems[world->numSystems - 1] = system;

    return system;
}

void ecs_system_call(ecs_system* system, ecs_entity* entity) {
    system->callback(entity, system->data);
}