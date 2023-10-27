#include "events__sdl2.h"

#include <unordered_map>
#include <cpaf_libs/utils/cpaf_stl_utils.h>


namespace cpaf::gui::events {


// SDL_Event event{};
// while (SDL_PollEvent(&event) == 1) {
//     /



// SDL_EventType

event events_sdl::convert_event(const SDL_Event& sdl_event) const
{
    if (events_sdl::is_sdl_app_event(static_cast<SDL_EventType>(sdl_event.type))) {
        return convert_from_app(sdl_event);
    }

    switch( sdl_event.type ) {
    case SDL_WINDOWEVENT:
        return convert_from_window(sdl_event.window); break;
    case SDL_MOUSEMOTION:
        return convert_from_mouse_motion(sdl_event.motion); break;
    default:
        break;
    }

    return create_none();
}

event events_sdl::convert_from_app(const SDL_Event& sdl_app_event) const
{
    events::application ev;
    ev.tp = to_application_type(static_cast<SDL_EventType>(sdl_app_event.type));
    return events::event{ev};
}



event events_sdl::convert_from_window(const SDL_WindowEvent& sdl_window_event) const
{
    events::window ev;

    ev.tp = events_sdl::to_window_type(static_cast<SDL_WindowEventID>(sdl_window_event.event));
    ev.width = sdl_window_event.data1;
    ev.height = sdl_window_event.data2;
    return event{ev};
}

event events_sdl::convert_from_mouse_motion(const SDL_MouseMotionEvent& sdl_mouse_event) const
{
    events::mouse ev;
    ev.tp = mouse::type::move;
    ev.x = sdl_mouse_event.x;
    ev.y = sdl_mouse_event.y;
    ev.xrel = sdl_mouse_event.xrel;
    ev.yrel = sdl_mouse_event.yrel;
    return event{ev};
}

event events_sdl::convert_from_mouse_button(const SDL_MouseButtonEvent& sdl_mouse_event) const
{
    events::mouse ev;
    ev.tp = sdl_mouse_event.state == SDL_PRESSED ? mouse::type::button_down : mouse::type::button_up;
    ev.x = sdl_mouse_event.x;
    ev.y = sdl_mouse_event.y;
    ev.btn = to_mouse_button(sdl_mouse_event.button);
    return event{ev};
}

event events_sdl::convert_from_mouse_wheel(const SDL_MouseWheelEvent& sdl_mouse_event) const
{
    events::mouse ev;
    ev.tp = mouse::type::wheel;
    ev.x = sdl_mouse_event.x;
    ev.y = sdl_mouse_event.y;
    ev.btn = to_mouse_button(sdl_mouse_event.button);
    return event{ev};
}

// -------------------------------------------
// --- PRIVATE STATIC: Classify functions  ---
// -------------------------------------------

bool events_sdl::is_sdl_app_event(SDL_EventType sdl_event_type)
{
    switch (sdl_event_type) {
    case SDL_QUIT:
    case SDL_APP_TERMINATING:
    case SDL_APP_LOWMEMORY:
    case SDL_APP_WILLENTERBACKGROUND:
    case SDL_APP_DIDENTERBACKGROUND:
    case SDL_APP_WILLENTERFOREGROUND:
    case SDL_APP_DIDENTERFOREGROUND:
        return true;
        break;
    default:
        break;
    }
    return false;
}


// ------------------------------------------------------------
// --- STATIC application: Event type conversion functions  ---
// ------------------------------------------------------------
application::type events_sdl::to_application_type(SDL_EventType sdl_event_type)
{
    static const std::unordered_map<SDL_EventType, application::type> look_up = {
        {SDL_QUIT,                      application::type::user_quit                },
        {SDL_APP_TERMINATING,           application::type::terminating              },
        {SDL_APP_LOWMEMORY,             application::type::low_memory               },
        {SDL_APP_WILLENTERBACKGROUND,   application::type::will_enter_background    },
        {SDL_APP_DIDENTERBACKGROUND,    application::type::did_enter_background     },
        {SDL_APP_WILLENTERFOREGROUND,   application::type::will_enter_foreground    },
        {SDL_APP_DIDENTERFOREGROUND,    application::type::did_enter_foreground     }
    };

    return cpaf::map_get(look_up, sdl_event_type, application::type::unknown);
}

// -------------------------------------------------------
// --- STATIC window: Event type conversion functions  ---
// -------------------------------------------------------

/**
 *  @note We use a map to ensure any future changes to the SDL enum does not invalidate this conversion
*/
window::type events_sdl::to_window_type(SDL_WindowEventID sdl_window_event_id)
{
    static const std::unordered_map<SDL_WindowEventID, window::type> look_up = {
        {SDL_WINDOWEVENT_NONE,              window::type::unknown       },
        {SDL_WINDOWEVENT_SHOWN,             window::type::shown         },
        {SDL_WINDOWEVENT_HIDDEN,            window::type::hidden        },
        {SDL_WINDOWEVENT_EXPOSED,           window::type::exposed       },
        {SDL_WINDOWEVENT_MOVED,             window::type::move          },
        {SDL_WINDOWEVENT_RESIZED,           window::type::resized       },
        {SDL_WINDOWEVENT_SIZE_CHANGED,      window::type::unknown       },  // TODO: Need to figure out when SDL fires SDL_WINDOWEVENT_RESIZED vs. SDL_WINDOWEVENT_SIZE_CHANGED
        {SDL_WINDOWEVENT_MINIMIZED,         window::type::minimized     },
        {SDL_WINDOWEVENT_MAXIMIZED,         window::type::maximized     },
        {SDL_WINDOWEVENT_RESTORED,          window::type::restored      },
        {SDL_WINDOWEVENT_ENTER,             window::type::mouse_entered },
        {SDL_WINDOWEVENT_LEAVE,             window::type::mouse_left    },
        {SDL_WINDOWEVENT_FOCUS_GAINED,      window::type::focus_gained  },
        {SDL_WINDOWEVENT_FOCUS_LOST,        window::type::focus_lost    },
        {SDL_WINDOWEVENT_CLOSE,             window::type::close         },
        {SDL_WINDOWEVENT_TAKE_FOCUS,        window::type::unknown       },   // TODO: What to do with SDL_WINDOWEVENT_TAKE_FOCUS
        {SDL_WINDOWEVENT_HIT_TEST,          window::type::unknown       },   // TODO: What to do with SDL_WINDOWEVENT_TAKE_FOCUS
        {SDL_WINDOWEVENT_ICCPROF_CHANGED,   window::type::unknown       },   // TODO: What to do with SDL_WINDOWEVENT_TAKE_FOCUS
        {SDL_WINDOWEVENT_DISPLAY_CHANGED,   window::type::unknown       }    // TODO: What to do with SDL_WINDOWEVENT_TAKE_FOCUS
    };

    return cpaf::map_get(look_up, sdl_window_event_id, window::type::unknown);
}

// ------------------------------------------------------
// --- STATIC Mouse: Event type conversion functions  ---
// ------------------------------------------------------

/**
 *  @note We use a map to ensure any future changes to the SDL enum does not invalidate this conversion
*/
mouse::button events_sdl::to_mouse_button(uint8_t sdl_button)
{
    static const std::unordered_map<uint8_t, mouse::button> look_up = {
        {SDL_BUTTON_LEFT,   mouse::button::left},
        {SDL_BUTTON_MIDDLE, mouse::button::middle},
        {SDL_BUTTON_RIGHT,  mouse::button::right},
        {SDL_BUTTON_X1,     mouse::button::xbutton1},
        {SDL_BUTTON_X2,     mouse::button::xbutton2},
    };

    return cpaf::map_get(look_up, sdl_button, mouse::button::unknown);
}






// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui::events

