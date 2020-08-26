//
// Created by nihal on 15-08-2020.
//

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

VkResult create_debug_messenger (const VkDebugUtilsMessengerCreateInfoEXT* debug_messenger_create_info)
{
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr (instance, "vkCreateDebugUtilsMessengerEXT");

    if (func)
    {
        return func (instance, debug_messenger_create_info, nullptr, &debug_messenger);
    }
    else
    {
        return VK_ERROR_INITIALIZATION_FAILED;
    }
}

void destroy_debug_messenger ()
{
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr (instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func)
    {
        func (instance, debug_messenger, nullptr);
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


AGE_RESULT create_instance () {
    std::vector<const char *> instance_extensions;
    instance_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    instance_extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);

    std::vector<const char *> instance_layers;

    if (is_validation_needed) {
        instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
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

AGE_RESULT vulkan_interface_init (struct android_app* pApp) {
    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "vulkan_interface_init");
    if (!LoadVulkanSymbols()) {
        __android_log_write(ANDROID_LOG_ERROR, TAG, "Vulkan not found");
        return AGE_RESULT::ERROR_LOADING_SYMBOLS;
    }

    AGE_RESULT age_result = AGE_RESULT::SUCCESS;
    VkResult vk_result = VK_SUCCESS;

#ifdef NDEBUG
    is_validation_needed = false;
#else
    is_validation_needed = true;
#endif

    age_result = create_instance();
    if (age_result != AGE_RESULT::SUCCESS)
    {
        return age_result;
    }

    if (is_validation_needed)
    {

    }

    age_result = create_surface(pApp->window);
    if (age_result != AGE_RESULT::SUCCESS) {
        return age_result;
    }

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