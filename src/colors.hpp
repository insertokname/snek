#pragma once

#include <cstdint>

namespace snek::colors {
    struct Color {
        uint8_t r, g, b, a;
    };

    consteval Color rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return {.r = r, .g = g, .b = b, .a = a};
    }

    constexpr Color BLACK = rgba(0, 0, 0, 255);
    constexpr Color TAIL_GREEN = rgba(121, 116, 14, 255);
    constexpr Color BODY_GREEN = rgba(152, 151, 26, 255);
    constexpr Color HEAD_GREEN = rgba(184, 187, 38, 255);
    constexpr Color FOOD_RED = rgba(251, 73, 52, 255);
    constexpr Color BORDER_WHITE = rgba(255, 255, 255, 255);
}