#pragma once

#include "entity.h"

typedef struct ecs_world_t ecs_world;

typedef struct {    
    u32 numRequirements;
    ecs_component_type* requirements;

    void(*updateCallback)(ecs_entity*, void*);
    void(*destroyCallback)(void*);
    void* data;
} ecs_system;

ecs_system* ecs_system_create(u32 numRequirements, ecs_component_type* requirements, void(*updateCallback)(ecs_entity*, void*), void(*destroyCallback)(void*), void* data);
void ecs_system_destroy(ecs_system* system);

void ecs_system_call(ecs_system* system, ecs_entity* entity);
void ecs_system_set_data(ecs_system* system, void* data);