#include "app.hpp"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

namespace snek {
#ifdef __EMSCRIPTEN__
    void App::run() {
        static App* app_instance = this;

        static auto main_loop_callback = []() {
            if (app_instance->m_game_loop.get_cur_game_state() ==
                GameState::Quitting) {
                emscripten_cancel_main_loop();
                return;
            }
            app_instance->m_game_loop.game_tick();
        };
        emscripten_set_main_loop(main_loop_callback, 0, 1);
    }
#else
    void App::run() {
        while (this->m_game_context.get_cur_game_state() !=
               GameState::Quitting) {
            this->m_game_loop.game_tick();
        }
    }
#endif
}