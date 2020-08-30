//
// Created by nihal on 15-08-2020.
//

#ifndef DODDROID_GAME_HPP
#define DODDROID_GAME_HPP

#include "error.hpp"
#include <cstddef>

AGE_RESULT game_init (struct android_app* p_app);
AGE_RESULT game_update (size_t delta_msecs);
AGE_RESULT game_submit_present ();
void game_shutdown ();

#endif //DODDROID_GAME_HPP