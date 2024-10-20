#include "gfx_device.h"

void GfxDevice::initialize() {
    std::cout << "INFO: " << "Vulkan Initializing..." << std::endl;
    volkInitialize();               // Initialize Volk
    init_vk_instance();             // Initialize VkInstance

    init_physical_device();         // Initialize VkPhysicalDevice(GPU)
    init_vk_device();               // Initialize VkDevice(Logical Device)
}

void GfxDevice::init_vk_instance() {
    std::cout << "INFO: " << "Creating Vulkan instance..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    const char *app_name = nullptr;
    VkApplicationInfo app_info{
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

    std::vector<const char *> layers;
    std::vector<const char *> extensions;

    uint32 glfw_req_count = 0;
    const char **glfw_ext_names = glfwGetRequiredInstanceExtensions(&glfw_req_count);
    std::for_each_n(glfw_ext_names, glfw_req_count, [&](auto val) { extensions.push_back(val); });

#if defined(_DEBUG) || defined(DEBUG)
    layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

    create_info.enabledExtensionCount = uint32(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();
    create_info.enabledLayerCount = uint32(layers.size());
    create_info.ppEnabledLayerNames = layers.data();

    vkCreateInstance(&create_info, nullptr, &vk_instance);
    volkLoadInstance(vk_instance);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "INFO: " << "Vulkan instance created. done in " << duration << "ms" << std::endl;
}

void GfxDevice::init_physical_device() {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "INFO: " << "Initializing physical device..." << std::endl;
    uint32 count = 0;
    vkEnumeratePhysicalDevices(vk_instance, &count, nullptr);
    std::vector<VkPhysicalDevice> devices(count);
    vkEnumeratePhysicalDevices(vk_instance, &count, devices.data());

    std::cout << "INFO: " << "Physical devices found: " << count << std::endl;
    vk_physical_device = devices[0];
    vkGetPhysicalDeviceMemoryProperties(vk_physical_device, &vk_memory_properties);
    vkGetPhysicalDeviceProperties(vk_physical_device, &vk_device_properties);

    std::cout << "INFO: " << "Using first device: " << std::endl;
    std::cout << "INFO: " << "  Device name: " << vk_device_properties.deviceName << std::endl;
    std::cout << "INFO: " << "  Device ID: " << vk_device_properties.deviceID << std::endl;
    std::cout << "INFO: " << "  Vendor ID: " << vk_device_properties.vendorID << std::endl;
    std::cout << "INFO: " << "  API Version: " << vk_device_properties.apiVersion << std::endl;
    std::cout << "INFO: " << "  Driver Version: " << vk_device_properties.driverVersion << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "INFO: " << "Physical device initialized. done in " << duration << "ms" << std::endl;

    vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(count);
    vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device, &count, queue_families.data());

    uint32 gfx_queue_family_index = 0;
    for (uint32 i = 0; i < count; i++) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            gfx_queue_family_index = i;
            break;
        }
    }
    assert(gfx_queue_family_index != ~0u);
    graphics_queue_family_index = gfx_queue_family_index;
    std::cout << "INFO: " << "Graphics queue family index: " << gfx_queue_family_index << std::endl;
}

void GfxDevice::init_vk_device() {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<const char *> extensions = {
            // FIXME: For some reason, Cannot use VK_KHR_swapchain.
            // VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            // For environment that cannot use Vulkan 1.3
            VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
    };

    VkPhysicalDeviceFeatures2 phys_features2{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2
    };
    VkPhysicalDeviceVulkan11Features vk11_features{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES
    };
    VkPhysicalDeviceVulkan12Features vk12_features{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES
    };
    VkPhysicalDeviceVulkan13Features vk13_features{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES
    };

    phys_features2.pNext = &vk11_features;
    vk11_features.pNext = &vk12_features;
    vk12_features.pNext = &vk13_features;
    vkGetPhysicalDeviceFeatures2(vk_physical_device, &phys_features2);

    // Set features explicitly.
    vk13_features.dynamicRendering = VK_TRUE;
    vk13_features.synchronization2 = VK_TRUE;
    vk13_features.maintenance4 = VK_TRUE;

    // Generate VkDevice
    const float queue_priority[] = {1.0f};
    VkDeviceQueueCreateInfo queue_info{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = graphics_queue_family_index,
            .queueCount = 1,
            .pQueuePriorities = queue_priority,
    };
    VkDeviceCreateInfo device_info{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queue_info,
            .enabledExtensionCount = uint32(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
    };
    device_info.pNext = &phys_features2;
    vkCreateDevice(vk_physical_device, &device_info, nullptr, &vk_device);

    volkLoadDevice(vk_device);
    vkGetDeviceQueue(vk_device, graphics_queue_family_index, 0, &vk_graphics_queue);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "INFO: " << "Vulkan device initialized. done in " << duration << "ms" << std::endl;
}