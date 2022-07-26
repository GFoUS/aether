#include "world.h"

void ecs_world_create(ecs_world* world) {
    CLEAR_MEMORY(world);
    world->entities = malloc(0);
    world->systems = malloc(0);

    world->updateTransformSystem = ecs_system_update_transform_create();
}

static ecs_world* worldGlobal = NULL;
ecs_world* ecs_world_get() {
    if (worldGlobal == NULL) {
        worldGlobal = malloc(sizeof(ecs_world));
        ecs_world_create(worldGlobal);
    }

    return worldGlobal;
}

void ecs_world_destroy() {
    ecs_world* world = ecs_world_get();

    u32 numEntities = world->numEntities;
    while (world->numEntities != 0) {
        ecs_entity_destroy(world->entities[0]);
    }
    free(world->entities);

    u32 numSystems = world->numSystems;
    while (world->numSystems != 0) {
        ecs_system_destroy(world->systems[0]);
    }
    free(world->systems);

    free(world);
    worldGlobal = NULL;
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