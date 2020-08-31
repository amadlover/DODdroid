//
// Created by nihal on 15-08-2020.
//

#include "vulkan_interface.hpp"

#include "error.hpp"
#include "utils.hpp"

#include <android/log.h>
#include <vector>
#include <android_native_app_glue.h>


bool is_validation_needed = false;
bool is_debug_utils_ext_avaiable = false;
VkDebugUtilsMessengerEXT debug_utils_messenger = VK_NULL_HANDLE;
VkSurfaceKHR surface = VK_NULL_HANDLE;
VkSurfaceCapabilitiesKHR surface_capabilities = { 0 };
VkPresentModeKHR chosen_present_mode;

VkInstance instance;
VkPhysicalDevice physical_device;
VkDevice device;
uint32_t graphics_queue_family_index;
uint32_t compute_queue_family_index;
uint32_t transfer_queue_family_index;
VkQueue graphics_queue;
VkQueue compute_queue;
VkQueue transfer_queue;
VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
VkPhysicalDeviceLimits physical_device_limits;
VkExtent2D surface_extent;
VkSwapchainKHR swapchain;
std::vector<VkImage> swapchain_images;
std::vector<VkImageView> swapchain_image_views;
uint32_t swapchain_image_count;
VkExtent2D current_extent;
VkSurfaceFormatKHR chosen_surface_format;
VkCommandPool graphics_command_pool;
VkSampler common_sampler;

static const char* TAG = "Asteroids";

VKAPI_ATTR VkBool32 VKAPI_CALL debug_utils_messenger_callback (
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_types,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void* pUserData) {
    if (callback_data) {
        printf("Debug Callback Message: %s\n\n", callback_data->pMessage);
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "Debug Callback Message: %s\n\n",
                            callback_data->pMessage);
    }

    return false;
}

AGE_RESULT create_debug_utils_messenger () {
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT> (vkGetDeviceProcAddr(
            device, "vkCreateDebugUtilsMessengerEXT"));

    VkDebugUtilsMessengerCreateInfoEXT create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.pNext = nullptr;
    create_info.flags = 0;
    create_info.messageSeverity = /*VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |*/
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;

    create_info.pfnUserCallback = debug_utils_messenger_callback;

    assert (func);
    VkResult vk_result = func(instance, &create_info, nullptr, &debug_utils_messenger);

    if (vk_result != VK_SUCCESS) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "%d", vk_result);
        return AGE_RESULT::ERROR_GRAPHICS_CREATE_DEBUG_UTILS_MESSENGER;
    }

    return AGE_RESULT::SUCCESS;
}

void destroy_debug_utils_messenger () {
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT> (vkGetDeviceProcAddr(
            device, "vkDestroyDebugUtilsMessengerEXT"));

    assert (func);

    func (instance, debug_utils_messenger, nullptr);
}

AGE_RESULT create_instance () {
    std::vector<const char *> instance_extensions;
    instance_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    instance_extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);

    std::vector<const char *> instance_layers;

    if (is_validation_needed) {

        uint32_t available_extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &available_extension_count, nullptr);

        auto available_extensions = (VkExtensionProperties *) utils_malloc(
                sizeof(VkExtensionProperties) * available_extension_count);
        vkEnumerateInstanceExtensionProperties(nullptr, &available_extension_count,
                                               available_extensions);

        for (size_t e = 0; e < available_extension_count; ++e) {
            if (strcmp(available_extensions[e].extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) ==
                0) {
                is_debug_utils_ext_avaiable = true;
                instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }
        }

        if (!is_debug_utils_ext_avaiable) {
            __android_log_write(ANDROID_LOG_VERBOSE, TAG, "Debug utils extension not available");
        }

        utils_free (available_extensions);
        instance_layers.push_back("VK_LAYER_KHRONOS_validation");
    }

    VkApplicationInfo application_info = {};
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pNext = nullptr;
    application_info.apiVersion = VK_API_VERSION_1_1;
    application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    application_info.pEngineName = "AGE";
    application_info.pApplicationName = "Asteroids";
    application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.flags = 0;
    create_info.pNext = nullptr;
    create_info.enabledExtensionCount = instance_extensions.size();
    create_info.ppEnabledExtensionNames = instance_extensions.data();
    create_info.pApplicationInfo = &application_info;

    if (is_validation_needed) {
        create_info.enabledLayerCount = instance_layers.size();
        create_info.ppEnabledLayerNames = instance_layers.data();
    }

    VkResult vk_result = vkCreateInstance(&create_info, nullptr, &instance);
    if (vk_result != VK_SUCCESS) {
        return AGE_RESULT::ERROR_GRAPHICS_CREATE_INSTANCE;
    }

    return AGE_RESULT::SUCCESS;
}

AGE_RESULT create_surface (ANativeWindow* p_window) {
    VkAndroidSurfaceCreateInfoKHR create_info = {};

    create_info.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    create_info.flags = 0;
    create_info.window = p_window;

    VkResult vk_result = vkCreateAndroidSurfaceKHR (instance, &create_info, nullptr, &surface);
    if (vk_result != VK_SUCCESS) {
        return AGE_RESULT::ERROR_GRAPHICS_CREATE_SURFACE;
    }

    return AGE_RESULT::SUCCESS;
}

AGE_RESULT get_physical_device () {
    uint32_t physical_device_count = 0;
    vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);

    if (physical_device_count == 0) {
        return AGE_RESULT::ERROR_GRAPHICS_GET_PHYSICAL_DEVICE;
    }

    auto physical_devices = (VkPhysicalDevice *) utils_malloc(
            sizeof(VkPhysicalDevice) * physical_device_count);
    vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices);

    physical_device = physical_devices[0];

    utils_free(physical_devices);

    return AGE_RESULT::SUCCESS;
}

AGE_RESULT check_physical_device_surface_support () {
    VkBool32 is_surface_supported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, graphics_queue_family_index, surface,
                                         &is_surface_supported);

    if (!is_surface_supported) {
        return AGE_RESULT::ERROR_GRAPHICS_SURFACE_SUPPORT;
    }

    return AGE_RESULT::SUCCESS;
}

AGE_RESULT get_physical_device_queue_families () {
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
    auto queue_family_properties = (VkQueueFamilyProperties *) utils_malloc(
            sizeof(VkQueueFamilyProperties) * queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                             queue_family_properties);

    for (size_t i = 0; i < queue_family_count; ++i) {
        if (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphics_queue_family_index = i;
            break;
        }
    }

    for (size_t i = 0; i < queue_family_count; ++i) {
        if (queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT &&
            (i != graphics_queue_family_index)) {
            compute_queue_family_index = i;
            break;
        }
    }

    if (compute_queue_family_index == -1) {
        for (size_t i = 0; i < queue_family_count; ++i) {
            if (queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
                compute_queue_family_index = i;
                break;
            }
        }
    }

    for (size_t i = 0; i < queue_family_count; ++i) {
        if (queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT &&
            (i != graphics_queue_family_index) && (i != compute_queue_family_index)) {
            transfer_queue_family_index = i;
            break;
        }
    }

    if (transfer_queue_family_index == -1) {
        for (size_t i = 0; i < queue_family_count; ++i) {
            if (queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
                transfer_queue_family_index = i;
                break;
            }
        }
    }

    utils_free(queue_family_properties);

    return AGE_RESULT::SUCCESS;
}

AGE_RESULT get_physical_device_properties () {
    vkGetPhysicalDeviceMemoryProperties(physical_device, &physical_device_memory_properties);

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(physical_device, &device_properties);
    physical_device_limits = device_properties.limits;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &surface_capabilities);

    current_extent = surface_capabilities.currentExtent;
    uint32_t surface_format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count, nullptr);

    auto surface_formats = (VkSurfaceFormatKHR *) utils_malloc(
            sizeof(VkSurfaceFormatKHR) * surface_format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count,
                                         surface_formats);

    chosen_surface_format = surface_formats[0];

    uint32_t present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count,
                                              nullptr);

    auto present_modes = (VkPresentModeKHR *) utils_malloc(
            sizeof(VkPresentModeKHR) * present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count,
                                              present_modes);

    for (size_t p = 0; p < present_mode_count; p++) {
        if (present_modes[p] == VK_PRESENT_MODE_MAILBOX_KHR) {
            chosen_present_mode = present_modes[p];
            break;
        }
    }

    utils_free(surface_formats);
    utils_free(present_modes);

    return AGE_RESULT::SUCCESS;
}

AGE_RESULT create_device () {
    std::vector<const char *> device_extensions;
    device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    float priorities[3] = {1.f, 1.f, 1.f};

    VkDeviceQueueCreateInfo queue_create_infos[3] = {};
    size_t unique_queue_family_indices[3] = {0, 0, 0};
    size_t unique_queue_count[3] = {1, 1, 1};
    size_t unique_queue_family_index_count = 0;

    if (graphics_queue_family_index == compute_queue_family_index) {
        unique_queue_family_indices[0] = graphics_queue_family_index;
        ++unique_queue_family_index_count;
        ++unique_queue_count[0];
    } else {
        unique_queue_family_indices[0] = graphics_queue_family_index;
        unique_queue_family_indices[1] = compute_queue_family_index;
        unique_queue_family_index_count += 2;
    }

    if (graphics_queue_family_index != transfer_queue_family_index) {
        unique_queue_family_indices[unique_queue_family_index_count] = transfer_queue_family_index;
        ++unique_queue_family_index_count;
    }

    for (size_t ui = 0; ui < unique_queue_family_index_count; ++ui) {
        queue_create_infos[ui].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[ui].pNext = nullptr;
        queue_create_infos[ui].flags = 0;
        queue_create_infos[ui].pQueuePriorities = priorities;
        queue_create_infos[ui].queueCount = unique_queue_count[ui];
        queue_create_infos[ui].queueFamilyIndex = unique_queue_family_indices[ui];
    }

    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(physical_device, &device_features);

    VkDeviceCreateInfo device_create_info = {
            VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            nullptr,
            0,
            (uint32_t)unique_queue_family_index_count,
            queue_create_infos,
            0,
            nullptr,
            (uint32_t)device_extensions.size(),
            device_extensions.data(),
            &device_features
    };

    VkResult vk_result = vkCreateDevice(physical_device, &device_create_info, nullptr, &device);

    if (vk_result != VK_SUCCESS) {
        return AGE_RESULT::ERROR_GRAPHICS_CREATE_GRAPHICS_DEVICE;
    }

    return AGE_RESULT::SUCCESS;
}

AGE_RESULT create_swapchain () {
    VkSwapchainCreateInfoKHR swapchain_create_info = {
            VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            nullptr,
            0,
            surface,
            std::max<uint32_t>(surface_capabilities.minImageCount, 3),
            chosen_surface_format.format,
            chosen_surface_format.colorSpace,
            surface_capabilities.currentExtent,
            1,
            surface_capabilities.supportedUsageFlags,
            VK_SHARING_MODE_EXCLUSIVE,
            0,
            nullptr,
            surface_capabilities.currentTransform,
            VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
            chosen_present_mode,
            1,
            VK_NULL_HANDLE
    };

    VkResult vk_result = vkCreateSwapchainKHR(device, &swapchain_create_info, nullptr, &swapchain);

    if (vk_result != VK_SUCCESS) {
        return AGE_RESULT::ERROR_GRAPHICS_CREATE_SWAPCHAIN;
    }

    return AGE_RESULT::SUCCESS;
}

AGE_RESULT create_swapchain_image_views () {
    VkResult vk_result = VK_SUCCESS;

    vkGetSwapchainImagesKHR(device, swapchain, &swapchain_image_count, nullptr);
    swapchain_images.resize(swapchain_image_count);
    vkGetSwapchainImagesKHR(device, swapchain, &swapchain_image_count, swapchain_images.data());
    swapchain_image_views.resize(swapchain_image_count);

    VkImageSubresourceRange subresource_range = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    VkComponentMapping component_mapping = {VK_COMPONENT_SWIZZLE_IDENTITY,
                                            VK_COMPONENT_SWIZZLE_IDENTITY,
                                            VK_COMPONENT_SWIZZLE_IDENTITY,
                                            VK_COMPONENT_SWIZZLE_IDENTITY};
    VkImageViewCreateInfo image_view_create_info = {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            nullptr,
            0,
            VK_NULL_HANDLE,
            VK_IMAGE_VIEW_TYPE_2D,
            chosen_surface_format.format,
            component_mapping,
            subresource_range
    };

    for (size_t i = 0; i < swapchain_image_count; ++i) {
        image_view_create_info.image = swapchain_images[i];
        vk_result = vkCreateImageView(device, &image_view_create_info, nullptr,
                                      &swapchain_image_views[i]);

        if (vk_result != VK_SUCCESS) {
            return AGE_RESULT::ERROR_GRAPHICS_CREATE_IMAGE_VIEW;
        }
    }

    return AGE_RESULT::SUCCESS;
}

AGE_RESULT get_device_queues ()
{
    size_t graphics_queue_index = 0;
    size_t compute_queue_index = graphics_queue_family_index == compute_queue_family_index ? 1 : 0;
    size_t transfer_queue_index = transfer_queue_family_index == compute_queue_family_index ? compute_queue_index + 1 : 0;

    vkGetDeviceQueue (device, graphics_queue_family_index, graphics_queue_index, &graphics_queue);
    vkGetDeviceQueue (device, compute_queue_family_index, compute_queue_index, &compute_queue);
    vkGetDeviceQueue (device, transfer_queue_family_index, transfer_queue_index, &transfer_queue);

    return AGE_RESULT::SUCCESS;
}

AGE_RESULT create_graphics_command_pool () {
    VkCommandPoolCreateInfo graphics_command_pool_create_info = {
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            nullptr,
            VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
            graphics_queue_family_index
    };

    VkResult vk_result = vkCreateCommandPool(device, &graphics_command_pool_create_info, nullptr,
                                             &graphics_command_pool);
    if (vk_result != VK_SUCCESS) {
        return AGE_RESULT::ERROR_GRAPHICS_CREATE_COMMAND_POOL;
    }

    return AGE_RESULT::SUCCESS;
}

AGE_RESULT create_common_sampler () {
    VkSamplerCreateInfo sampler_create_info = {
            VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            nullptr,
            0,
            VK_FILTER_LINEAR,
            VK_FILTER_LINEAR,
            VK_SAMPLER_MIPMAP_MODE_LINEAR,
            VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
            VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
            VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
            0,
            VK_FALSE,
            0,
            VK_FALSE,
            VK_COMPARE_OP_NEVER,
            0,
            0,
            VK_BORDER_COLOR_INT_OPAQUE_BLACK,
            VK_FALSE
    };

    VkResult vk_result = vkCreateSampler(device, &sampler_create_info, nullptr, &common_sampler);
    if (vk_result != VK_SUCCESS) {
        return AGE_RESULT::ERROR_GRAPHICS_CREATE_SAMPLER;
    }

    return AGE_RESULT::SUCCESS;
}

AGE_RESULT vulkan_interface_init (struct android_app* pApp) {
    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "vulkan_interface_init");
    if (!LoadVulkanSymbols()) {
        __android_log_write(ANDROID_LOG_ERROR, TAG, "Vulkan not found");
        return AGE_RESULT::ERROR_LOADING_SYMBOLS;
    }

    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

#ifdef NDEBUG
    is_validation_needed = false;
#else
    is_validation_needed = true;
#endif

    age_result = create_instance();
    if (age_result != AGE_RESULT::SUCCESS) {
        return age_result;
    }

    age_result = create_surface(pApp->window);
    if (age_result != AGE_RESULT::SUCCESS) {
        return age_result;
    }

    age_result = get_physical_device();
    if (age_result != AGE_RESULT::SUCCESS) {
        return age_result;
    }

    age_result = check_physical_device_surface_support();
    if (age_result != AGE_RESULT::SUCCESS) {
        return age_result;
    }

    age_result = get_physical_device_queue_families();
    if (age_result != AGE_RESULT::SUCCESS) {
        return age_result;
    }

    age_result = get_physical_device_properties();
    if (age_result != AGE_RESULT::SUCCESS) {
        return age_result;
    }

    age_result = create_device();
    if (age_result != AGE_RESULT::SUCCESS) {
        return age_result;
    }

    if (is_validation_needed && is_debug_utils_ext_avaiable) {
        age_result = create_debug_utils_messenger();
        if (age_result != AGE_RESULT::SUCCESS) {
            return age_result;
        }
    }

    age_result = create_swapchain();
    if (age_result != AGE_RESULT::SUCCESS) {
        return age_result;
    }

    age_result = create_swapchain_image_views();
    if (age_result != AGE_RESULT::SUCCESS) {
        return age_result;
    }

    age_result = get_device_queues();
    if (age_result != AGE_RESULT::SUCCESS) {
        return age_result;
    }

    age_result = create_graphics_command_pool();
    if (age_result != AGE_RESULT::SUCCESS) {
        return age_result;
    }

    age_result = create_common_sampler();
    if (age_result != AGE_RESULT::SUCCESS) {
        return age_result;
    }

    return age_result;
}

void vulkan_interface_shutdown () {
    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "vulkan_interface_shutdown");

    if (graphics_command_pool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(device, graphics_command_pool, nullptr);
    }

    if (common_sampler != VK_NULL_HANDLE) {
        vkDestroySampler(device, common_sampler, nullptr);
    }

    for (auto &swapchain_image_view : swapchain_image_views) {
        if (swapchain_image_view != VK_NULL_HANDLE) {
            vkDestroyImageView(device, swapchain_image_view, nullptr);
        }
    }
    swapchain_image_views.clear();

    if (swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(device, swapchain, nullptr);
    }

    swapchain_images.clear();

    if (surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }

    if (is_validation_needed && is_debug_utils_ext_avaiable) {
        destroy_debug_utils_messenger();
    }

    if (device != VK_NULL_HANDLE) {
        vkDestroyDevice(device, nullptr);
    }

    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
    }
}