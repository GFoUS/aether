#include "asset.h"

void project_asset_create(project_asset* asset, project_asset_type type, const char* path) {
    CLEAR_MEMORY(asset);
    asset->type = type;
    asset->path = path;
}

void project_asset_destroy(project_asset* asset) {
    if (asset->numTimesLoaded != 0) {
        switch (asset->type) {
        case(ASSET_TYPE_MODEL) : asset->numTimesLoaded = 1; project_asset_model_unload((project_asset_model*)asset->loadedData); break; // numTimesLoaded is set to 1 to ensure the model is unloaded
        }
    }
}

void project_asset_decode(project_asset* out, const cJSON* data) {
    CLEAR_MEMORY(out);
    out->type = (project_asset_type)cJSON_GetObjectItemCaseSensitive(data, "type")->valueint;
    out->path = cJSON_GetObjectItemCaseSensitive(data, "path")->valuestring;
}  

cJSON* project_asset_encode(project_asset* asset) {
    cJSON* data = cJSON_CreateObject();
    cJSON_AddNumberToObject(data, "type", (double)asset->type);
    cJSON_AddStringToObject(data, "path", asset->path);
    return data;
}

project_asset_model* project_asset_model_load(project_asset* asset, project_asset_model_load_config* config) {
    assert(asset->type == ASSET_TYPE_MODEL);
    if (asset->numTimesLoaded == 0) {
        project_asset_model* model = malloc(sizeof(project_asset_model));
        CLEAR_MEMORY(model);

        model->asset = asset;
        model->model = model_load(asset->path, config->ctx, config->materialSetLayout, config->modelSetLayout);

        model->asset->numTimesLoaded++;
        model->asset->loadedData = (void*)model;

        return model;
    } else {
        return (project_asset_model*)asset->loadedData;
    }
}

void project_asset_model_unload(project_asset_model* model) {
    model->asset->numTimesLoaded--;
    if (model->asset->numTimesLoaded == 0) {
        model->asset->loadedData = NULL;
        model_unload(model->model);
        free(model);
    }
}