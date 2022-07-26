#include "transform.h"

#include "project/ecs/components/model.h"

#define PI 3.1415926535897932384626433832795

ecs_component_transform* ecs_component_transform_create(ecs_entity* entity) {
    ecs_component_transform* component = malloc(sizeof(ecs_component_transform));
    CLEAR_MEMORY(component);
    ecs_component_create(&component->component, entity, COMPONENT_TYPE_TRANSFORM);
    glm_mat4_identity(component->matrix);
    glm_vec3_zero(component->translation);
    glm_vec3_zero(component->rotation);
    glm_vec3_one(component->scale);

    return component;
}

void ecs_component_transform_destroy(ecs_component_transform* component) {
    ecs_component_destroy(&component->component);
    free(component);
}

cJSON* ecs_component_transform_encode(ecs_component_transform* component) {
    cJSON* data = cJSON_CreateObject();

    cJSON* translation = cJSON_AddArrayToObject(data, "translation");
    for (u32 i = 0; i < 3; i++) {
        cJSON_AddItemToArray(translation, cJSON_CreateNumber((double)component->translation[i]));
    }
    
    cJSON* rotation = cJSON_AddArrayToObject(data, "rotation");
    for (u32 i = 0; i < 4; i++) {
        cJSON_AddItemToArray(rotation, cJSON_CreateNumber((double)component->rotation[i]));
    }

    cJSON* scale = cJSON_AddArrayToObject(data, "scale");
    for (u32 i = 0; i < 3; i++) {
        cJSON_AddItemToArray(scale, cJSON_CreateNumber((double)component->scale[i]));
    }

    return data;
}

void ecs_component_transform_update(ecs_component_transform* component, vec3 translation, vec3 rotation, vec3 scale) {
    memcpy(component->translation, translation, sizeof(vec3));
    memcpy(component->rotation, rotation, sizeof(vec3));
    memcpy(component->scale, scale, sizeof(vec3));

    mat4 t, r, s;
    glm_mat4_identity(component->matrix);
    glm_mat4_identity(t);
    glm_mat4_identity(r);
    glm_mat4_identity(s);
    
    glm_scale(s, component->scale);

    float degreesToRadians = (float)(PI/180);
    vec3 rotationRadians = { rotation[0] * degreesToRadians, rotation[1] * degreesToRadians, rotation[2] * degreesToRadians };
    mat4 rotationMatrix;
    glm_euler(rotationRadians, r);

    glm_translate(t, component->translation);

    glm_mat4_mul(component->matrix, s, component->matrix);
    glm_mat4_mul(component->matrix, r, component->matrix);
    glm_mat4_mul(component->matrix, t, component->matrix);
}

void ecs_component_transform_decode(ecs_entity* entity, const cJSON* data) {
    ecs_component_transform* component = ecs_component_transform_create(entity);

    cJSON* translation = cJSON_GetObjectItemCaseSensitive(data, "translation");
    for (u32 i = 0; i < 3; i++) {
        component->translation[i] = (float)cJSON_GetArrayItem(translation, i)->valuedouble;
    }
    
    cJSON* rotation = cJSON_GetObjectItemCaseSensitive(data, "rotation");
    for (u32 i = 0; i < 3; i++) {
        component->rotation[i] = (float)cJSON_GetArrayItem(rotation, i)->valuedouble;
    }

    cJSON* scale = cJSON_GetObjectItemCaseSensitive(data, "scale");
    for (u32 i = 0; i < 3; i++) {
        component->scale[i] = (float)cJSON_GetArrayItem(scale, i)->valuedouble;
    }

    ecs_component_transform_update(component, component->translation, component->rotation, component->scale);
}

