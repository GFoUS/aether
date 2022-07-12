#include "component.h"

#include "entity.h"
#include "project/ecs/components/model.h"
#include "project/project.h"

void ecs_component_create(ecs_component* component, ecs_entity* entity, ecs_component_type type) {
    CLEAR_MEMORY(component);

    component->entity = entity;
    component->type = type;

    component->entity->numComponents++;
    component->entity->components = realloc(component->entity->components, sizeof(ecs_component*) * component->entity->numComponents);
    component->entity->components[component->entity->numComponents - 1] = component;

    return component;
}

void ecs_component_destroy(ecs_component* component) {
    for (u32 i = 0; i < component->entity->numComponents; i++) {
        if (component->entity->components[i]->type == component->type) {
            memcpy(component->entity->components[i], component->entity->components[i+1], sizeof(ecs_component*) * (component->entity->numComponents - (i + 1)));
            component->entity->numComponents--;
            break;
        }
    }

    free(component);
}

cJSON* ecs_component_encode(ecs_component* component) {
    cJSON* data = cJSON_CreateObject();
    cJSON_AddNumberToObject(data, "type", component->type);

    switch (component->type) {
    case (COMPONENT_TYPE_MODEL) : cJSON_AddItemToObject(data, "data", ecs_component_model_encode((ecs_component_model*)component)); break;
    }

    return data;
}

void ecs_component_decode(ecs_entity* entity, const cJSON* component) {
    ecs_component_type type = (ecs_component_type)cJSON_GetObjectItemCaseSensitive(component, "type")->valueint;
    const cJSON* data = cJSON_GetObjectItemCaseSensitive(component, "data");
    switch (type) {
    case (COMPONENT_TYPE_MODEL) : ecs_component_model_decode(entity, data); break;
    }
}