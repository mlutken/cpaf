#pragma once

#include <chrono>
#include <string>
#include <variant>
#include <cpaf_libs/gui/events/base_events.h>
#include <cpaf_libs/gui/events/display_events.h>
#include <cpaf_libs/gui/events/joystick_events.h>
#include <cpaf_libs/gui/events/keyboard_events.h>
#include <cpaf_libs/gui/events/mouse_events.h>
#include <cpaf_libs/gui/events/sensor_events.h>
#include <cpaf_libs/gui/events/touch_events.h>

namespace cpaf::gui::events {

/** @brief Main event class
*
*/
struct event {
    using variant_type = std::variant<
        events::none,
        events::application,
        events::window,
        events::display,
        events::keyboard,
        events::mouse,
        events::joystick,
        events::touch,
        events::sensor
        >;

    static events::event    create_none()   { return events::event{events::none{}};     }

    template <class EVENT_CLASS_NAME>
    static events::event    create()        { return events::event{EVENT_CLASS_NAME{}}; }

    event() = default;

    template <class EVENT_CLASS_NAME>
    explicit event(const EVENT_CLASS_NAME& src_type) : instance{src_type} {}

    template <class EVENT_CLASS_NAME>
    event& operator=(const EVENT_CLASS_NAME& src_type) {
        this->instance = src_type;
        return *this;
    }

    template <class EVENT_CLASS_NAME>
    bool    is () const { return as<EVENT_CLASS_NAME>() != nullptr; }

    bool    is_none     () const { return as<events::none>() != nullptr; }

    template <class EVENT_CLASS_NAME>
    const EVENT_CLASS_NAME* as() const { return std::get_if<EVENT_CLASS_NAME>(&instance);}

    template <class EVENT_CLASS_NAME>
    EVENT_CLASS_NAME*       as()       { return std::get_if<EVENT_CLASS_NAME>(&instance);}

    // -------------------
    // --- Member data ---
    // -------------------
private:
    variant_type    instance    {events::none{}};           //!< The actual event object of type 'type' - an std::variant
};


/** @brief Enumeration of the different types of events
     *
    */
// enum class type : std::uint8_t
// {
//     none,                       //!< No event
//     window,                     //!< Window event
//     display,                    //!< Display/screen / main window event
//     keyboard,                   //!< Keyboard event
//     mouse,                      //!< Mouse event
//     joystick,                   //!< Joystick event
//     touch,                      //!< Touch event
//     sensor,                     //!< Sensor event
//     //user,                       //!< user event

//     COUNT                       //!< Keep last -- the total number of event types
// };


// event::type     type        {event::type::none};        //!< Type of the event

// static events::event    create() { return events::event{EVENT_CLASS_NAME(), event::type::none}; }
// bool    is_type (events::event::type evt_type) const { return evt_type == type; }


} //END namespace cpaf::gui::events



