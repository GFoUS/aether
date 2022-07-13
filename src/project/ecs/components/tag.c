#include "tag.h"

ecs_component_tag* ecs_component_tag_create(ecs_entity* entity, const char* tag) {
    ecs_component_tag* component = malloc(sizeof(ecs_component_tag));
    ecs_component_create(&component->component, entity, COMPONENT_TYPE_TAG);
    component->tag = tag;

    return component;
}

void ecs_component_tag_destroy(ecs_component_tag* component) {
    ecs_component_destroy(&component->component);
    free(component);
}

cJSON* ecs_component_tag_encode(ecs_component_tag* component) {
    cJSON* data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "tag", component->tag);
    return data;
}

void ecs_component_tag_decode(ecs_entity* entity, const cJSON* data) {
    const char* tag = cJSON_GetObjectItemCaseSensitive(data, "tag")->valuestring;
    ecs_component_tag_create(entity, tag);
}
