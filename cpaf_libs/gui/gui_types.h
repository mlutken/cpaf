#pragma once

#include <cstdint>
#include <string>
#include <cpaf_libs/math/base/v2.hpp>

namespace cpaf::gui {

/** \unit gui_types.h
 *  Basic gui related types and enums
\author Martin Lutken
*/

using size_2d = cpaf::math::v2<std::int32_t>;
using pos_2d = cpaf::math::v2<std::int32_t>;


enum class exit_status_t : int { success = 0, failure = 1 };


//std::string to_string   (audio_format_t audio_format);

} //END namespace cpaf::gui

