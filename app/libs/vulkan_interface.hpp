//
// Created by nihal on 15-08-2020.
//

#ifndef DODDROID_VULKAN_INTERFACE_HPP
#define DODDROID_VULKAN_INTERFACE_HPP

#endif //DODDROID_VULKAN_INTERFACE_HPP

#include <vector>

#include "error.hpp"
#include "vulkan_wrapper.hpp"

extern VkInstance instance;
extern VkPhysicalDevice physical_device;
extern VkDevice device;
extern uint32_t graphics_queue_family_index;
extern uint32_t compute_queue_family_index;
extern uint32_t transfer_queue_family_index;
extern VkQueue graphics_queue;
extern VkQueue compute_queue;
extern VkQueue transfer_queue;
extern VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
extern VkPhysicalDeviceLimits physical_device_limits;
extern VkExtent2D surface_extent;
extern VkSwapchainKHR swapchain;
extern std::vector<VkImage> swapchain_images;
extern std::vector<VkImageView> swapchain_image_views;
extern size_t swapchain_image_count;
extern VkExtent2D current_extent;
extern VkSurfaceFormatKHR chosen_surface_format;
extern VkCommandPool transfer_command_pool;
extern VkSampler common_sampler;


AGE_RESULT vulkan_interface_init (struct android_app* pApp);
void vulkan_interface_shutdown ();