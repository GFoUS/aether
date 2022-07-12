#pragma once

#include "project/ecs/world.h"
#include "project/asset.h"

typedef struct {
    ecs_component component;
    project_asset* asset;
} ecs_component_model;

ecs_component_model* ecs_component_model_create(ecs_entity* entity, project_asset* asset);
void ecs_component_model_destroy(ecs_component_model* component);

cJSON* ecs_component_model_encode(ecs_component_model* component);
void ecs_component_model_decode(ecs_entity* entity, const cJSON* data);