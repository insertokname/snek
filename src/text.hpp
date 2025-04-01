#pragma once

#include <string_view>

#include "SDL_ttf.h"
#include "colors.hpp"
#include "rect_tools.hpp"

namespace snek::text {

    struct TextStyleParams {
        colors::Color color;
        TTF_Font *font;
    };

    class Text {
    public:
        std::string_view content;
        colors::Color color;
        TTF_Font *font;
        Text(std::string_view content, const TextStyleParams &text_style)
            : content(content),
              color(text_style.color),
              font(text_style.font) {}

        Text(std::string_view content, colors::Color color, TTF_Font *font)
            : content(content), color(color), font(font) {}
    };

    void draw_text_in_rect(const Text &text,
                           const SDL_Rect &parent_rect,
                           const rect_tools::RectStyle &style,
                           SDL_Renderer *renderer);
}