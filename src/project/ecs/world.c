#include "world.h"

void ecs_world_create(ecs_world* world) {
    CLEAR_MEMORY(world);
    world->entities = malloc(0);
    world->systems = malloc(0);
}

ecs_world* ecs_world_get() {
    static ecs_world* world = NULL;
    if (world == NULL) {
        world = malloc(sizeof(ecs_world));
        ecs_world_create(world);
    }

    return world;
}

void ecs_world_destroy() {
    ecs_world* world = ecs_world_get();
    if (world != NULL) {
        u32 numEntities = world->numEntities;
        while (world->numEntities != 0) {
            ecs_entity_destroy(world->entities[0]);
        }
        CLEAR_MEMORY_ARRAY(world->entities, numEntities);

        u32 numSystems = world->numSystems;
        for (u32 i = 0; i < world->numSystems; i++) {
            free(world->systems[i]);
        }
        CLEAR_MEMORY_ARRAY(world->systems, numSystems);
    }
}

void ecs_world_update() {
    ecs_world* world = ecs_world_get();
    for (u32 i = 0; i < world->numSystems; i++) {
        for (u32 j = 0; j < world->numEntities; j++) {
            bool requirementsMet = true;

            for (u32 k = 0; k < world->systems[i]->numRequirements; k++) {
                if (ecs_entity_get_component(world->entities[j], world->systems[i]->requirements[k]) == NULL) {
                    requirementsMet = false;
                }
            }

            if (requirementsMet) {
                ecs_system_call(world->systems[i], world->entities[j]);
            }
        }
    }
}

cJSON* ecs_world_save_entities() {
    ecs_world* world = ecs_world_get();
    cJSON* entities = cJSON_CreateArray();

    for (u32 i = 0; i < world->numEntities; i++) {
        cJSON_AddItemToArray(entities, ecs_entity_encode(world->entities[i]));
    }

    return entities;
}

void ecs_world_load_entities(const cJSON* entities) {
    ecs_world_destroy();
    ecs_world* world = ecs_world_get();
    for (u32 i = 0; i < (u32)cJSON_GetArraySize(entities); i++) {
        ecs_entity_decode(cJSON_GetArrayItem(entities, i));
    }
}