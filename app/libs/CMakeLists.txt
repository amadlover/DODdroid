cmake_minimum_required (VERSION 3.4.1)

add_library (game SHARED game.cpp vulkan_wrapper.cpp)

target_link_libraries (game android log)