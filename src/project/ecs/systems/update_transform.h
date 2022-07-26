#pragma once

#include "project/ecs/system.h"

typedef struct {
    ecs_system* system;
} ecs_system_update_transform;

ecs_system_update_transform* ecs_system_update_transform_create();