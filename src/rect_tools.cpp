#include "rect_tools.hpp"

#include <cmath>

namespace snek::rect_tools {
    namespace rel_rect_tools {
        void set_relative_x_pos(SDL_Rect &out_child_rect,
                                const SDL_Rect &parent_rect,
                                double x_pos_percent) {
            out_child_rect.x = static_cast<int>(
                std::lerp(parent_rect.x,
                          parent_rect.x + (parent_rect.w - out_child_rect.w),
                          x_pos_percent));
        }

        void set_relative_y_pos(SDL_Rect &out_child_rect,
                                const SDL_Rect &parent_rect,
                                double y_pos_percent) {
            out_child_rect.y = static_cast<int>(
                std::lerp(parent_rect.y,
                          parent_rect.y + (parent_rect.h - out_child_rect.h),
                          y_pos_percent));
        }

        void set_relative_height(SDL_Rect &out_child_rect,
                                 const SDL_Rect &parent_rect,
                                 double height_percent) {
            out_child_rect.h =
                static_cast<int>(std::lerp(0, parent_rect.h, height_percent));
        }

        void set_relative_width(SDL_Rect &out_child_rect,
                                const SDL_Rect &parent_rect,
                                double width_percent) {
            out_child_rect.w =
                static_cast<int>(std::lerp(0, parent_rect.w, width_percent));
        }
    }

    void RelativeRectStyle::m_apply(SDL_Rect &out_child_rect,
                                    const SDL_Rect &parent_rect) const {
        rel_rect_tools::set_relative_width(
            out_child_rect, parent_rect, this->m_relative_width);
        rel_rect_tools::set_relative_height(
            out_child_rect, parent_rect, this->m_relative_height);
        rel_rect_tools::set_relative_x_pos(
            out_child_rect, parent_rect, this->m_relative_x_pos);
        rel_rect_tools::set_relative_y_pos(
            out_child_rect, parent_rect, this->m_relative_y_pos);
    }

    void SymmetricRelativeRectStyle::m_apply(
        SDL_Rect &out_child_rect,
        const SDL_Rect &parent_rect) const {
        rel_rect_tools::set_relative_width(
            out_child_rect, parent_rect, this->m_size_percent);
        rel_rect_tools::set_relative_height(
            out_child_rect, parent_rect, this->m_size_percent);
        rel_rect_tools::set_relative_x_pos(
            out_child_rect, parent_rect, this->m_pos_percent);
        rel_rect_tools::set_relative_y_pos(
            out_child_rect, parent_rect, this->m_pos_percent);
    }

    void BorderRectStyle::m_apply(SDL_Rect &out_child_rect,
                                  const SDL_Rect &parent_rect) const {
        out_child_rect.x = parent_rect.x - this->m_border_thickness;
        out_child_rect.y = parent_rect.y - this->m_border_thickness;
        out_child_rect.w = parent_rect.w + this->m_border_thickness * 2;
        out_child_rect.h = parent_rect.h + this->m_border_thickness * 2;
    }

    void RelativePositionedRectStyle::m_apply(
        SDL_Rect &out_child_rect,
        const SDL_Rect &parent_rect) const {
        rel_rect_tools::set_relative_x_pos(
            out_child_rect, parent_rect, this->m_relative_x_pos);
        rel_rect_tools::set_relative_y_pos(
            out_child_rect, parent_rect, this->m_relative_y_pos);
    }

    void apply_rect_style(SDL_Rect &out_child_rect,
                          const SDL_Rect &parent_rect,
                          const RectStyle &style) {
        style.m_apply(out_child_rect, parent_rect);
    }
}