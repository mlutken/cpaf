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
using size_2df = cpaf::math::v2<float>;
using pos_2d = cpaf::math::v2<std::int32_t>;
using pos_2df = cpaf::math::v2<float>;
using pos_2df = cpaf::math::v2<float>;


struct render_geometry_t
{
    render_geometry_t() = default;
    ~render_geometry_t() = default;
    render_geometry_t(const render_geometry_t& other) = default;
    render_geometry_t& operator=(const render_geometry_t& other) = default;
    explicit render_geometry_t(size_2d render_size) : size(render_size) {}

    render_geometry_t(pos_2d  top_left_corner_pos, size_2d render_size)
        : size(render_size), top_left(top_left_corner_pos) {}

    size_2d size        {0,0};
    pos_2d  top_left    {0,0};
};

enum class exit_status_t : int { success = 0, failure = 1 };




//std::string to_string   (audio_format_t audio_format);

} //END namespace cpaf::gui

