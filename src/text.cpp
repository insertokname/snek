#include "text.hpp"

#include <SDL_rect.h>
#include <SDL_render.h>

#include <string_view>

#include "SDL_ttf.h"
#include "colors.hpp"
#include "rect_tools.hpp"

namespace snek::text {
    SDL_Texture *create_text_texture(SDL_Renderer *renderer, const Text &text) {
        SDL_Surface *surface = TTF_RenderText_Solid(
            text.font, text.content.data(), text.color.to_sdl_color());

        return SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    void draw_text_in_rect(const Text &text,
                           const SDL_Rect &parent_rect,
                           const rect_tools::RectStyle &style,
                           SDL_Renderer *renderer) {
        SDL_Texture *texture = create_text_texture(renderer, text);

        SDL_Rect out_text_rect{
            0,
            0,
        };
        SDL_QueryTexture(
            texture, nullptr, nullptr, &out_text_rect.w, &out_text_rect.h);

        rect_tools::apply_rect_style(out_text_rect, parent_rect, style);

        SDL_RenderCopy(renderer, texture, nullptr, &out_text_rect);

        SDL_DestroyTexture(texture);
    }
}