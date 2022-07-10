#pragma once

#include "core/core.h"
#include "cJSON.h"

typedef struct {
    u64 version;
} project_meta;

typedef struct {
    const char* name;
    project_meta meta;

    cJSON* json;
} project_project;

project_project* project_create(const char* name);
project_project* project_load(const char* path);

project_project* project_get();
void project_save(const char* path);

void project_destroy();
