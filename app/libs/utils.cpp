//
// Created by nihal on 26-08-2020.
//

#include "utils.hpp"
#include <cstdlib>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <android/log.h>


uint8_t* utils_load_image_data_from_asset (AAsset* image_asset, int* width, int* height, int* bpp) {
    uint64_t length = AAsset_getLength64(image_asset);
    auto buffer = (uint8_t *) utils_malloc(length);

    if (AAsset_read(image_asset, buffer, length) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "Asteroids", "Could not read image");
    }

    return stbi_load_from_memory(buffer, length, width, height, bpp, 4);
}

void* utils_malloc (const size_t size)
{
    return std::malloc (size);
}

void* utils_realloc (void* ptr, const size_t size)
{
    return std::realloc(ptr, size);
}

void utils_free (void* ptr)
{
    if (ptr != nullptr) {
        free (ptr);
    }
}
