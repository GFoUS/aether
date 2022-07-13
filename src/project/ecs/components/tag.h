#pragma once

#include "project/ecs/component.h"

#define MAX_TAG_LENGTH 256

typedef struct {
    ecs_component component;
    char* tag;
} ecs_component_tag;

ecs_component_tag* ecs_component_tag_create(ecs_entity* entity, const char* tag);
void ecs_component_tag_destroy(ecs_component_tag* component);

cJSON* ecs_component_tag_encode(ecs_component_tag* component);
void ecs_component_tag_decode(ecs_entity* entity, const cJSON* data);

void ecs_component_tag_update(ecs_component_tag* component, const char* tag);