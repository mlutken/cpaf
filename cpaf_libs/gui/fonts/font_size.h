#pragma once

#include <cstdint>

namespace cpaf::gui {

class system_window;

/**
 *
 *
 * @see https://websemantics.uk/tools/font-size-conversion-pixel-point-em-rem-percent/
*/

class font_size
{
public:
    static constexpr float dpi_base = 72;
    static const float platform_scale_factor;

    static int32_t to_pixels_display       (float size_in_points, int32_t display_index);
    static int32_t to_pixels               (float size_in_points, const system_window* window_ptr);
    static int32_t to_pixels               (float size_in_points, float dpi);

    enum class mode_t {points, relative};
    static font_size create_for_display (float size_in_points, int32_t display_index);
    static font_size create_relative    (float size_relative)   { return font_size(size_relative, font_size::mode_t::relative);}

    font_size() = default;
    explicit font_size(float size_in_points) : size_in_points_(size_in_points) {}
    explicit font_size(float size_in_points, float dpi) : size_in_points_(size_in_points), dpi_factor_(dpi/dpi_base) {}
    explicit font_size(float size, mode_t mode) : size_in_points_(size), mode_(mode) {}

    uint32_t    in_pixels   () const;
    uint32_t    in_pixels   (float viewport_height) const;

    font_size&  operator +=(float val)  { size_in_points_ += val; return *this; }
    font_size&  operator -=(float val)  { size_in_points_ -= val; return *this; }
    font_size&  operator *=(float val)  { size_in_points_ *= val; return *this; }
    font_size&  operator /=(float val)  { size_in_points_ /= val; return *this; }

    mode_t      mode        () const    { return mode_; }
    float       dpi         () const    { return dpi_factor_;  }

private:
    float   size_in_points_             {11};
    float   dpi_factor_                 {96/dpi_base};
    mode_t  mode_                       = mode_t::points;
};

} // namespace cpaf::gui
