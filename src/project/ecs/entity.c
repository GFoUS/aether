#include "entity.h"

#include "component.h"
#include "world.h"

ecs_entity* ecs_entity_create() {
    ecs_entity* entity = malloc(sizeof(ecs_entity));
    CLEAR_MEMORY(entity);
    entity->components = malloc(0);

    ecs_world* world = ecs_world_get();
    world->numEntities++;
    world->entities = realloc(world->entities, sizeof(ecs_entity*) * world->numEntities);
    world->entities[world->numEntities - 1] = entity;

    return entity;
}

void ecs_entity_destroy(ecs_entity* entity) {
    ecs_world* world = ecs_world_get();

    while (entity->numComponents) {
        ecs_component_destroy(entity->components[0]);
    }

    for (u32 i = 0; i < world->numEntities; i++) {
        if (world->entities[i] == entity) {
            memcpy(world->entities[i],  world->entities[i+1], sizeof(ecs_entity*) * (world->numEntities - (i + 1)));
            world->numEntities--;
            break;
        }
    }

    free(entity->components);
    free(entity);
}

void ecs_entity_remove_component(ecs_entity* entity, ecs_component_type type) {
    for (u32 i = 0; i < entity->numComponents; i++) {
        if (entity->components[i]->type == type) {
            ecs_component_destroy(entity->components[i]);
            return;
        }
    }
}

ecs_component* ecs_entity_get_component(ecs_entity* entity, ecs_component_type type) {
    for (u32 i = 0; i < entity->numComponents; i++) {
        if (entity->components[i]->type == type) {
            return entity->components[i];
        }
    }

    return NULL;
}

cJSON* ecs_entity_encode(ecs_entity* entity) {
    cJSON* data = cJSON_CreateObject();
    
    cJSON* components = cJSON_CreateArray();
    for (u32 i = 0; i < entity->numComponents; i++) {
        cJSON_AddItemToArray(components, ecs_component_encode(entity->components[i]));
    }
    cJSON_AddItemToObject(data, "components", components);

    return data;
}

void ecs_entity_decode(const cJSON* data) {
    ecs_entity* entity = ecs_entity_create();

    const cJSON* components = cJSON_GetObjectItemCaseSensitive(data, "components");
    for (u32 i = 0; i < (u32)cJSON_GetArraySize(components); i++) {
        ecs_component_decode(entity, cJSON_GetArrayItem(components, i));
    }
}