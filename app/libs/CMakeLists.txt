cmake_minimum_required (VERSION 3.4.1)

add_definitions (-DVK_USE_PLATFORM_ANDROID_KHR)
add_library (native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
target_include_directories (native_app_glue PUBLIC ${ANDROID_NDK}/sources/android/native_app_glue)

set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_library (game SHARED game.cpp vulkan_wrapper.cpp vulkan_interface.cpp graphics.cpp main.cpp log.cpp utils.cpp types.cpp)

target_link_libraries (game native_app_glue android log)