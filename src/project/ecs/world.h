#pragma once

#include "component.h"
#include "entity.h"
#include "system.h"
#include "systems/update_transform.h"

#include "cJSON.h"

typedef struct ecs_world_t {
    u32 numEntities;
    ecs_entity** entities;

    u32 numSystems;
    ecs_system** systems;

    ecs_system_update_transform* updateTransformSystem;
} ecs_world;

ecs_world* ecs_world_get();
void ecs_world_destroy();

void ecs_world_update();

cJSON* ecs_world_save_entities();
void ecs_world_load_entities(const cJSON* entities);

