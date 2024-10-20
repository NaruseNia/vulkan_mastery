#ifndef VULKAN_MASTERY_GFX_DEVICE_H
#define VULKAN_MASTERY_GFX_DEVICE_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>

class GfxDevice {
    VkInstance vk_instance;

public:
    void init_vk_instance();
    void initialize();
};


#endif //VULKAN_MASTERY_GFX_DEVICE_H
