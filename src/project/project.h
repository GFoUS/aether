#pragma once

#include "core/core.h"
#include "cJSON.h"

#include "asset.h"

typedef struct {
    u64 version;
} project_meta;

typedef struct {
    const char* name;
    project_meta meta;

    u32 numAssets;
    project_asset* assets;

    cJSON* json;
} project_project;

project_project* project_create(const char* name);
void project_destroy();

void project_save(const char* path);
project_project* project_load(const char* path);

project_project* project_get();

project_asset* project_add_asset(project_asset_type type, const char* path);
project_asset** project_get_assets_with_type(u32* numAssets, project_asset_type type);
