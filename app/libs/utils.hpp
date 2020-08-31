//
// Created by nihal on 26-08-2020.
//

#ifndef DODDROID_UTILS_HPP
#define DODDROID_UTILS_HPP

#include <cstdint>
#include <android/asset_manager.h>

uint8_t* utils_load_image_data_from_asset (AAsset* image_asset, int* width, int* height, int* bpp);

void* utils_malloc (const size_t size);
void* utils_realloc (void* ptr, const size_t size);
void utils_free (void* ptr);


#endif //DODDROID_UTILS_HPP
