//
// Created by nihal on 15-08-2020.
//

#include <android_native_app_glue.h>
#include <jni.h>
#include <android/log.h>
#include <memory>
#include <android/input.h>
#include <android/native_window.h>

#include "game.hpp"
#include "log.hpp"

const char* TAG = "Asteroids";

static void draw_something (struct android_app* p_app) {
    __android_log_write(ANDROID_LOG_VERBOSE, TAG, "draw_something");
    ANativeWindow_Buffer window_buffer;
    ANativeWindow *p_window = p_app->window;
    ANativeWindow_setBuffersGeometry(p_window, 0, 0, WINDOW_FORMAT_RGBA_8888);
    if (ANativeWindow_lock(p_window, &window_buffer, nullptr) < 0) {
        return;
    }
    memset(window_buffer.bits, 0, window_buffer.stride * window_buffer.height * sizeof(uint32_t));

    ANativeWindow_unlockAndPost(p_window);
}

static void handle_cmd (struct android_app* p_app, int cmd) {
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "handling cmd");

    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            game_init(p_app);
            game_submit_present();
            break;

        case APP_CMD_TERM_WINDOW:
            game_shutdown();
            break;

        default:
            break;
    }
}

static int32_t handle_input_event (struct android_app* p_app, AInputEvent* event) {
    __android_log_print(ANDROID_LOG_VERBOSE, TAG, "handling input");

    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        int32_t action = AMotionEvent_getAction(event);

        int event_action = AMotionEvent_getAction(event);
        switch (event_action) {
            case AMOTION_EVENT_ACTION_DOWN:
                __android_log_write(ANDROID_LOG_VERBOSE, TAG, "down");
                break;

            case AMOTION_EVENT_ACTION_MOVE:
                __android_log_write(ANDROID_LOG_VERBOSE, TAG, "move");
                break;

            case AMOTION_EVENT_ACTION_UP:
                __android_log_write(ANDROID_LOG_VERBOSE, TAG, "up");
                break;

            case AMOTION_EVENT_ACTION_HOVER_ENTER:
                __android_log_write(ANDROID_LOG_VERBOSE, TAG, "hover enter");
                break;

            case AMOTION_EVENT_ACTION_HOVER_MOVE:
                __android_log_write(ANDROID_LOG_VERBOSE, TAG, "hover move");
                break;

            case AMOTION_EVENT_ACTION_HOVER_EXIT:
                __android_log_write(ANDROID_LOG_VERBOSE, TAG, "hover exit");
                break;

            default:
                break;
        }

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

        if ((id = ALooper_pollOnce(-1, nullptr, &events,
                                     reinterpret_cast<void **>  (&p_source))) >= 0) {
            if (p_source != nullptr) {
                p_source->process(p_app, p_source);
            }

            if (p_app->destroyRequested == 1) {
                return;
            }
        }
    }
}