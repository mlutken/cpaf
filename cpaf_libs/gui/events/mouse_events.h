#pragma once
#include <cstdint>
#include <cpaf_libs/gui/events/base_events.h>

namespace cpaf::gui::events {


/** @brief Mouse events
 *
 *  The pointer_type which may be pointer_type::mouse for real mouse events or pointer_type::touch for events that were generated by
 *  a touch input device, and . You might want to ignore such events, if your application already handles events::touch.
*/
struct mouse : public base
{
    enum class type {
        unknown,                //!< Unknown type
        move,                   //!< Mouse pointer was moved
        button_down,            //!< Mouse button pressed down
        button_up,              //!< Mouse button released (up)
        clicked,                //!< Mouse button clicked (down => up)
        double_clicked,         //!< Mouse button clicked (down => up), (down => up)
        wheel,                  //!< Mouse wheel event
        entered,                //!< Mouse entered region/main window
        left,                   //!< Mouse left region/main window
        COUNT
    };

    /**
    * @brief Mouse buttons
    */
    enum class button : std::int8_t
    {
        unknown,    //!< Unknown button
        left,       //!< The left mouse button
        right,      //!< The right mouse button
        middle,     //!< The middle (wheel) mouse button
        xbutton1,   //!< The first extra mouse button
        xbutton2,   //!< The second extra mouse button

        COUNT       //!< Keep last -- the total number of mouse buttons
    };

    /**
    * @brief Mouse wheels
    */
    enum class wheel : std::int8_t
    {
        unknown,        //!< Unknown wheel
        vertical,       //!< The vertical mouse wheel
        horizontal,     //!< The horizontal mouse wheel

        COUNT       //!< Keep last -- the total number of mouse wheels
    };


    mouse::type             tp          = mouse::type::move;        //!< Mouse event type
    std::int32_t            x           = 0;                        //!< X position of the mouse pointer, relative to the left of the owner window
    std::int32_t            y           = 0;                        //!< Y position of the mouse pointer, relative to the top of the owner window
    std::int32_t            xrel        = 0;                        //!< Relative motion in the X direction
    std::int32_t            yrel        = 0;                        //!< Relative motion in the Y direction
    mouse::button           button      = mouse::button::left;      //!< Which button
    mouse::wheel            wheel       = mouse::wheel::vertical;   //!< Which wheel (for mice with multiple ones)
    float                   delta_wheel = 0;                        //!< Wheel offset (positive is up/left, negative is down/right). High-precision mice may use non-integral offsets.
    events::pointer_type    pointer_type= pointer_type::mouse;      //!< Event generated by a mouse or a touch device
};



} //END namespace cpaf::gui::events
