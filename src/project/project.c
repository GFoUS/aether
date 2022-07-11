#include "project.h"

// project_meta
void project_meta_decode(project_meta* out, const cJSON* data) {
    const cJSON* meta = cJSON_GetObjectItemCaseSensitive(data, "meta");
    out->version = cJSON_GetObjectItemCaseSensitive(meta, "version")->valueint;
}

cJSON* project_meta_encode(project_meta* meta) {
    cJSON* out = cJSON_CreateObject();
    cJSON_AddNumberToObject(out, "version", (double)meta->version);
    return out;
}

// project_project
static project_project* project = NULL;

project_project* project_create(const char* name) {
    project_destroy();
    project = malloc(sizeof(project_project));
    CLEAR_MEMORY(project);

    project->name = name;
    project->numAssets = 0;
    project->assets = malloc(0);

    return project;
}

void project_destroy() {
    if (project) {

        for (u32 i = 0; i < project->numAssets; i++) {
            project_asset_destroy(&project->assets[i]);
        }
        free(project->assets);

        if (project->json) cJSON_Delete(project->json); // Need to keep cJSON's strings alive till the end of the program
        free(project);
    }
}

void project_save(const char* path) {
    // Encode project
    cJSON* data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "name", project->name);
    cJSON_AddItemToObject(data, "meta", project_meta_encode(&project->meta));
    
    cJSON* assets = cJSON_AddArrayToObject(data, "assets");
    for (u32 i = 0; i < project->numAssets; i++) {
        cJSON_AddItemToArray(assets, project_asset_encode(&project->assets[i]));
    }

    // Save to file
    const char* projectFileText = cJSON_Print(data);
    FILE* file = fopen(path, "w");
    if (!file) {
        FATAL("Error opening project file: %s for writing", path);
    }

    fprintf(file, "%s", projectFileText);
    fclose(file);

    INFO("Saved project data to file %s", path);
}

project_project* project_load(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        FATAL("Error loading project file: %s", path);
    }

    fseek(file, 0L, SEEK_END);
    u64 fileSize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char* projectFileText = malloc(fileSize);
    CLEAR_MEMORY_ARRAY(projectFileText, fileSize);
    fread(projectFileText, fileSize, 1, file);
    fclose(file);

    project_destroy();
    project = malloc(sizeof(project_project));
    project->json = cJSON_Parse(projectFileText);
    project->name = cJSON_GetObjectItemCaseSensitive(project->json, "name")->valuestring;
    
    project_meta_decode(&project->meta, project->json);

    const cJSON* assets = cJSON_GetObjectItemCaseSensitive(project->json, "assets");
    project->numAssets = cJSON_GetArraySize(assets);
    project->assets = malloc(sizeof(project_asset) * project->numAssets);
    for (u32 i = 0; i < project->numAssets; i++) {
        project_asset_decode(&project->assets[i], cJSON_GetArrayItem(assets, (i32)i));
    }

    INFO("Loaded project from: %s", path);

    return project;
}

project_project* project_get() {
    return project;
}

project_asset* project_add_asset(project_asset_type type, const char* path) {
    project->numAssets++;
    project->assets = realloc(project->assets, sizeof(project_asset) * project->numAssets);
    project_asset_create(&project->assets[project->numAssets - 1], type, path);
    return &project->assets[project->numAssets - 1];
}

project_asset** project_get_assets_with_type(u32* numAssets, project_asset_type type) { // Used a double for loop to avoid reallocing
    *numAssets = 0; 
    if (project == NULL) { 
        return NULL;
    }

    for (u32 i = 0; i < project->numAssets; i++) {
        if (project->assets[i].type == type) {
            (*numAssets)++;
        }
    }

    project_asset** assets = malloc(sizeof(project_asset*) * (*numAssets));
    CLEAR_MEMORY_ARRAY(assets, *numAssets);

    u32 ptr = 0;
    for (u32 i = 0; i < project->numAssets; i++) {
        if (project->assets[i].type == type) {
            assets[ptr++] = &project->assets[i];
        }
    }

    return assets;
}