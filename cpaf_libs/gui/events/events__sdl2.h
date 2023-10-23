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

    event                   create_none             () const { return event{events::none()}; }
    event                   convert_from_app        (const SDL_Event& sdl_app_event) const;

    event                   convert_from_window     (const SDL_WindowEvent& sdl_window_event) const;


    // -------------------------------------------
    // --- PRIVATE STATIC: Classify functions  ---
    // -------------------------------------------
    static bool                             is_sdl_app_event    (SDL_EventType sdl_event_type);


    // ------------------------------------------------------------
    // --- STATIC application: Event type conversion functions  ---
    // ------------------------------------------------------------
    static cpaf::gui::events::application::type     to_application_type (SDL_EventType sdl_event_type);

    // -------------------------------------------------------
    // --- STATIC window: Event type conversion functions  ---
    // -------------------------------------------------------
    static cpaf::gui::events::window::type          to_window_type      (SDL_WindowEventID sdl_window_event_id);

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
