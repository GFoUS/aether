#pragma once

#include "entity.h"

typedef struct ecs_world_t ecs_world;

typedef struct {    
    u32 numRequirements;
    ecs_component_type* requirements;

    void(*callback)(ecs_entity*, void*);
    void* data;
} ecs_system;

ecs_system* ecs_system_create(u32 numRequirements, ecs_component_type* requirements, void(*callback)(ecs_entity*, void*));
void ecs_system_call(ecs_system* system, ecs_entity* entity);