#include "model.h"

#include "project/project.h"

ecs_component_model* ecs_component_model_create(ecs_entity* entity, project_asset* asset) {
    assert(asset->type == ASSET_TYPE_MODEL);

    ecs_component_model* component = malloc(sizeof(ecs_component_model));
    CLEAR_MEMORY(component);
    ecs_component_create(&component->component, entity, COMPONENT_TYPE_MODEL);
    component->asset = asset;

    return component;
}

void ecs_component_model_destroy(ecs_component_model* component) {
    ecs_component_destroy(&component->component);
    free(component);
}

cJSON* ecs_component_model_encode(ecs_component_model* component) {
    cJSON* data = cJSON_CreateObject();
    size_t assetIndex = (component->asset - project_get()->assets) / sizeof(project_asset);
    cJSON_AddNumberToObject(data, "asset", (double)assetIndex);
    return data;
}  

void ecs_component_model_decode(ecs_entity* entity, const cJSON* data) {
    u32 assetIndex = cJSON_GetObjectItemCaseSensitive(data, "asset")->valueint;
    ecs_component_model_create(entity, &project_get()->assets[assetIndex]);
}