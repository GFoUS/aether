#pragma once

#include "core/core.h"
#include "graphics/model.h"

#include "vulkan/context.h"
#include "vulkan/descriptor.h"

#include "cJSON.h"

typedef enum {
    ASSET_TYPE_MODEL = 0
} project_asset_type;

typedef struct {
    project_asset_type type;
    const char* path;
    u32 numTimesLoaded;
    void* loadedData;
} project_asset;

void project_asset_create(project_asset* asset, project_asset_type type, const char* path);
void project_asset_destroy(project_asset* asset);

void project_asset_decode(project_asset* out, const cJSON* data);
cJSON* project_asset_encode(project_asset* asset);

typedef struct {
    project_asset* asset;
    model_model* model;
} project_asset_model;

typedef struct {
    vulkan_context* ctx;
    vulkan_descriptor_set_layout* materialSetLayout;
    vulkan_descriptor_set_layout* modelSetLayout;
} project_asset_model_load_config;

project_asset_model* project_asset_model_load(project_asset* asset, project_asset_model_load_config* config);
void project_asset_model_unload(project_asset_model* model);