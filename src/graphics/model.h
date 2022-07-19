#pragma once

#include "cglm/cglm.h"
#include "cgltf.h"
#include "vulkan/buffer.h"
#include "vulkan/image.h"
#include "vulkan/descriptor.h"

typedef struct {
    cgltf_data* data;
    vulkan_context* ctx;

    vulkan_descriptor_set_layout* materialSetLayout;
    vulkan_descriptor_allocator* materialSetAllocator;
    vulkan_descriptor_set** materialSets;
    vulkan_buffer* materialDataBuffer;

    vulkan_buffer* modelMatrixBuffer;
    vulkan_descriptor_set_layout* modelSetLayout;
    vulkan_descriptor_allocator* modelSetAllocator;
    vulkan_descriptor_set* modelSet;

    vulkan_buffer** buffers;
    vulkan_image** images;
    vulkan_sampler** samplers;

    mat4 globalTransform;
} model_model;

model_model* model_load(const char* path, vulkan_context* ctx, vulkan_descriptor_set_layout* materialSetLayout, vulkan_descriptor_set_layout* modelSetLayout);
void model_unload(model_model* model);

void model_get_node_matrix(model_model* model, mat4 result);

void model_render(model_model* model, VkCommandBuffer cmd, VkPipelineLayout pipelineLayout);
void model_set_global_transform(model_model* model, mat4 globalTransform);