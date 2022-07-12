#include "world.h"

ecs_world* ecs_world_create() {
    ecs_world* world = malloc(sizeof(ecs_world));
    CLEAR_MEMORY(world);
    world->entities = malloc(0);
    world->systems = malloc(0);

    return world;
}

ecs_world* ecs_world_get() {
    static ecs_world* world = NULL;
    if (world == NULL) {
        world = ecs_world_create();
    }

    return world;
}

void ecs_world_destroy() {
    ecs_world* world = ecs_world_get();
    if (world != NULL) {
        for (u32 i = 0; i < world->numEntities; i++) {
            ecs_entity_destroy(world->entities[i]);
        }
        free(world->entities);

        for (u32 i = 0; i < world->numSystems; i++) {
            free(world->systems[i]);
        }
        free(world->systems);

        free(world);
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
    ecs_world* world = ecs_world_get();
    for (u32 i = 0; i < (u32)cJSON_GetArraySize(entities); i++) {
        ecs_entity_decode(cJSON_GetArrayItem(entities, i));
    }
}