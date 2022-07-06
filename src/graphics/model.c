#include "model.h"

char* merge_paths(const char* path, const char* uri) {
    size_t uriLength = strlen(uri);
    char* lastSlash = strrchr(path, (int)"/"[0]);
    u64 lastSlashPos = lastSlash - path;

    char* result = malloc(lastSlashPos + 1 + uriLength + 1); // lastSlashPos + 1 is the path up to and including the last slash, uriLength will be needed for the new path, and the final + 1 is for the null-termination character 
    CLEAR_MEMORY_ARRAY(result, lastSlashPos + 1 + uriLength + 1);
    memcpy(result, path, lastSlashPos + 1);
    memcpy(&result[lastSlashPos + 1], uri, uriLength);

    return result;
}

VkFilter gltf_filter_to_vk_filter(cgltf_int filter) {
    switch (filter) {
        case(9728) : return VK_FILTER_LINEAR;
        case(9729) : return VK_FILTER_NEAREST;
    }

    return VK_FILTER_LINEAR;
}

VkSamplerAddressMode gltf_wrap_mode_to_vk_address_mode(cgltf_int wrapMode) {
    switch (wrapMode) {
        case(33071) : return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case(33648) : return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case(10497) : return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    }

    return VK_SAMPLER_ADDRESS_MODE_REPEAT;
}

typedef struct {
    vec4 baseColorFactor; // 16
    vec4 padding1;        // 16 + 16 = 32
    vec4 padding2;        // 16 + 32 = 48
    vec4 padding3;        // 16 + 48 = 64
    mat4 padding4;        // 64 + 64 = 128
    mat4 padding5;        // 64 + 128 = 192
    mat4 padding6;        // 64 + 192 = 256
} model_material_data;

model_model* model_load(const char* path, vulkan_context* ctx, vulkan_descriptor_set_layout* materialSetLayout) {
    model_model* model = malloc(sizeof(model_model));
    CLEAR_MEMORY(model);
    model->materialSetLayout = materialSetLayout;
    model->materialSetAllocator = vulkan_descriptor_allocator_create(ctx->device, model->materialSetLayout);

    cgltf_options options;
    CLEAR_MEMORY(&options);
    cgltf_result parseResult = cgltf_parse_file(&options, path, &model->data);
    if (parseResult != cgltf_result_success) {
        FATAL("GLTF parsing failed for file: %s with error code: %d", path, parseResult);
    }

    cgltf_result loadResult = cgltf_load_buffers(&options, model->data, path);
    if (loadResult != cgltf_result_success) {
        FATAL("GLTF buffer loading failed for file: %s with error code: %d", path, loadResult);
    }

    cgltf_result validateResult = cgltf_validate(model->data);
    if (validateResult != cgltf_result_success) {
        FATAL("GLTF validation failed for file: %s with error code: %d", path, validateResult);
    }

    model->buffers = malloc(sizeof(vulkan_buffer*) * model->data->buffers_count);
    for (u32 i = 0; i < model->data->buffers_count; i++) {
        model->buffers[i] = vulkan_buffer_create_with_data(ctx, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, model->data->buffers[i].size, model->data->buffers[i].data);
    }

    model->images = malloc(sizeof(vulkan_image*) * model->data->images_count);
    for (u32 i = 0; i < model->data->images_count; i++) {
        char* imagePath = merge_paths(path, model->data->images[i].uri);
        model->images[i] = vulkan_image_create_from_file(ctx, imagePath, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    model->samplers = malloc(sizeof(vulkan_sampler*) * model->data->samplers_count);
    for (u32 i = 0; i < model->data->samplers_count; i++) {
        cgltf_sampler* sampler = &model->data->samplers[i];
        model->samplers[i] = vulkan_sampler_create(ctx, gltf_filter_to_vk_filter(sampler->mag_filter), gltf_filter_to_vk_filter(sampler->min_filter), gltf_wrap_mode_to_vk_address_mode(sampler->wrap_s), gltf_wrap_mode_to_vk_address_mode(sampler->wrap_t));
    }

    model->materialSets = malloc(sizeof(vulkan_descriptor_set*) * model->data->materials_count);
    model->materialDataBuffer = vulkan_buffer_create(ctx, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, sizeof(model_material_data) * model->data->materials_count);
    model_material_data* materialData = malloc(sizeof(model_material_data) * model->data->materials_count);
    for (u32 i = 0; i < model->data->materials_count; i++) {
        cgltf_material* material = &model->data->materials[i];
        model->materialSets[i] = vulkan_descriptor_set_allocate(model->materialSetAllocator);
        
        glm_vec4_copy(material->pbr_metallic_roughness.base_color_factor, materialData[i].baseColorFactor);
        
        vulkan_descriptor_set_write_buffer(model->materialSets[i], 0, model->materialDataBuffer);
        size_t baseColorTextureImageIndex = material->pbr_metallic_roughness.base_color_texture.texture->image - model->data->images;
        size_t baseColorTextureSamplerIndex = material->pbr_metallic_roughness.base_color_texture.texture->sampler - model->data->samplers;
        vulkan_descriptor_set_write_image(model->materialSets[i], 1, model->images[baseColorTextureImageIndex], model->samplers[baseColorTextureSamplerIndex]);
    }
    vulkan_buffer_update(model->materialDataBuffer, sizeof(model_material_data) * model->data->materials_count, (void*)materialData);
    free(materialData);

    return model;
}

void model_unload(model_model* model) {
    for (u32 i = 0; i < model->data->buffers_count; i++) {
        vulkan_buffer_destroy(model->buffers[i]);
    }
    free(model->buffers);

    for (u32 i = 0; i < model->data->images_count; i++) {
        vulkan_image_destroy(model->images[i]);
    }
    free(model->images);

    for (u32 i = 0; i < model->data->samplers_count; i++) {
        vulkan_sampler_destroy(model->samplers[i]);
    }
    free(model->samplers);

    vulkan_buffer_destroy(model->materialDataBuffer);
    vulkan_descriptor_allocator_destroy(model->materialSetAllocator);

    cgltf_free(model->data);
    free(model);
}

void render_node(model_model* model, cgltf_node* node, VkCommandBuffer cmd, VkPipelineLayout pipelineLayout) {
    // TODO: Model matricies

    if (node->mesh != NULL) {
        for (u32 i = 0; i < node->mesh->primitives_count; i++) {
            cgltf_primitive* primitive = &node->mesh->primitives[i];
            
            size_t materialIndex = primitive->material - model->data->materials;
            u32 offset = (u32)(sizeof(model_material_data) * materialIndex);
            vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 1, 1, &model->materialSets[materialIndex]->set, 1, &offset);

            VkIndexType indexType = primitive->indices->component_type == cgltf_component_type_r_32u ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16;
            size_t indexBufferIndex = (primitive->indices->buffer_view->buffer - model->data->buffers) / sizeof(cgltf_buffer*);
            vkCmdBindIndexBuffer(cmd, model->buffers[indexBufferIndex]->buffer, primitive->indices->offset + primitive->indices->buffer_view->offset, indexType);

            for (u32 j = 0; j < primitive->attributes_count; j++) {
                cgltf_attribute* attribute = &primitive->attributes[j];

                u32 attributeIndex = UINT32_MAX;
                switch (attribute->type) {
                case (cgltf_attribute_type_position) : attributeIndex = 0; break;
                case (cgltf_attribute_type_normal)   : attributeIndex = 1; break;
                case (cgltf_attribute_type_texcoord) : attributeIndex = 2; break;
                }

                if (attributeIndex != UINT32_MAX) {
                    size_t attributeBufferIndex = (attribute->data->buffer_view->buffer - model->data->buffers) / sizeof(cgltf_buffer*);
                    VkDeviceSize attributeOffset = attribute->data->offset + attribute->data->buffer_view->offset;
                    vkCmdBindVertexBuffers(cmd, attributeIndex, 1, &model->buffers[attributeBufferIndex]->buffer, &attributeOffset);
                }
            }

            vkCmdDrawIndexed(cmd, (u32)primitive->indices->count, 1, 0, 0, 0);
        }
    }

    for (u32 i = 0; i < node->children_count; i++) {
        render_node(model, node->children[i], cmd, pipelineLayout);
    }
}

void model_render(model_model* model, VkCommandBuffer cmd, VkPipelineLayout pipelineLayout) {
    for (u32 i = 0; i < model->data->scene->nodes_count; i++) {
        render_node(model, model->data->scene->nodes[i], cmd, pipelineLayout);
    }
}
