#pragma once

#include <cstddef>
namespace snek {
    constexpr std::size_t INITIAL_SCREEN_WIDTH = 1280,
                          INITIAL_SCREEN_HEIGHT = 720,
                          BOARD_HEIGHT = 10, BOARD_WIDTH = 20,
                          MOVE_SPEED = 250000;
#ifdef __EMSCRIPTEN__
    constexpr std::size_t PADDING = 0;
#else
    constexpr std::size_t PADDING = 75;
#endif
}