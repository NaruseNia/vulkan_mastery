#ifndef VULKAN_MASTERY_GFX_DEVICE_H
#define VULKAN_MASTERY_GFX_DEVICE_H

#include <algorithm>
#include <vector>
#include <chrono>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "common.h"
#include "../logging/logger.h"

class GfxDevice {
    VkInstance vk_instance;
    VkPhysicalDevice vk_physical_device;
    VkPhysicalDeviceMemoryProperties vk_memory_properties;
    VkPhysicalDeviceProperties vk_device_properties;
    uint32 graphics_queue_family_index;

public:
    void init_vk_instance();
    void initialize();
    void init_physical_device();
};


#endif //VULKAN_MASTERY_GFX_DEVICE_H
