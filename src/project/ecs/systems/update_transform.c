#include "update_transform.h"

#include "project/ecs/ecs.h"

void ecs_system_update_transform_call(ecs_entity* entity, void* systemPtr) {
    ecs_component_model* model = (ecs_component_model*)ecs_entity_get_component(entity, COMPONENT_TYPE_MODEL);
    ecs_component_transform* transform = (ecs_component_transform*)ecs_entity_get_component(entity, COMPONENT_TYPE_TRANSFORM);

    if (model->asset->numTimesLoaded != 0) {
        model_set_global_transform(((project_asset_model*)model->asset->loadedData)->model, transform->matrix);
    }
}

void ecs_system_update_transform_destroy(void* systemPtr) {
    ecs_system_update_transform* system = (ecs_system_update_transform*)systemPtr;
    free(system);
}

static ecs_component_type REQUIREMENTS[] = {COMPONENT_TYPE_TRANSFORM, COMPONENT_TYPE_MODEL};

ecs_system_update_transform* ecs_system_update_transform_create() {
    ecs_system_update_transform* system = malloc(sizeof(ecs_system_update_transform));
    CLEAR_MEMORY(system);

    system->system = ecs_system_create(ARRAY_SIZE(REQUIREMENTS), REQUIREMENTS, ecs_system_update_transform_call, ecs_system_update_transform_destroy, system);

    return system;
}
