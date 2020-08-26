//
// Created by nihal on 15-08-2020.
//

#include "log.hpp"
#include <android/log.h>

AGE_RESULT log_error (const AGE_RESULT result) {
    const char *TAG = "Asteroids";

    switch (result) {
        case AGE_RESULT::ERROR_LOADING_SYMBOLS:
            __android_log_write(ANDROID_LOG_ERROR, TAG,
                                "Vulkan Error: Loading Vulkan Symbols\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_POPULATE_INSTANCE_LAYERS_AND_EXTENSIONS:
            __android_log_write(ANDROID_LOG_ERROR, TAG,
                                "Graphics Error: Populating instance Layers and Extensions\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_INSTANCE:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create instance\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_SETUP_DEBUG_UTILS_MESSENGER:
            __android_log_write(ANDROID_LOG_ERROR, TAG,
                                "Graphics Error: Setup Debug Utils Messenger\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_DEBUG_UTILS_MESSENGER:
            __android_log_write(ANDROID_LOG_ERROR, TAG,
                                "Graphics Error: Create Debug Utils Messenger\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_DESTROY_DEBUG_UTILS_MESSENGER:
            __android_log_write(ANDROID_LOG_ERROR, TAG,
                                "Graphics Error: Destroy Debud Utils Messenger\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_SURFACE:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create surface\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_GET_PHYSICAL_DEVICE:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Get Physical Device\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_POPULATE_DEVICE_LAYERS_AND_EXTENSIONS:
            __android_log_write(ANDROID_LOG_ERROR, TAG,
                                "Graphics Error: Populate Device Layers and Extensions\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_GRAPHICS_DEVICE:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create Graphics Device\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_SURFACE_SUPPORT:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: surface Support\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_SWAPCHAIN:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create swapchain\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_IMAGE_VIEW:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create Image View\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_BUFFER:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create buffer\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_ALLOCATE_MEMORY:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Allocate Memory\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_BIND_BUFFER_MEMORY:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Bind buffer Memory\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_IMAGE:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create Image\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_BIND_IMAGE_MEMORY:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Bind Image Memory\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_MAP_MEMORY:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Map Image Memory\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_DESCRIPTOR_SET_LAYOUT:
            __android_log_write(ANDROID_LOG_ERROR, TAG,
                                "Graphics Error: Create Descriptor Set Layout\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_PIPELINE_LAYOUT:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create Pipeline Layout\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_DESCRIPTOR_POOL:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create Descriptor Pool\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_ALLOCATE_DESCRIPTOR_SETS:
            __android_log_write(ANDROID_LOG_ERROR, TAG,
                                "Graphics Error: Allocate Descriptor Set\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_RENDER_PASS:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create Render Pass\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_SHADER_MODULE:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create Shader Module\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_FRAMEBUFFER:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create Framebuffer\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_BEGIN_COMMAND_BUFFER:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Begin Command buffer\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_END_COMMAND_BUFFER:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: End Command buffer\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_COMMAND_POOL:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create Command Pool\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_RESET_COMMAND_POOL:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Reset Command Pool\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_ALLOCATE_COMMAND_BUFFER:
            __android_log_write(ANDROID_LOG_ERROR, TAG,
                                "Graphics Error: Allocate Command buffer\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_GRAPHICS_PIPELINE:
            __android_log_write(ANDROID_LOG_ERROR, TAG,
                                "Graphics Error: Create Graphics Pipeline\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_SEMAPHORE:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create Semaphore\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_ACQUIRE_NEXT_IMAGE:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Acquire Next Image\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_WAIT_FOR_FENCES:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Wait for Fence\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_FENCE:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create Fence\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_RESET_FENCES:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Reset Fence\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_QUEUE_SUBMIT:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Queue Submit\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_QUEUE_PRESENT:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Queue Present\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_UPDATE_UNIFORM_BUFFER:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Update Uniform buffer\n");
            break;

        case AGE_RESULT::ERROR_GRAPHICS_CREATE_SAMPLER:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "Graphics Error: Create Sampler\n");
            break;

        case AGE_RESULT::ERROR_GLTF_IMPORT:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "GLTF Error: Import GLTF File\n");
            break;

        case AGE_RESULT::ERROR_SYSTEM_ALLOCATE_MEMORY:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "System Error: Allocate Memory\n");
            break;

        case AGE_RESULT::ERROR_SYSTEM_TMP_FILE:
            __android_log_write(ANDROID_LOG_ERROR, TAG, "System Error: Create TMP File\n");
            break;

        default:
            break;
    }

    return AGE_RESULT::SUCCESS;
}