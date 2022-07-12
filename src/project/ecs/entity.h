#pragma once

#include "core/core.h"

#include "cJSON.h"

typedef enum ecs_component_type ecs_component_type;
typedef struct ecs_component_t ecs_component;
typedef struct ecs_world_t ecs_world;

typedef struct ecs_entity_t {
    u32 numComponents;
    ecs_component** components;
} ecs_entity;

ecs_entity* ecs_entity_create();
void ecs_entity_destroy(ecs_entity* entity);

void ecs_entity_remove_component(ecs_entity* entity, ecs_component_type type);
ecs_component* ecs_entity_get_component(ecs_entity* entity, ecs_component_type type);

cJSON* ecs_entity_encode(ecs_entity* entity);
void ecs_entity_decode(const cJSON* data);
