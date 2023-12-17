#pragma once

#include <cstdint>
#include <string>
#include <cpaf_libs/math/base/v4.hpp>

namespace cpaf::gui {

/** \unit gui_types.h
 *  Basic gui related types and enums
\author Martin Lutken
*/



class color : public cpaf::math::v4f
{
    using cpaf::math::v4f::v4f; // "Import" constructors to scope
};





//std::string to_string   (audio_format_t audio_format);

} //END namespace cpaf::gui

