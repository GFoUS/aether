#pragma once

#include "vk_mem_alloc.h"

#include "core/core.h"
#include "graphics/window.h"

#include "instance.h"
#include "physical.h"
#include "device.h"
#include "swapchain.h"
#include "command.h"

typedef struct _vulkan_context {
	vulkan_instance*        instance;
	vulkan_physical_device* physical;
	vulkan_device*          device;
	VmaAllocator            allocator;

	VkSurfaceKHR surface;
	vulkan_command_pool* commandPool;

	window_window* window;
} vulkan_context;

vulkan_context* vulkan_context_create(window_window* window);
void vulkan_context_destroy(vulkan_context* ctx);

typedef struct {
	u32 numWaitSemaphores;
	VkSemaphore* waitSemaphores;
	VkPipelineStageFlags* waitStages;
	u32 numSignalSemaphores;
	VkSemaphore* signalSemaphores;
	VkFence fence;
} vulkan_context_submit_config;

VkCommandBuffer vulkan_context_start_recording(vulkan_context* ctx);
void vulkan_context_submit(vulkan_context* ctx, VkCommandBuffer cmd);

VkSemaphore vulkan_context_get_semaphore(vulkan_context* ctx, VkSemaphoreCreateFlags flags);
VkFence vulkan_context_get_fence(vulkan_context* ctx, VkFenceCreateFlags flags);