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

    return project;
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

    INFO("Loaded project from: %s", path);

    return project;
}

project_project* project_get() {
    return project;
}


void project_destroy() {
    if (project) {
        if (project->json) cJSON_Delete(project->json); // Need to keep cJSON's strings alive till the end of the program
        free(project);
    }
}

void project_save(const char* path) {
    // Encode project
    cJSON* data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "name", project->name);
    cJSON_AddItemToObject(data, "meta", project_meta_encode(&project->meta));

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