//
// Created by nihal on 15-08-2020.
//

#include "vulkan_wrapper.hpp"
#include "vulkan_interface.hpp"

#include "error.hpp"

#include <android/log.h>
#include <vector>
#include <android_native_app_glue.h>

bool is_validation_needed = false;
VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;
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
size_t swapchain_image_count;
VkExtent2D current_extent;
VkSurfaceFormatKHR chosen_surface_format;
VkCommandPool transfer_command_pool;
VkSampler common_sampler;

static const char* TAG = "Asteroids";

VkResult create_debug_messenger (VkInstance instance,
                                       const VkDebugUtilsMessengerCreateInfoEXT* debug_messenger_create_info,
                                       const VkAllocationCallbacks* allocation_callbacks,
                                       VkDebugUtilsMessengerEXT* debug_messenger)
{
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr (instance, "vkCreateDebugUtilsMessengerEXT");

    if (func)
    {
        return func (instance, debug_messenger_create_info, allocation_callbacks, debug_messenger);
    }
    else
    {
        return VK_ERROR_INITIALIZATION_FAILED;
    }
}

void destroy_debug_messenger (VkInstance instance,
                                    VkDebugUtilsMessengerEXT debug_messenger,
                                    const VkAllocationCallbacks* allocation_callbacks)
{
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr (instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func)
    {
        func (instance, debug_messenger, allocation_callbacks);
    }
    else
    {
        printf ("Could not destroy debug utils messenger\n");
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL debug_messenger_callback (
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_types,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void* pUserData)
{
    if (callback_data) {
        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "Debug Callback Message: %s\n\n",
                            callback_data->pMessage);
    }

    return VK_FALSE;
}


AGE_RESULT vulkan_interface_init (struct android_app* pApp) {
    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "vulkan_interface_init");
    if (!LoadVulkanSymbols()) {
        __android_log_write(ANDROID_LOG_ERROR, TAG, "Vulkan not found");
    }

    AGE_RESULT age_result = AGE_RESULT::SUCCESS;
    VkResult vk_result = VK_SUCCESS;

#ifdef NDEBUG
    is_validation_needed = false;
#else
    is_validation_needed = true;
#endif

    VkInstanceCreateInfo instance_create_info;
    VkAndroidSurfaceCreateInfoKHR surface_create_info;

    VkApplicationInfo application_info = {
            VK_STRUCTURE_TYPE_APPLICATION_INFO,
            NULL,
            "Asteroids",
            VK_MAKE_VERSION (1, 0, 0),
            "AGE",
            VK_MAKE_VERSION (1, 0, 0),
            VK_API_VERSION_1_1
    };

    std::vector<const char *> instance_extensions, device_extensions;
    instance_extensions.push_back("VK_KHR_surface");
    instance_extensions.push_back("VK_KHR_android_surface");
    device_extensions.push_back("VK_KHR_swapchain");

    instance_create_info = {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            nullptr,
            0,
            &application_info,
            0,
            nullptr,
            instance_extensions.size(),
            instance_extensions.data ()
    };

    vk_result = vkCreateInstance (&instance_create_info,nullptr, &instance);
    if (vk_result!= VK_SUCCESS) {
        age_result = AGE_RESULT::ERROR_GRAPHICS_CREATE_INSTANCE;
        goto exit;
    }

    surface_create_info = {
            VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
            nullptr,
            0,
            pApp->window
    };

    vk_result = vkCreateAndroidSurfaceKHR (instance, &surface_create_info, nullptr, &surface);
    if (vk_result != VK_SUCCESS)
    {
        age_result = AGE_RESULT::ERROR_GRAPHICS_CREATE_SURFACE;
        goto exit;
    }

exit:
    return age_result;
}

void vulkan_interface_shutdown () {
    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "vulkan_interface_shutdown");

    if (surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }

    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
    }
}