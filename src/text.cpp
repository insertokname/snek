#include "text.hpp"

#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_video.h>

#include <string_view>

#include "SDL_ttf.h"
#include "colors.hpp"
#include "config.hpp"
#include "rect_tools.hpp"

namespace snek::text {
    TextCreateResult::~TextCreateResult() {
        SDL_FreeSurface(this->surface);
        SDL_DestroyTexture(this->texture);
    }

    TextCreateResult create_text_texture(SDL_Renderer *renderer,
                                         const Text &text) {
        SDL_Surface *surface = TTF_RenderText_Solid(
            text.font, text.content.data(), text.color.to_sdl_color());

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        return {.texture = texture, .surface = surface};
    }

    void draw_text_in_rect(const Text &text,
                           const SDL_Rect &parent_rect,
                           const rect_tools::RectStyle &style,
                           SDL_Renderer *renderer) {
        TextCreateResult text_create_result =
            create_text_texture(renderer, text);

        SDL_Rect out_text_rect{
            0,
            0,
        };
        SDL_QueryTexture(text_create_result.texture,
                         nullptr,
                         nullptr,
                         &out_text_rect.w,
                         &out_text_rect.h);

        rect_tools::apply_rect_style(out_text_rect, parent_rect, style);

        SDL_RenderCopy(
            renderer, text_create_result.texture, nullptr, &out_text_rect);
    }

    int get_relative_font_size(const int initial_font_size,
                               SDL_Window *window) {
        int height = game_config::INITIAL_SCREEN_SIZE.height,
            width = game_config::INITIAL_SCREEN_SIZE.width;
        SDL_GetWindowSize(window, &width, &height);

        double width_ratio =
            static_cast<double>(width) / game_config::INITIAL_SCREEN_SIZE.width;
        double height_ratio = static_cast<double>(height) /
                              game_config::INITIAL_SCREEN_SIZE.height;

        return static_cast<int>(std::min(width_ratio, height_ratio) *
                                initial_font_size);
    }
}