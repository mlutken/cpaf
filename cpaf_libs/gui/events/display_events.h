#pragma once
#include <cstdint>

namespace cpaf::gui::display {

////////////////////////////////////////////////////////////
/// \brief Display orientaion
///
////////////////////////////////////////////////////////////
enum class orientation : std::uint8_t
{
    horizontal,     //!< Horizontal display orientation
    vertical        //!< Vertical display orientation

};

} //END namespace cpaf::gui::display
