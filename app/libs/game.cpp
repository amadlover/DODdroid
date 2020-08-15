//
// Created by nihal on 15-08-2020.
//

#include "vulkan_wrapper.hpp"
#include <jni.h>

extern "C"
JNIEXPORT void JNICALL
Java_com_ntkinteractive_doddroid_MainActivity_game_1init(JNIEnv *env, jobject thiz) {
    LoadVulkanSymbols();

}

extern "C"
JNIEXPORT void JNICALL
Java_com_ntkinteractive_doddroid_MainActivity_game_1update(JNIEnv *env, jobject thiz) {
    // TODO: implement game_update()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ntkinteractive_doddroid_MainActivity_game_1shutdown(JNIEnv *env, jobject thiz) {
    // TODO: implement game_shutdown()
}
