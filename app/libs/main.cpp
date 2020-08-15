//
// Created by nihal on 15-08-2020.
//

#include <android_native_app_glue.h>
#include <jni.h>
#include <android/log.h>

#include "game.hpp"
#include "log.hpp"

extern "C" {
void handle_cmd(android_app *pApp, int32_t cmd) {
    AGE_RESULT age_result = AGE_RESULT::SUCCESS;

    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            __android_log_write(ANDROID_LOG_VERBOSE, "Asteroids", "Init Window");

            age_result = game_init(pApp);
            if (age_result != AGE_RESULT::SUCCESS) {
                log_error(age_result);
            }

            break;

        case APP_CMD_TERM_WINDOW:
            __android_log_write(ANDROID_LOG_VERBOSE, "Asteroids", "Term Window");

            game_shutdown();

            break;

        default:
            break;
    }
}
}

void android_main (struct android_app *pApp) {
    pApp->onAppCmd = handle_cmd;

    int events;
    android_poll_source *pSource;

    do {
        if (ALooper_pollAll(0, nullptr, &events, (void **) &pSource) >= 0) {
            if (pSource) {
                pSource->process(pApp, pSource);
            }
        }
    } while (!pApp->destroyRequested);
}