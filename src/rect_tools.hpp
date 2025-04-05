#pragma once

#include "SDL_rect.h"
#include "SDL_video.h"

namespace snek::rect_tools {
    namespace rel_rect_tools {
        // Requires that out_child_rect has an initialized width!
        void set_relative_x_pos(SDL_Rect &out_child_rect,
                                const SDL_Rect &parent_rect,
                                double x_pos_percent);

        // Requires that out_child_rect has an initialized height!
        void set_relative_y_pos(SDL_Rect &out_child_rect,
                                const SDL_Rect &parent_rect,
                                double y_pos_percent);

        void set_relative_height(SDL_Rect &out_child_rect,
                                 const SDL_Rect &parent_rect,
                                 double height_percent);

        void set_relative_width(SDL_Rect &out_child_rect,
                                const SDL_Rect &parent_rect,
                                double width_percent);
    }

    class RectStyle {
        friend void apply_rect_style(SDL_Rect &out_child_rect,
                                     const SDL_Rect &parent_rect,
                                     const RectStyle &style);

    public:
        RectStyle() = default;
        RectStyle(const RectStyle &) = default;
        RectStyle(RectStyle &&) = delete;
        RectStyle &operator=(const RectStyle &) = default;
        RectStyle &operator=(RectStyle &&) = delete;
        virtual ~RectStyle() = default;

    private:
        virtual void m_apply(SDL_Rect &out_child_rect,
                             const SDL_Rect &parent_rect) const = 0;
    };

    struct SymmetricRelativeRectParams {
        double pos_percent;
        double size_percent;
    };
    class SymmetricRelativeRectStyle : public RectStyle {
    public:
        explicit SymmetricRelativeRectStyle(
            const SymmetricRelativeRectParams &params)
            : m_pos_percent(params.pos_percent),
              m_size_percent(params.pos_percent) {}

    private:
        double m_pos_percent;
        double m_size_percent;
        void m_apply(SDL_Rect &out_child_rect,
                     const SDL_Rect &parent_rect) const override;
    };

    struct RelativeRectParams {
        double relative_x_pos;
        double relative_y_pos;
        double relative_width;
        double relative_height;
    };
    class RelativeRectStyle : public RectStyle {
    public:
        explicit RelativeRectStyle(const RelativeRectParams &params)
            : m_relative_height(params.relative_height),
              m_relative_width(params.relative_width),
              m_relative_x_pos(params.relative_x_pos),
              m_relative_y_pos(params.relative_y_pos) {}

    private:
        double m_relative_x_pos;
        double m_relative_y_pos;
        double m_relative_width;
        double m_relative_height;
        void m_apply(SDL_Rect &out_child_rect,
                     const SDL_Rect &parent_rect) const override;
    };

    struct BorderRectParams {
        int border_thickness;
    };
    class BorderRectStyle : public RectStyle {
    public:
        explicit BorderRectStyle(const BorderRectParams &params)
            : m_border_thickness(params.border_thickness) {}

    private:
        int m_border_thickness;
        void m_apply(SDL_Rect &out_child_rect,
                     const SDL_Rect &parent_rect) const override;
    };

    struct RelativePositionedRectParams {
        double relative_x_pos;
        double relative_y_pos;
    };
    class RelativePositionedRectStyle : public RectStyle {
    public:
        explicit RelativePositionedRectStyle(
            const RelativePositionedRectParams &params)
            : m_relative_x_pos(params.relative_x_pos),
              m_relative_y_pos(params.relative_y_pos) {}

    private:
        double m_relative_x_pos;
        double m_relative_y_pos;
        void m_apply(SDL_Rect &out_child_rect,
                     const SDL_Rect &parent_rect) const override;
    };

    struct SymmetrictPaddingRectParams {
        int x_padding;
        int y_padding;
    };
    class SymmetrictPaddingRectStyle : public RectStyle {
    public:
        explicit SymmetrictPaddingRectStyle(
            const SymmetrictPaddingRectParams &params)
            : m_x_padding(params.x_padding), m_y_padding(params.y_padding) {}

    private:
        int m_x_padding;
        int m_y_padding;
        void m_apply(SDL_Rect &out_child_rect,
                     const SDL_Rect &parent_rect) const override;
    };

    void apply_rect_style(SDL_Rect &out_child_rect,
                          const SDL_Rect &parent_rect,
                          const RectStyle &style);

    void get_screen_rect(SDL_Window *window, SDL_Rect &out_screen_rect);
}