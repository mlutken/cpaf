#pragma once
#include <cstdint>

namespace cpaf::gui::mouse {

////////////////////////////////////////////////////////////
/// \brief Mouse buttons
///
////////////////////////////////////////////////////////////
enum class button : std::int8_t
{
    left,       //!< The left mouse button
    right,      //!< The right mouse button
    middle,     //!< The middle (wheel) mouse button
    xbutton1,   //!< The first extra mouse button
    xbutton2,   //!< The second extra mouse button

    COUNT       //!< Keep last -- the total number of mouse buttons
};

////////////////////////////////////////////////////////////
/// \brief Mouse wheels
///
////////////////////////////////////////////////////////////
enum class wheel : std::int8_t
{
    vertical,   //!< The vertical mouse wheel
    horizontal, //!< The horizontal mouse wheel

    COUNT       //!< Keep last -- the total number of mouse wheels
};

} //END namespace cpaf::gui::mouse
