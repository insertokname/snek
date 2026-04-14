#pragma once

#include <cstddef>

#include "dimensions.hpp"

namespace snek {
    namespace game_config {
        constexpr double FOLLOW_EXACT_THRESHOLD = 0.60;
        constexpr std::size_t RENDER_INTERVAL_US = 16667;
        constexpr int MIN_SWIPE_DISTANCE = 30;
        constexpr Dimensions INITIAL_SCREEN_SIZE = Dimensions{
            .height = 720,
            .width = 1280,
        };
        constexpr Dimensions BOARD_SIZE = Dimensions{
            .height = 10,
            .width = 20,
        };
#ifdef __EMSCRIPTEN__
        constexpr std::size_t PADDING = 0;
#else
        constexpr std::size_t PADDING = 75;
#endif

#ifdef SNEK_ALGORITHM
#ifdef SNEK_PERF
        constexpr std::size_t MOVE_SPEED = 0;
#else
        constexpr std::size_t MOVE_SPEED = 50000;
#endif
        constexpr std::size_t INPUT_POLL_INTERVAL_US = 8000;
#else
        constexpr std::size_t MOVE_SPEED = 225000;
        constexpr std::size_t INPUT_POLL_INTERVAL_US = 0;
#endif
    }

    namespace gui_config {
        constexpr int MIN_CELL_SIZE = 1;
        constexpr double BOARD_XY_POS = 0.5;

        constexpr double POPUP_SCREEN_TOP_SIDE_HEIGHT = 0.5;
        constexpr double POPUP_SCREEN_TOP_SIDE_Y_POS = 0.1;
        constexpr double POPUP_SCREEN_BOTTOM_SIDE_HEIGHT = 0.4;
        constexpr double POPUP_SCREEN_BOTTOM_SIDE_Y_POS = 0.8;

        constexpr double POPUP_SCREEN_XY_POS = 0.5;
        constexpr double POPUP_SCREEN_TITLE_Y_POS = 0.5;
        constexpr double POPUP_SCREEN_SUB_TITLE_Y_POS = 0.5;
        constexpr double POPUP_SCREEN_TEXT_X_POS = 0.5;

        constexpr int POPUP_SCREEN_TITLE_FONT_SIZE = 75;
        constexpr int POPUP_SCREEN_SUB_TITLE_FONT_SIZE = 35;
        constexpr int POPUP_SCREEN_ERROR_SUB_TITLE_FONT_SIZE = 25;
        constexpr int POPUP_BORDER_THICKNESS = 4;
    }
}
