//
// Created by nihal on 15-08-2020.
//

#include "vulkan_interface.hpp"

#include <jni.h>
#include <android/log.h>

#include "game.hpp"

AGE_RESULT game_init (struct android_app* pApp) {
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    age_result = vulkan_interface_init(pApp);

    return age_result;
}

void game_shutdown () {
    vulkan_interface_shutdown();
}
