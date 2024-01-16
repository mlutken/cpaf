#pragma once

#include <cstdint>
#include <string>
#include <cpaf_libs/math/base/v2.hpp>
#include <cpaf_libs/math/base/v4.hpp>

namespace cpaf::gui {

/** \unit gui_types.h
 *  Basic gui related types and enums
\author Martin Lutken
*/

using size_2d = cpaf::math::v2<std::int32_t>;
using pos_2d = cpaf::math::v2<std::int32_t>;

using size_2df = cpaf::math::v2<float>;
using pos_2df = cpaf::math::v2<float>;


/** @todo Rename to rect */
struct rect
{
    rect() = default;
    ~rect() = default;
    rect(const rect& other) = default;
    rect& operator=(const rect& other) = default;
    explicit rect(size_2df render_size) : size(render_size) {}

    rect(pos_2df  top_left_corner_pos, size_2df render_size)
        : top_left(top_left_corner_pos)
        , size(render_size) {}

    rect(float top_left_x, float top_left_y, float render_width, float render_height)
        : top_left(top_left_x, top_left_y)
        , size(render_width, render_height)
    {}

    pos_2df     top_left    {0,0};
    size_2df    size        {0,0};
};

enum class exit_status_t : int { success = 0, failure = 1 };




//std::string to_string   (audio_format_t audio_format);

} //END namespace cpaf::gui

