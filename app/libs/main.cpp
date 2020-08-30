//
// Created by nihal on 15-08-2020.
//

#include <android_native_app_glue.h>
#include <jni.h>
#include <android/log.h>

#include "game.hpp"
#include "log.hpp"

const char* TAG = "Asteroids";

static void handle_cmd (struct android_app* p_app, int cmd) {
    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "handling cmd");
}

static int32_t handle_input_event (struct android_app* p_app, AInputEvent* event) {
    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "handling input");

    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        float x = AMotionEvent_getX(event, 0);
        float y = AMotionEvent_getY(event, 0);

        __android_log_print(ANDROID_LOG_VERBOSE, TAG, "x: %f y: %f\n", x, y);
        return 1;
    }

    return 0;
}

void android_main(struct android_app *p_app) {
    p_app->onAppCmd = handle_cmd;
    p_app->onInputEvent = handle_input_event;

    while (true) {
        int id;
        int events;
        struct android_poll_source *p_source;

        while ((id = ALooper_pollAll(0, nullptr, &events, (void **) &p_source)) >= 0) {
            if (p_source != nullptr) {
                p_source->process(p_app, p_source);
            }

            if (p_app->destroyRequested == 1) {
                return;
            }
        }
    }
}