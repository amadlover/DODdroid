//
// Created by nihal on 15-08-2020.
//

#include "vulkan_interface.hpp"
#include <jni.h>
#include <android/log.h>

static const char* TAG = "GameVulkanInterface";

extern "C"
JNIEXPORT void JNICALL
Java_com_ntkinteractive_doddroid_MainActivity_game_1init(JNIEnv *env, jobject thiz) {
    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "game_init");
    vulkan_interface_init();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ntkinteractive_doddroid_MainActivity_game_1update(JNIEnv *env, jobject thiz) {
    // TODO: implement game_update()

    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "game_update");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ntkinteractive_doddroid_MainActivity_game_1shutdown(JNIEnv *env, jobject thiz) {
    // TODO: implement game_shutdown()

    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "game_shutdown");
    vulkan_interface_shutdown();
}
