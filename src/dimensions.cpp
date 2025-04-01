#include "dimensions.hpp"

#include <SDL_video.h>

namespace snek {
    Dimensions get_screen_dimension(SDL_Window* window) {
        int tmp_width = 0, tmp_height = 0;
        SDL_GetWindowSize(window, &tmp_width, &tmp_height);

        return Dimensions{
            .height = static_cast<size_t>(tmp_height),
            .width = static_cast<size_t>(tmp_width),
        };
    }
}