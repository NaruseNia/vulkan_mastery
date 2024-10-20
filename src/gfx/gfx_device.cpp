#include "gfx_device.h"

void GfxDevice::initialize() {
    std::cout << "INFO: " << "Vulkan Initializing..." << std::endl;
    init_vk_instance();
}

void GfxDevice::init_vk_instance() {
    std::cout << "INFO: " << "Vulkan creating instance..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    const char* app_name = nullptr;
    VkApplicationInfo app_info {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = app_name,
        .pEngineName = app_name,
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_3,
    };
    VkInstanceCreateInfo create_info{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
    };

    std::vector<const char*> layers;
    std::vector<const char*> extensions;

    uint32_t glfw_req_count = 0;
    const char** glfw_ext_names = glfwGetRequiredInstanceExtensions(&glfw_req_count);
    std::for_each_n(glfw_ext_names, glfw_req_count, [&](auto val) { extensions.push_back(val); });

#if defined(_DEBUG) || defined(DEBUG)
layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

    create_info.enabledExtensionCount = uint32_t(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();
    create_info.enabledLayerCount = uint32_t(layers.size());
    create_info.ppEnabledLayerNames = layers.data();

    vkCreateInstance(&create_info, nullptr, &vk_instance);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "INFO: " << "Vulkan instance created. done in " << duration << "ms" << std::endl;
}
