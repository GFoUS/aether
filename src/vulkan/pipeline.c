#include "pipeline.h"

#include "vertex.h"

typedef struct {
    vulkan_vertex_info vertexInfo;
    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
} vulkan_pipeline_vertex_info;

vulkan_pipeline_vertex_info* get_vertex_input(vulkan_pipeline_config* config) {
    vulkan_pipeline_vertex_info* vertexInfo = malloc(sizeof(vulkan_pipeline_vertex_info));
    CLEAR_MEMORY(vertexInfo);

    if (config->verticesFromBuffer) {
        vertexInfo->vertexInfo = vulkan_vertex_get_info();
    }

    vertexInfo->vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInfo->vertexInputInfo.vertexBindingDescriptionCount = vertexInfo->vertexInfo.numAttributes;
    vertexInfo->vertexInputInfo.pVertexBindingDescriptions = vertexInfo->vertexInfo.bindings;
    vertexInfo->vertexInputInfo.vertexAttributeDescriptionCount = vertexInfo->vertexInfo.numAttributes;
    vertexInfo->vertexInputInfo.pVertexAttributeDescriptions = vertexInfo->vertexInfo.attributes;

    return vertexInfo;
}

VkPipelineInputAssemblyStateCreateInfo get_input_assembly(vulkan_pipeline_config* config) {
    VkPipelineInputAssemblyStateCreateInfo inputAssembly;
    CLEAR_MEMORY(&inputAssembly);

    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    return inputAssembly;
}

typedef struct {
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineViewportStateCreateInfo state;
} vulkan_pipeline_viewport_info;

vulkan_pipeline_viewport_info* get_viewport_info(vulkan_pipeline_config* config) {
    vulkan_pipeline_viewport_info* viewportInfo = malloc(sizeof(vulkan_pipeline_viewport_info));
    CLEAR_MEMORY(viewportInfo);

    viewportInfo->viewport.x = 0.0f;
    viewportInfo->viewport.y = 0.0f;
    viewportInfo->viewport.width = (float)config->width;
    viewportInfo->viewport.height = (float)config->height;
    viewportInfo->viewport.minDepth = 0.0f;
    viewportInfo->viewport.maxDepth = 1.0f;

    viewportInfo->scissor.offset.x = 0;
    viewportInfo->scissor.offset.y = 0;
    viewportInfo->scissor.extent.width = config->width;
    viewportInfo->scissor.extent.height = config->height;

    viewportInfo->state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo->state.viewportCount = 1;
    viewportInfo->state.pViewports = &viewportInfo->viewport;
    viewportInfo->state.scissorCount = 1;
    viewportInfo->state.pScissors = &viewportInfo->scissor;

    return viewportInfo;
}

VkPipelineRasterizationStateCreateInfo get_rasterization(vulkan_pipeline_config* config) {
    VkPipelineRasterizationStateCreateInfo rasterizer;
    CLEAR_MEMORY(&rasterizer);

    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.cullMode = config->rasterizerCullMode;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.lineWidth = 1.0f;

    return rasterizer;
}

VkPipelineMultisampleStateCreateInfo get_multisampling(vulkan_pipeline_config* config) {
    VkSubpassDescription* subpass = &config->renderpass->subpasses[config->subpass];
    VkSampleCountFlagBits samples = config->samples;
    
    VkPipelineMultisampleStateCreateInfo multisampling;
    CLEAR_MEMORY(&multisampling);

    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE;
    multisampling.rasterizationSamples = samples;

    return multisampling;
}

VkPipelineDepthStencilStateCreateInfo get_depth_stencil(vulkan_pipeline_config* config) {
    VkPipelineDepthStencilStateCreateInfo depthStencil;
    CLEAR_MEMORY(&depthStencil);

    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    return depthStencil;
}

VkPipelineColorBlendStateCreateInfo get_blending(vulkan_pipeline_config* config) {
    VkPipelineColorBlendStateCreateInfo blending;
    CLEAR_MEMORY(&blending);

    blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    blending.logicOpEnable = VK_FALSE;
    blending.attachmentCount = config->numBlendingAttachments;
    blending.pAttachments = config->blendingAttachments;

    return blending;
}

vulkan_pipeline_layout* vulkan_pipeline_layout_create(vulkan_device* device, vulkan_pipeline_layout_config* config) {
    VkPipelineLayoutCreateInfo createInfo;
    CLEAR_MEMORY(&createInfo);

    VkDescriptorSetLayout* setLayouts = malloc(sizeof(VkDescriptorSetLayout) * config->numSetLayouts);
    CLEAR_MEMORY_ARRAY(setLayouts, config->numSetLayouts);
    for (u32 i = 0; i < config->numSetLayouts; i++) {
        setLayouts[i] = config->setLayouts[i]->layout;
    }

    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.setLayoutCount = config->numSetLayouts;
    createInfo.pSetLayouts = setLayouts;
    
    vulkan_pipeline_layout* layout = malloc(sizeof(vulkan_pipeline_layout));
    CLEAR_MEMORY(layout);
    layout->device = device;

    VkResult result = vkCreatePipelineLayout(device->device, &createInfo, NULL, &layout->layout);
    if (result != VK_SUCCESS) {
        FATAL("Vulkan pipeline layout creation failed with error code: %d", result);
    }

    return layout;
}

VkPipelineDynamicStateCreateInfo get_dynamic_state(vulkan_pipeline_config* config) {
    VkPipelineDynamicStateCreateInfo dynamicState;
    CLEAR_MEMORY(&dynamicState);

    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = config->numDynamicStates;
    dynamicState.pDynamicStates = config->dynamicStates;

    return dynamicState;
}

void vulkan_pipeline_layout_destroy(vulkan_pipeline_layout* layout) {
    vkDestroyPipelineLayout(layout->device->device, layout->layout, NULL);
    free(layout);
}

vulkan_pipeline* vulkan_pipeline_create(vulkan_device* device, vulkan_pipeline_config* config) {
    VkPipelineShaderStageCreateInfo shaderStages[2];
    shaderStages[0] = vulkan_shader_get_stage_info(config->vertexShader);
    shaderStages[1] = vulkan_shader_get_stage_info(config->fragmentShader);
    vulkan_pipeline_vertex_info* vertexInfo = get_vertex_input(config);
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = get_input_assembly(config);
    vulkan_pipeline_viewport_info* viewportInfo = get_viewport_info(config);
    VkPipelineRasterizationStateCreateInfo rasterizer = get_rasterization(config);
    VkPipelineMultisampleStateCreateInfo multisampling = get_multisampling(config);
    VkPipelineDepthStencilStateCreateInfo depthStencil = get_depth_stencil(config);
    VkPipelineColorBlendStateCreateInfo blending = get_blending(config);
    VkPipelineDynamicStateCreateInfo dynamicState = get_dynamic_state(config);
    VkSubpassDescription subpass = config->renderpass->subpasses[config->subpass];

    vulkan_pipeline_layout_config layoutConfig;
    CLEAR_MEMORY(&layoutConfig);
    layoutConfig.numSetLayouts = config->numSetLayouts;
    layoutConfig.setLayouts = config->setLayouts;
    vulkan_pipeline_layout* layout = vulkan_pipeline_layout_create(device, &layoutConfig);
    
    VkGraphicsPipelineCreateInfo  createInfo;
    CLEAR_MEMORY(&createInfo);

    createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    createInfo.stageCount = 2;
    createInfo.pStages = shaderStages;
    createInfo.pVertexInputState = &vertexInfo->vertexInputInfo;
    createInfo.pInputAssemblyState = &inputAssembly;
    createInfo.pViewportState = &viewportInfo->state;
    createInfo.pRasterizationState = &rasterizer;
    createInfo.pMultisampleState = &multisampling;
    createInfo.pDepthStencilState = subpass.pDepthStencilAttachment != NULL ? &depthStencil : NULL;
    createInfo.pColorBlendState = &blending;
    createInfo.pDynamicState = &dynamicState;
    createInfo.layout = layout->layout;
    createInfo.renderPass = config->renderpass->renderpass;
    createInfo.subpass = config->subpass;

    vulkan_pipeline* pipeline = malloc(sizeof(vulkan_pipeline));
    pipeline->device = device;
    pipeline->layout = layout;
    VkResult result = vkCreateGraphicsPipelines(device->device, VK_NULL_HANDLE, 1, &createInfo, NULL, &pipeline->pipeline);
    if (result != VK_SUCCESS) {
        FATAL("Vulkan pipeline creation failed with error code: %d", result);
    }

    free(vertexInfo);
    free(viewportInfo);

    return pipeline;
}

void vulkan_pipeline_destroy(vulkan_pipeline* pipeline) {
    vulkan_pipeline_layout_destroy(pipeline->layout);
    vkDestroyPipeline(pipeline->device->device, pipeline->pipeline, NULL);
    free(pipeline);
}

VkPipelineColorBlendAttachmentState vulkan_get_default_blending() {
    VkPipelineColorBlendAttachmentState blending;
    CLEAR_MEMORY(&blending);
    blending.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    blending.blendEnable = VK_FALSE;
    return blending;
}
