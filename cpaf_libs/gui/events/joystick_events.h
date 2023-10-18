#pragma once
#include <cstdint>

namespace cpaf::gui::joystick {

////////////////////////////////////////////////////////////
/// \brief Constants related to joysticks capabilities
///
////////////////////////////////////////////////////////////
// NOLINTBEGIN(readability-identifier-naming)
static constexpr std::uint8_t count         {8};    //!< Maximum number of supported joysticks
static constexpr std::uint8_t button_count  {32};   //!< Maximum number of supported buttons
static constexpr std::uint8_t axis_count    {8};    //!< Maximum number of supported axes
// NOLINTEND(readability-identifier-naming)

////////////////////////////////////////////////////////////
/// \brief Axes supported by SFML joysticks
///
////////////////////////////////////////////////////////////
enum class axis : std::int8_t
{
    x,      //!< The X axis
    y,      //!< The Y axis
    z,      //!< The Z axis
    r,      //!< The R axis
    u,      //!< The U axis
    v,      //!< The V axis
    povx,   //!< The X axis of the point-of-view hat
    povy,   //!< The Y axis of the point-of-view hat

    COUNT   //!< Keep last -- the total number of joystick axes
};

} //END namespace cpaf::gui::joystick
