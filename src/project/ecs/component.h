#pragma once

#include "core/core.h"

#include "cJSON.h"

typedef enum ecs_component_type {
    COMPONENT_TYPE_MODEL = 0,
    COMPONENT_TYPE_TAG
} ecs_component_type;

typedef struct ecs_entity_t ecs_entity;

typedef struct ecs_component_t {
    ecs_entity* entity;
    ecs_component_type type;
} ecs_component;

void ecs_component_create(ecs_component* component, ecs_entity* entity, ecs_component_type type);
void ecs_component_destroy(ecs_component* component);

cJSON* ecs_component_encode(ecs_component* component);
void ecs_component_decode(ecs_entity* entity, const cJSON* component);