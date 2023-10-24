#pragma once
#include <cstdint>
#include <cpaf_libs/gui/events/base_events.h>


namespace cpaf::gui::events {


/** @brief Joystick events
 *
*/

struct joystick : public base
{
    std::string name() const override { return "joystick"; }
    enum class type {
        unknown,        //!< Unknown type
        axis,           //!< User moved an axis of the joystick
        ball,           //!< User moved a trackball on the joystick
        hat,            //!< User moved a hat on the joystick
        button,         //!< User pressed or released a button on the joystick
        connected,      //!< A joystick was connected (data in event.joystickConnect)
        disconnected,   //!< A joystick was disconnected (data in event.joystickConnect)
        COUNT
    };

    ////////////////////////////////////////////////////////////
    /// @brief Axes supported by joysticks (inpired from SFML)
    /// @todo NOT used currently
    ////////////////////////////////////////////////////////////
    // enum class axis : std::int8_t
    // {
    //     unknown,        //!<
    //     x,              //!< The X axis
    //     y,              //!< The Y axis
    //     z,              //!< The Z axis
    //     r,              //!< The R axis
    //     u,              //!< The U axis
    //     v,              //!< The V axis
    //     povx,           //!< The X axis of the point-of-view hat
    //     povy,           //!< The Y axis of the point-of-view hat

    //     COUNT           //!< Keep last -- the total number of joystick axes
    // };

    enum class hat_pos {
        unknown,
        centered,           //!<
        up,                 //!<
        right_up,           //!<
        right,              //!<
        right_down,         //!<
        down,               //!<
        left_down,          //!<
        left,               //!<
        left_up,            //!<
        COUNT
    };

    enum class button_action {
        down,       //!< User pressed down a button on the joystick
        up,         //!< User released a button on the joystick
        COUNT
    };

    static std::string to_name (joystick::type t);
    static std::string to_name (joystick::hat_pos pos);
    static std::string to_name (joystick::button_action action);

    type                        tp              = type::axis;           //!< Joystick event type
    std::uint32_t               id              = 0;                    //!< The instance id of the joystick that reported the event
    std::uint8_t                type_index      = 0;                    //!< Index of the axis, ball or hat that moved or button activated
    std::int16_t                axis_value      = 0;                    //!< The current position of the axis (range: -32768 to 32767)
    std::int16_t                ball_xrel       = 0;                    //!< The relative joyball motion in the X direction (range: -32768 to 32767)
    std::int16_t                ball_yrel       = 0;                    //!< The relative joyball motion in the Y direction (range: -32768 to 32767)
    joystick::hat_pos           hpos            = hat_pos::centered;    //!< Hat position
    joystick::button_action     btn_action      = button_action::down;  //!< Button pressed or released
};




// ////////////////////////////////////////////////////////////
// /// \brief Constants related to joysticks capabilities
// ///
// ////////////////////////////////////////////////////////////
// // NOLINTBEGIN(readability-identifier-naming)
// static constexpr std::uint8_t count         {8};    //!< Maximum number of supported joysticks
// static constexpr std::uint8_t button_count  {32};   //!< Maximum number of supported buttons
// static constexpr std::uint8_t axis_count    {8};    //!< Maximum number of supported axes
// // NOLINTEND(readability-identifier-naming)


} //END namespace cpaf::gui::events
