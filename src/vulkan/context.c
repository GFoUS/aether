#include "context.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "image.h"

vulkan_context* vulkan_context_create(window_window* window)
{
	vulkan_context* ctx = malloc(sizeof(vulkan_context));
	ctx->window = window;

	u32 numGlfwExtensions;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&numGlfwExtensions);
	u32 numWantedExtensions = 1;
	const char* wantedExtensions[] = { VK_KHR_SURFACE_EXTENSION_NAME };

	u32 numExtensions = numGlfwExtensions + numWantedExtensions;
	char** extensions = malloc(sizeof(const char*) * numExtensions);
	memcpy(extensions, glfwExtensions, sizeof(const char*) * numGlfwExtensions);
	memcpy(&extensions[numGlfwExtensions], wantedExtensions, sizeof(const char*) * numWantedExtensions);

	const char* layers[] = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
	ctx->instance = vulkan_instance_create(numExtensions, extensions, 0, NULL);
#else
	ctx->instance = vulkan_instance_create(numExtensions, extensions, 1, layers);
#endif
	INFO("Created vulkan instance");
	free(extensions);

	VkResult surfaceResult = glfwCreateWindowSurface(ctx->instance->instance, ctx->window->window, NULL, &ctx->surface);
	if (surfaceResult != VK_SUCCESS) {
		FATAL("Failed to create vulkan surface with error code %d", surfaceResult);
	}
	INFO("Created vulkan surface");

	const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	ctx->physical = vulkan_physical_device_create(ctx->instance, ctx->surface, 1, deviceExtensions);
	INFO("Using GPU: %s", ctx->physical->properties.deviceName);

	ctx->device = vulkan_device_create(ctx->instance, ctx->physical, 1, deviceExtensions, 1, layers);
	INFO("Created vulkan device");

	VmaAllocatorCreateInfo allocatorCreateInfo;
	CLEAR_MEMORY(&allocatorCreateInfo);
	allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
	allocatorCreateInfo.physicalDevice = ctx->physical->physical;
	allocatorCreateInfo.device = ctx->device->device;
	allocatorCreateInfo.instance = ctx->instance->instance;

	VkResult allocatorResult = vmaCreateAllocator(&allocatorCreateInfo, &ctx->allocator);
	if (allocatorResult != VK_SUCCESS) {
		FATAL("Vulkan memory allocator creation failed with error code: %d", allocatorResult);
	}
	INFO("Created VMA allocator");

	ctx->commandPool = vulkan_command_pool_create(ctx->device, ctx->physical->queues.graphicsIndex);
	INFO("Created command pool");

	return ctx;
}

void vulkan_context_destroy(vulkan_context* ctx)
{
	vulkan_sampler_destroy_default(ctx);

	vulkan_command_pool_destroy(ctx->commandPool);
	vmaDestroyAllocator(ctx->allocator);
	vulkan_device_destroy(ctx->device);
	vulkan_physical_device_destroy(ctx->physical);
	vkDestroySurfaceKHR(ctx->instance->instance, ctx->surface, NULL);
	vulkan_instance_destroy(ctx->instance);

	free(ctx);
}

VkCommandBuffer vulkan_context_start_recording(vulkan_context* ctx) {
	VkCommandBuffer cmd = vulkan_command_pool_get_buffer(ctx->commandPool);
	
	VkCommandBufferBeginInfo beginInfo;
	CLEAR_MEMORY(&beginInfo);

	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	VkResult beginResult = vkBeginCommandBuffer(cmd, &beginInfo);
	if (beginResult != VK_SUCCESS) {
		FATAL("Vulkan command buffer begin failed with error code: %d", beginResult);
	}

	return cmd;
}

void vulkan_context_submit(vulkan_context* ctx, VkCommandBuffer cmd) {
	VkResult endResult = vkEndCommandBuffer(cmd);
	if (endResult != VK_SUCCESS) {
		FATAL("Vulkan command buffer end failed with error code: %d", endResult);
	}

	VkSubmitInfo submitInfo;
	CLEAR_MEMORY(&submitInfo);

	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmd;

	VkResult submitResult = vkQueueSubmit(ctx->device->graphics, 1, &submitInfo, NULL);
	if (submitResult != VK_SUCCESS) {
		FATAL("Vulkan command buffer submissions failed with error code: %d", submitResult);
	}

	vkQueueWaitIdle(ctx->device->graphics);
	vulkan_command_pool_free_buffer(ctx->commandPool, cmd);
}

VkSemaphore vulkan_context_get_semaphore(vulkan_context* ctx, VkSemaphoreCreateFlags flags) {
	VkSemaphoreCreateInfo createInfo;
	CLEAR_MEMORY(&createInfo);
	createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	createInfo.flags = flags;

	VkSemaphore semaphore;
	VkResult result = vkCreateSemaphore(ctx->device->device, &createInfo, NULL, &semaphore);
	if (result != VK_SUCCESS) {
		FATAL("Semaphore createion failed with error code: %d", result);
	}
	
	return semaphore;
}

VkFence vulkan_context_get_fence(vulkan_context* ctx, VkFenceCreateFlags flags) {
	VkFenceCreateInfo createInfo;
	CLEAR_MEMORY(&createInfo);
	createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	createInfo.flags = flags;

	VkFence fence;
	VkResult result = vkCreateFence(ctx->device->device, &createInfo, NULL, &fence);
	if (result != VK_SUCCESS) {
		FATAL("Fence createion failed with error code: %d", result);
	}
	
	return fence;
}