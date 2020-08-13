//
// Created by nihal on 13-08-2020.
//

#include <string.h>
#include <jni.h>

JNIEXPORT jstring JNICALL
Java_com_ntkinteractive_doddroid_MainActivity_stringFromJNI(JNIEnv *env, jobject this) {
    return (*env)->NewStringUTF (env, "VK Incoming!");
}