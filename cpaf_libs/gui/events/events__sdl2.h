#pragma once

#include <config/cpaf_platform_definitions.h>


#if (CPAF_GUI_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/gui/base/events__sdl2.h when CPAF_GUI_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

#include <SDL2/SDL.h>
#include <cpaf_libs/gui/events/base_events.h>
#include <cpaf_libs/gui/events/event.h>

namespace cpaf::gui::events {

/** @brief Create gui::events::event from SDL_Event
 *
*/

class events_sdl {
public:
    event                   convert_event           (const SDL_Event& sdl_event) const;
private:

    event                   create_none                 () const { return event{events::none()}; }
    event                   convert_from_app            (const SDL_Event& sdl_app_event) const;
    event                   convert_from_window         (const SDL_WindowEvent& sdl_window_event) const;
    event                   convert_from_mouse_motion   (const SDL_MouseMotionEvent& sdl_mouse_event) const;
    event                   convert_from_mouse_button   (const SDL_MouseButtonEvent& sdl_mouse_event) const;
    event                   convert_from_mouse_wheel    (const SDL_MouseWheelEvent& sdl_mouse_event) const;
    event                   convert_from_touch          (const SDL_TouchFingerEvent& sdl_touch_event) const;
    event                   convert_from_keyboard       (const SDL_KeyboardEvent& sdl_keyboard_event) const;
    event                   convert_from_display        (const SDL_DisplayEvent& sdl_display_event) const;


    // -------------------------------------------
    // --- PRIVATE STATIC: Classify functions  ---
    // -------------------------------------------
    static bool                                     is_sdl_app_event    (SDL_EventType sdl_event_type);


    // ------------------------------------------------------------
    // --- STATIC application: Event type conversion functions  ---
    // ------------------------------------------------------------
    static cpaf::gui::events::application::type     to_application_type (SDL_EventType sdl_event_type);

    // -------------------------------------------------------
    // --- STATIC window: Event type conversion functions  ---
    // -------------------------------------------------------
    static cpaf::gui::events::window::type          to_window_type      (SDL_WindowEventID sdl_window_event_id);


    // ------------------------------------------------------
    // --- STATIC mouse: Event type conversion functions  ---
    // ------------------------------------------------------
    static mouse::button                            to_mouse_button     (uint8_t sdl_button);

    // events::none,
    //     events::window,
    //     events::display,
    //     events::keyboard,
    //     events::mouse,
    //     events::joystick,
    //     events::touch,
    //     events::sensor

};



} //END namespace cpaf::gui::events
