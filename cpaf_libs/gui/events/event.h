#pragma once

#include <chrono>
#include <string>
#include <variant>
#include <cpaf_libs/gui/events/display_events.h>
#include <cpaf_libs/gui/events/joystick_events.h>
#include <cpaf_libs/gui/events/keyboard_events.h>
#include <cpaf_libs/gui/events/mouse_events.h>
#include <cpaf_libs/gui/events/sensor_events.h>
#include <cpaf_libs/gui/events/touch_events.h>

namespace cpaf::gui {



struct event {

    using time_point = std::chrono::steady_clock::time_point;

    ////////////////////////////////////////////////////////////
    /// \brief Main window Size events parameters (Resized)
    ///
    ////////////////////////////////////////////////////////////
    struct resize_t
    {
        std::uint32_t width;  //!< New width, in pixels
        std::uint32_t height; //!< New height, in pixels
    };

    ////////////////////////////////////////////////////////////
    /// \brief Main window Size events parameters (Resized)
    ///
    ////////////////////////////////////////////////////////////
    struct orientation_t
    {
        std::uint32_t           width;          //!< New width, in pixels
        std::uint32_t           height;         //!< New height, in pixels
        display::orientation    orientation;    //!< New main window orientation
    };

    ////////////////////////////////////////////////////////////
    /// \brief Keyboard event parameters (KeyPressed, KeyReleased)
    ///
    ////////////////////////////////////////////////////////////
    struct key_t
    {
        kb::key            code;     //!< Code of the key that has been pressed
        kb::scan_code      scancode; //!< Physical code of the key that has been pressed
        bool               alt;      //!< Is the Alt key pressed?
        bool               control;  //!< Is the Control key pressed?
        bool               shift;    //!< Is the Shift key pressed?
        bool               system;   //!< Is the System key pressed?
    };

    /////////////////////////////////////////////////////////////////
    /// \brief Char event parameters (TextEntered) / SFML text event
    ///
    /////////////////////////////////////////////////////////////////
    struct char_t
    {
        std::uint32_t unicode; //!< UTF-32 Unicode value of the character
    };

    ////////////////////////////////////////////////////////////////
    /// \brief Charevent parameters (TextEntered) / SFML text event
    ///
    ////////////////////////////////////////////////////////////////
    struct text_t
    {
        std::string text; //!< Text input
    };

    ////////////////////////////////////////////////////////////
    /// \brief Mouse move event parameters (MouseMoved)
    ///
    ////////////////////////////////////////////////////////////
    struct mouse_move_t
    {
        std::int32_t x; //!< X position of the mouse pointer, relative to the left of the owner window
        std::int32_t y; //!< Y position of the mouse pointer, relative to the top of the owner window
    };

    ////////////////////////////////////////////////////////////
    /// \brief Mouse buttons events parameters
    ///        (MouseButtonPressed, MouseButtonReleased)
    ///
    ////////////////////////////////////////////////////////////
    struct mouse_button_t
    {
        std::int32_t    x;      //!< X position of the mouse pointer, relative to the left of the owner window
        std::int32_t    y;      //!< Y position of the mouse pointer, relative to the top of the owner window
        mouse::button   button; //!< Code of the button that has been pressed
    };

    ////////////////////////////////////////////////////////////
    /// \brief Mouse wheel events parameters (Mouse Wheel - Scrolled)
    ///
    ////////////////////////////////////////////////////////////
    struct mouse_wheel_t
    {
        std::int32_t    x;      //!< X position of the mouse pointer, relative to the left of the owner window
        std::int32_t    y;      //!< Y position of the mouse pointer, relative to the top of the owner window
        mouse::wheel    wheel;  //!< Which wheel (for mice with multiple ones)
        float           delta;  //!< Wheel offset (positive is up/left, negative is down/right). High-precision mice may use non-integral offsets.
    };

    ////////////////////////////////////////////////////////////
    /// \brief Joystick connection events parameters
    ///        (JoystickConnected, JoystickDisconnected)
    ///
    ////////////////////////////////////////////////////////////
    struct joystick_connect_t
    {
        std::uint32_t joystick_id; //!< Index of the joystick (in range [0 .. Joystick::Count - 1])
    };

    ////////////////////////////////////////////////////////////
    /// \brief Joystick axis move event parameters (JoystickMoved)
    ///
    ////////////////////////////////////////////////////////////
    struct joystick_move_t
    {
        std::uint32_t   joystickId; //!< Index of the joystick (in range [0 .. Joystick::Count - 1])
        joystick::axis  axis;       //!< Axis on which the joystick moved
        float           position;   //!< New position on the axis (in range [-100 .. 100])
    };

    ////////////////////////////////////////////////////////////
    /// \brief Joystick buttons events parameters
    ///        (JoystickButtonPressed, JoystickButtonReleased)
    ///
    ////////////////////////////////////////////////////////////
    struct joystick_button_t
    {
        std::uint32_t joystick_id; //!< Index of the joystick (in range [0 .. Joystick::Count - 1])
        std::uint32_t button; //!< Index of the button that has been pressed (in range [0 .. Joystick::ButtonCount - 1])
    };

    ////////////////////////////////////////////////////////////
    /// \brief Touch events parameters (TouchBegan, TouchMoved, TouchEnded)
    ///
    ////////////////////////////////////////////////////////////
    struct touch_t
    {
        std::uint32_t   finger; //!< Index of the finger in case of multi-touch events
        std::int32_t    x;      //!< X position of the touch, relative to the left of the owner window
        std::int32_t    y;      //!< Y position of the touch, relative to the top of the owner window
    };

    ////////////////////////////////////////////////////////////
    /// \brief Sensor event parameters (SensorChanged)
    ///
    ////////////////////////////////////////////////////////////
    struct sensor_t
    {
        cpaf::gui::sensor::type type; //!< Type of the sensor
        float        x;    //!< Current value of the sensor on X axis
        float        y;    //!< Current value of the sensor on Y axis
        float        z;    //!< Current value of the sensor on Z axis
    };

    ////////////////////////////////////////////////////////////
    /// \brief Enumeration of the different types of events
    ///
    ////////////////////////////////////////////////////////////
    enum class type : std::uint8_t
    {
        closed,                     //!< The window requested to be closed (no data)
        resized,                    //!< The window was resized (data in event.size)
        orientation_changed,        //!< The window was resized (data in event.size)
        lost_focus,                 //!< The window lost the focus (no data)
        gained_focus,               //!< The window gained the focus (no data)
        text_entered,               //!< A character was entered (data in event.text)
        key_pressed,                //!< A key was pressed (data in event.key)
        key_released,               //!< A key was released (data in event.key)
        mouse_wheel,                //!< The mouse wheel was scrolled (data in event.mouseWheelScroll)
        mouse_button_pressed,       //!< A mouse button was pressed (data in event.mouseButton)
        mouse_button_released,      //!< A mouse button was released (data in event.mouseButton)
        mouse_moved,                //!< The mouse cursor moved (data in event.mouseMove)
        mouse_entered,              //!< The mouse cursor entered the area of the window (no data)
        mouse_left,                 //!< The mouse cursor left the area of the window (no data)
        joystick_button_pressed,    //!< A joystick button was pressed (data in event.joystickButton)
        joystick_button_released,   //!< A joystick button was released (data in event.joystickButton)
        joystick_moved,             //!< The joystick moved along an axis (data in event.joystickMove)
        joystick_connected,         //!< A joystick was connected (data in event.joystickConnect)
        joystick_disconnected,      //!< A joystick was disconnected (data in event.joystickConnect)
        touch_began,                //!< A touch event began (data in event.touch)
        touch_moved,                //!< A touch moved (data in event.touch)
        touch_ended,                //!< A touch event ended (data in event.touch)
        sensor_changed,             //!< A sensor value changed (data in event.sensor)

        COUNT                       //!< Keep last -- the total number of event types
    };

    using event_data_t = std::variant<
        resize_t,
        orientation_t,
        key_t,
        char_t,
        text_t
        >;
    // -------------------
    // --- Member data ---
    // -------------------

    event_data_t    var_value;
    time_point      timestamp;
    type            type;  //!< Type of the event



    union
    {
        resize_t        resize;         //!< Size event parameters (Event::Resized)
        orientation_t   orientation;
        key_t           key;              //!< Key event parameters (Event::KeyPressed, Event::KeyReleased)
        char_t          ch;
        text_t          text;

        // TextEvent      text;      //!< Text event parameters (Event::TextEntered)
        // MouseMoveEvent mouseMove; //!< Mouse move event parameters (Event::MouseMoved)
        // MouseButtonEvent mouseButton; //!< Mouse button event parameters (Event::MouseButtonPressed, Event::MouseButtonReleased)
        // MouseWheelScrollEvent mouseWheelScroll; //!< Mouse wheel event parameters (Event::MouseWheelScrolled)
        // JoystickMoveEvent     joystickMove;     //!< Joystick move event parameters (Event::JoystickMoved)
        // JoystickButtonEvent joystickButton; //!< Joystick button event parameters (Event::JoystickButtonPressed, Event::JoystickButtonReleased)
        // JoystickConnectEvent joystickConnect; //!< Joystick (dis)connect event parameters (Event::JoystickConnected, Event::JoystickDisconnected)
        // TouchEvent  touch;  //!< Touch events parameters (Event::TouchBegan, Event::TouchMoved, Event::TouchEnded)
        // SensorEvent sensor; //!< Sensor event parameters (Event::SensorChanged)
    };

};


////////////////////////////////////////////////////////////
/// \brief Defines a system event and its parameters
///
////////////////////////////////////////////////////////////
struct Event
{


    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////

};



} //END namespace cpaf::gui



