#pragma once

#include <cstddef>

namespace snek {
    namespace game_config

    {
        constexpr double FOLLOW_EXACT_THRESHOLD = 0.60;
        constexpr std::size_t INITIAL_SCREEN_WIDTH = 1280,
                              INITIAL_SCREEN_HEIGHT = 720, BOARD_HEIGHT = 10,
                              BOARD_WIDTH = 20;
#ifdef __EMSCRIPTEN__
        constexpr std::size_t PADDING = 0;
#else
        constexpr std::size_t PADDING = 75;
#endif

#ifdef SNEK_ALGORITHM
        constexpr std::size_t MOVE_SPEED = 50000;
#else
        constexpr std::size_t MOVE_SPEED = 225000;
#endif
    }

    namespace gui_config {
        constexpr double GAME_OVER_TEXT_X_POS = 0.5;
        constexpr double GAME_OVER_TITLE_Y_POS = 0.3;
        constexpr double GAME_OVER_SUBHEADING1_Y_POS = 0.6;
        constexpr double GAME_OVER_SUBHEADING2_Y_POS = 0.7;
        constexpr int GAME_OVER_FONT_SIZE = 50;
        constexpr double GAME_OVER_XY_POS = 0.5;
    }
}