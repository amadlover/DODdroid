//
// Created by nihal on 26-08-2020.
//

#include "utils.hpp"

#include <cstdlib>

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

