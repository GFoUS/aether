#pragma once

#include "project/ecs/component.h"
#include "cglm/cglm.h"

typedef struct {
    ecs_component component;
    mat4 matrix;
    vec3 translation;
    vec3 rotation;
    vec3 scale;
} ecs_component_transform;

ecs_component_transform* ecs_component_transform_create(ecs_entity* entity);
void ecs_component_transform_destroy(ecs_component_transform* component);

cJSON* ecs_component_transform_encode(ecs_component_transform* component);
void ecs_component_transform_decode(ecs_entity* entity, const cJSON* data);

void ecs_component_transform_update(ecs_component_transform* component, vec3 translation, versor rotation, vec3 scale);