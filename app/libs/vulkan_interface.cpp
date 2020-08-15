//
// Created by nihal on 15-08-2020.
//

#include "vulkan_wrapper.hpp"
#include "vulkan_interface.hpp"

#include <android/log.h>

static const char* TAG = "GameVulkanInterface";

void vulkan_interface_init ()
{
    LoadVulkanSymbols();

    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "vulkan_interface_init");
}

void vulkan_interface_shutdown ()
{
    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "vulkan_interface_shutdown");
}