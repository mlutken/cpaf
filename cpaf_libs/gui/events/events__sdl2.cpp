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
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        return convert_from_mouse_button(sdl_event.button); break;
    case SDL_MOUSEWHEEL:
        return convert_from_mouse_wheel(sdl_event.wheel); break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        return convert_from_keyboard(sdl_event.key); break;
    case SDL_TEXTINPUT:
        return convert_from_text_input(sdl_event.text); break;
    case SDL_TEXTEDITING:
        return convert_from_text_editing(sdl_event.edit); break;
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
    ev.xrel = sdl_mouse_event.x;
    ev.yrel = sdl_mouse_event.y;
    ev.scroll_dir = sdl_mouse_event.direction == SDL_MOUSEWHEEL_FLIPPED ? mouse::scroll_direction::flipped : mouse::scroll_direction::normal;
    return event{ev};
}

event events_sdl::convert_from_touch(const SDL_TouchFingerEvent& sdl_touch_event) const
{
    events::touch ev;
    switch (sdl_touch_event.type) {
    case SDL_FINGERDOWN:    ev.tp = touch::type::down;  break;
    case SDL_FINGERUP:      ev.tp = touch::type::up;    break;
    case SDL_FINGERMOTION:  ev.tp = touch::type::move;  break;
    default:    ev.tp = touch::type::unknown;           break;
    }

    ev.x = sdl_touch_event.x;
    ev.y = sdl_touch_event.y;
    ev.delta_x = sdl_touch_event.dx;
    ev.delta_y = sdl_touch_event.dy;
    ev.finger = sdl_touch_event.fingerId;
    ev.pressure = sdl_touch_event.pressure;
    return event{ev};
}

event events_sdl::convert_from_keyboard(const SDL_KeyboardEvent& sdl_keyboard_event) const
{
    events::keyboard ev;
    if (sdl_keyboard_event.type == SDL_KEYDOWN)        { ev.tp = keyboard::type::pressed; }
    else if (sdl_keyboard_event.type == SDL_KEYUP)     { ev.tp = keyboard::type::released; }

    if (sdl_keyboard_event.state == SDL_PRESSED)       { ev.state = keyboard::type::pressed; }
    else if (sdl_keyboard_event.state == SDL_RELEASED) { ev.state = keyboard::type::released; }

    ev.sc = to_scan_code(sdl_keyboard_event.keysym.scancode);
    ev.k = to_key(sdl_keyboard_event.keysym.sym);

    ev.alt = (sdl_keyboard_event.keysym.mod & KMOD_ALT) != 0;
    ev.control = (sdl_keyboard_event.keysym.mod & KMOD_CTRL) != 0;
    ev.shift = (sdl_keyboard_event.keysym.mod & KMOD_SHIFT) != 0;

    ev.repeat = sdl_keyboard_event.repeat;
    return event{ev};
}

event events_sdl::convert_from_text_input(const SDL_TextInputEvent& sdl_text_input_event) const
{
    events::keyboard ev;
    ev.tp = keyboard::type::text_input;
    ev.text = sdl_text_input_event.text;
    return event{ev};
}

event events_sdl::convert_from_text_editing(const SDL_TextEditingEvent& sdl_text_editing_event) const
{
    events::keyboard ev;
    ev.tp = keyboard::type::text_editing;
    ev.text = sdl_text_editing_event.text;
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

keyboard::key events_sdl::to_key (SDL_Keycode sdl_key)
{
    static const std::unordered_map<SDL_Keycode, keyboard::key> look_up = {

        {SDLK_UNKNOWN, 				keyboard::key::unknown},
        {SDLK_RETURN, 				keyboard::key::enter},
        {SDLK_ESCAPE, 				keyboard::key::escape},
        {SDLK_BACKSPACE, 				keyboard::key::backspace},
        {SDLK_TAB, 				keyboard::key::tab},
        {SDLK_SPACE, 				keyboard::key::space},
        // {SDLK_EXCLAIM, 				keyboard::key::exclaim},
        // {SDLK_QUOTEDBL, 				keyboard::key::quotedbl},
        {SDLK_HASH, 				keyboard::key::hash},
        {SDLK_PERCENT, 				keyboard::key::percent},
        {SDLK_DOLLAR, 				keyboard::key::dollar},
        {SDLK_AMPERSAND, 				keyboard::key::ampersand},
        {SDLK_QUOTE, 				keyboard::key::quote},
        {SDLK_LEFTPAREN, 				keyboard::key::lparen},
        {SDLK_RIGHTPAREN, 				keyboard::key::rparen},
        {SDLK_ASTERISK, 				keyboard::key::asterisk},
        {SDLK_PLUS, 				keyboard::key::plus},
        {SDLK_COMMA, 				keyboard::key::comma},
        {SDLK_MINUS, 				keyboard::key::minus},
        {SDLK_PERIOD, 				keyboard::key::period},
        {SDLK_SLASH, 				keyboard::key::slash},
        {SDLK_0, 				keyboard::key::num0},
        {SDLK_1, 				keyboard::key::num1},
        {SDLK_2, 				keyboard::key::num2},
        {SDLK_3, 				keyboard::key::num3},
        {SDLK_4, 				keyboard::key::num4},
        {SDLK_5, 				keyboard::key::num5},
        {SDLK_6, 				keyboard::key::num6},
        {SDLK_7, 				keyboard::key::num7},
        {SDLK_8, 				keyboard::key::num8},
        {SDLK_9, 				keyboard::key::num9},
        {SDLK_COLON, 				keyboard::key::colon},
        {SDLK_SEMICOLON, 				keyboard::key::semicolon},
        {SDLK_LESS, 				keyboard::key::less},
        {SDLK_EQUALS, 				keyboard::key::equals},
        {SDLK_GREATER, 				keyboard::key::greater},
        {SDLK_QUESTION, 				keyboard::key::question},
        {SDLK_AT, 				keyboard::key::at},
        {SDLK_LEFTBRACKET, 				keyboard::key::lbracket},
        {SDLK_BACKSLASH, 				keyboard::key::backslash},
        {SDLK_RIGHTBRACKET, 				keyboard::key::rbracket},
        // {SDLK_CARET, 				keyboard::key::caret},
        {SDLK_UNDERSCORE, 				keyboard::key::underscore},
        {SDLK_BACKQUOTE, 				keyboard::key::backquote},
        {SDLK_a, 				keyboard::key::a},
        {SDLK_b, 				keyboard::key::b},
        {SDLK_c, 				keyboard::key::c},
        {SDLK_d, 				keyboard::key::d},
        {SDLK_e, 				keyboard::key::e},
        {SDLK_f, 				keyboard::key::f},
        {SDLK_g, 				keyboard::key::g},
        {SDLK_h, 				keyboard::key::h},
        {SDLK_i, 				keyboard::key::i},
        {SDLK_j, 				keyboard::key::j},
        {SDLK_k, 				keyboard::key::k},
        {SDLK_l, 				keyboard::key::l},
        {SDLK_m, 				keyboard::key::m},
        {SDLK_n, 				keyboard::key::n},
        {SDLK_o, 				keyboard::key::o},
        {SDLK_p, 				keyboard::key::p},
        {SDLK_q, 				keyboard::key::q},
        {SDLK_r, 				keyboard::key::r},
        {SDLK_s, 				keyboard::key::s},
        {SDLK_t, 				keyboard::key::t},
        {SDLK_u, 				keyboard::key::u},
        {SDLK_v, 				keyboard::key::v},
        {SDLK_w, 				keyboard::key::w},
        {SDLK_x, 				keyboard::key::x},
        {SDLK_y, 				keyboard::key::y},
        {SDLK_z, 				keyboard::key::z},
        {SDLK_CAPSLOCK, 		keyboard::key::capslock},
        {SDLK_F1, 				keyboard::key::f1},
        {SDLK_F2, 				keyboard::key::f2},
        {SDLK_F3, 				keyboard::key::f3},
        {SDLK_F4, 				keyboard::key::f4},
        {SDLK_F5, 				keyboard::key::f5},
        {SDLK_F6, 				keyboard::key::f6},
        {SDLK_F7, 				keyboard::key::f7},
        {SDLK_F8, 				keyboard::key::f8},
        {SDLK_F9, 				keyboard::key::f9},
        {SDLK_F10, 				keyboard::key::f10},
        {SDLK_F11, 				keyboard::key::f11},
        {SDLK_F12, 				keyboard::key::f12},
        {SDLK_PRINTSCREEN, 		keyboard::key::printscreen},
        {SDLK_SCROLLLOCK, 		keyboard::key::scrolllock},
        {SDLK_PAUSE, 				keyboard::key::pause},
        {SDLK_INSERT, 				keyboard::key::insert},
        {SDLK_HOME, 				keyboard::key::home},
        {SDLK_PAGEUP, 				keyboard::key::pageup},
        {SDLK_DELETE, 				keyboard::key::del},
        {SDLK_END, 				keyboard::key::end},
        {SDLK_PAGEDOWN, 				keyboard::key::pagedown},
        {SDLK_RIGHT, 				keyboard::key::right},
        {SDLK_LEFT, 				keyboard::key::left},
        {SDLK_DOWN, 				keyboard::key::down},
        {SDLK_UP, 				keyboard::key::up},
        // {SDLK_NUMLOCKCLEAR, 				keyboard::key::numlockclear},
        {SDLK_KP_DIVIDE, 				keyboard::key::numpad_divide},
        {SDLK_KP_MULTIPLY, 				keyboard::key::numpad_multiply},
        {SDLK_KP_MINUS, 				keyboard::key::numpad_minus},
        {SDLK_KP_PLUS, 				keyboard::key::numpad_plus},
        {SDLK_KP_ENTER, 				keyboard::key::numpad_enter},
        {SDLK_KP_1, 				keyboard::key::numpad1},
        {SDLK_KP_2, 				keyboard::key::numpad2},
        {SDLK_KP_3, 				keyboard::key::numpad3},
        {SDLK_KP_4, 				keyboard::key::numpad4},
        {SDLK_KP_5, 				keyboard::key::numpad5},
        {SDLK_KP_6, 				keyboard::key::numpad6},
        {SDLK_KP_7, 				keyboard::key::numpad7},
        {SDLK_KP_8, 				keyboard::key::numpad8},
        {SDLK_KP_9, 				keyboard::key::numpad9},
        {SDLK_KP_0, 				keyboard::key::numpad0},
        {SDLK_KP_PERIOD, 			keyboard::key::numpad_period},
        // {SDLK_APPLICATION, 				keyboard::key::application},
        // {SDLK_POWER, 				keyboard::key::power},
        {SDLK_KP_EQUALS, 				keyboard::key::numpad_equals},
        {SDLK_F13, 				keyboard::key::f13},
        {SDLK_F14, 				keyboard::key::f14},
        {SDLK_F15, 				keyboard::key::f15},
        {SDLK_F16, 				keyboard::key::f16},
        {SDLK_F17, 				keyboard::key::f17},
        {SDLK_F18, 				keyboard::key::f18},
        {SDLK_F19, 				keyboard::key::f19},
        {SDLK_F20, 				keyboard::key::f20},
        {SDLK_F21, 				keyboard::key::f21},
        {SDLK_F22, 				keyboard::key::f22},
        {SDLK_F23, 				keyboard::key::f23},
        {SDLK_F24, 				keyboard::key::f24},
        // {SDLK_EXECUTE, 				keyboard::key::execute},
        // {SDLK_HELP, 				keyboard::key::help},
        {SDLK_MENU, 				keyboard::key::menu},
        // {SDLK_SELECT, 				keyboard::key::select},
        // {SDLK_STOP, 				keyboard::key::stop},
        // {SDLK_AGAIN, 				keyboard::key::again},
        // {SDLK_UNDO, 				keyboard::key::undo},
        // {SDLK_CUT, 				keyboard::key::cut},
        // {SDLK_COPY, 				keyboard::key::copy},
        // {SDLK_PASTE, 				keyboard::key::paste},
        // {SDLK_FIND, 				keyboard::key::find},
        // {SDLK_MUTE, 				keyboard::key::mute},
        // {SDLK_VOLUMEUP, 				keyboard::key::volumeup},
        // {SDLK_VOLUMEDOWN, 				keyboard::key::volumedown},
        {SDLK_KP_COMMA, 				keyboard::key::numpad_comma},
        // {SDLK_KP_EQUALSAS400, 				keyboard::key::kp_equalsas400},
        // {SDLK_ALTERASE, 				keyboard::key::alterase},
        // {SDLK_SYSREQ, 				keyboard::key::sysreq},
        // {SDLK_CANCEL, 				keyboard::key::cancel},
        // {SDLK_CLEAR, 				keyboard::key::clear},
        // {SDLK_PRIOR, 				keyboard::key::prior},
        // {SDLK_RETURN2, 				keyboard::key::return2},
        // {SDLK_SEPARATOR, 				keyboard::key::separator},
        // {SDLK_OUT, 				keyboard::key::out},
        // {SDLK_OPER, 				keyboard::key::oper},
        // {SDLK_CLEARAGAIN, 				keyboard::key::clearagain},
        // {SDLK_CRSEL, 				keyboard::key::crsel},
        // {SDLK_EXSEL, 				keyboard::key::exsel},
        // {SDLK_KP_00, 				keyboard::key::kp_00},
        // {SDLK_KP_000, 				keyboard::key::kp_000},
        // {SDLK_THOUSANDSSEPARATOR, 				keyboard::key::thousandsseparator},
        // {SDLK_DECIMALSEPARATOR, 				keyboard::key::decimalseparator},
        // {SDLK_CURRENCYUNIT, 				keyboard::key::currencyunit},
        // {SDLK_CURRENCYSUBUNIT, 				keyboard::key::currencysubunit},
        // {SDLK_KP_LEFTPAREN, 				keyboard::key::kp_leftparen},
        // {SDLK_KP_RIGHTPAREN, 				keyboard::key::kp_rightparen},
        // {SDLK_KP_LEFTBRACE, 				keyboard::key::kp_leftbrace},
        // {SDLK_KP_RIGHTBRACE, 				keyboard::key::kp_rightbrace},
        // {SDLK_KP_TAB, 				keyboard::key::kp_tab},
        // {SDLK_KP_BACKSPACE, 				keyboard::key::kp_backspace},
        // {SDLK_KP_A, 				keyboard::key::kp_a},
        // {SDLK_KP_B, 				keyboard::key::kp_b},
        // {SDLK_KP_C, 				keyboard::key::kp_c},
        // {SDLK_KP_D, 				keyboard::key::kp_d},
        // {SDLK_KP_E, 				keyboard::key::kp_e},
        // {SDLK_KP_F, 				keyboard::key::kp_f},
        // {SDLK_KP_XOR, 				keyboard::key::kp_xor},
        // {SDLK_KP_POWER, 				keyboard::key::kp_power},
        // {SDLK_KP_PERCENT, 				keyboard::key::kp_percent},
        // {SDLK_KP_LESS, 				keyboard::key::kp_less},
        // {SDLK_KP_GREATER, 				keyboard::key::kp_greater},
        // {SDLK_KP_AMPERSAND, 				keyboard::key::kp_ampersand},
        // {SDLK_KP_DBLAMPERSAND, 				keyboard::key::kp_dblampersand},
        // {SDLK_KP_VERTICALBAR, 				keyboard::key::kp_verticalbar},
        // {SDLK_KP_DBLVERTICALBAR, 				keyboard::key::kp_dblverticalbar},
        // {SDLK_KP_COLON, 				keyboard::key::kp_colon},
        // {SDLK_KP_HASH, 				keyboard::key::kp_hash},
        // {SDLK_KP_SPACE, 				keyboard::key::kp_space},
        // {SDLK_KP_AT, 				keyboard::key::kp_at},
        // {SDLK_KP_EXCLAM, 				keyboard::key::kp_exclam},
        // {SDLK_KP_MEMSTORE, 				keyboard::key::kp_memstore},
        // {SDLK_KP_MEMRECALL, 				keyboard::key::kp_memrecall},
        // {SDLK_KP_MEMCLEAR, 				keyboard::key::kp_memclear},
        // {SDLK_KP_MEMADD, 				keyboard::key::kp_memadd},
        // {SDLK_KP_MEMSUBTRACT, 				keyboard::key::kp_memsubtract},
        // {SDLK_KP_MEMMULTIPLY, 				keyboard::key::kp_memmultiply},
        // {SDLK_KP_MEMDIVIDE, 				keyboard::key::kp_memdivide},
        // {SDLK_KP_PLUSMINUS, 				keyboard::key::kp_plusminus},
        // {SDLK_KP_CLEAR, 				keyboard::key::kp_clear},
        // {SDLK_KP_CLEARENTRY, 				keyboard::key::kp_clearentry},
        // {SDLK_KP_BINARY, 				keyboard::key::kp_binary},
        // {SDLK_KP_OCTAL, 				keyboard::key::kp_octal},
        // {SDLK_KP_DECIMAL, 				keyboard::key::kp_decimal},
        // {SDLK_KP_HEXADECIMAL, 				keyboard::key::kp_hexadecimal},
        {SDLK_LCTRL, 				keyboard::key::lcontrol},
        {SDLK_LSHIFT, 				keyboard::key::lshift},
        {SDLK_LALT, 				keyboard::key::lalt},
        // {SDLK_LGUI, 				keyboard::key::lgui},
        {SDLK_RCTRL, 				keyboard::key::rcontrol},
        {SDLK_RSHIFT, 				keyboard::key::rshift},
        {SDLK_RALT, 				keyboard::key::ralt},
        // {SDLK_RGUI, 				keyboard::key::rgui},
        // {SDLK_MODE, 				keyboard::key::mode},
        // {SDLK_AUDIONEXT, 				keyboard::key::audionext},
        // {SDLK_AUDIOPREV, 				keyboard::key::audioprev},
        // {SDLK_AUDIOSTOP, 				keyboard::key::audiostop},
        // {SDLK_AUDIOPLAY, 				keyboard::key::audioplay},
        // {SDLK_AUDIOMUTE, 				keyboard::key::audiomute},
        // {SDLK_MEDIASELECT, 				keyboard::key::mediaselect},
        // {SDLK_WWW, 				keyboard::key::www},
        // {SDLK_MAIL, 				keyboard::key::mail},
        // {SDLK_CALCULATOR, 				keyboard::key::calculator},
        // {SDLK_COMPUTER, 				keyboard::key::computer},
        // {SDLK_AC_SEARCH, 				keyboard::key::ac_search},
        // {SDLK_AC_HOME, 				keyboard::key::ac_home},
        // {SDLK_AC_BACK, 				keyboard::key::ac_back},
        // {SDLK_AC_FORWARD, 				keyboard::key::ac_forward},
        // {SDLK_AC_STOP, 				keyboard::key::ac_stop},
        // {SDLK_AC_REFRESH, 				keyboard::key::ac_refresh},
        // {SDLK_AC_BOOKMARKS, 				keyboard::key::ac_bookmarks},
        // {SDLK_BRIGHTNESSDOWN, 				keyboard::key::brightnessdown},
        // {SDLK_BRIGHTNESSUP, 				keyboard::key::brightnessup},
        // {SDLK_DISPLAYSWITCH, 				keyboard::key::displayswitch},
        // {SDLK_KBDILLUMTOGGLE, 				keyboard::key::kbdillumtoggle},
        // {SDLK_KBDILLUMDOWN, 				keyboard::key::kbdillumdown},
        // {SDLK_KBDILLUMUP, 				keyboard::key::kbdillumup},
        // {SDLK_EJECT, 				keyboard::key::eject},
        // {SDLK_SLEEP, 				keyboard::key::sleep},
        // {SDLK_APP1, 				keyboard::key::app1},
        // {SDLK_APP2, 				keyboard::key::app2},
        // {SDLK_AUDIOREWIND, 				keyboard::key::audiorewind},
        // {SDLK_AUDIOFASTFORWARD, 				keyboard::key::audiofastforward},
        // {SDLK_SOFTLEFT, 				keyboard::key::softleft},
        // {SDLK_SOFTRIGHT, 				keyboard::key::softright},
        // {SDLK_CALL, 				keyboard::key::call}

    };
    return cpaf::map_get(look_up, sdl_key, keyboard::key::unknown);

}


keyboard::scan_code events_sdl::to_scan_code(SDL_Scancode sdl_sc)
{
    static const std::unordered_map<SDL_Scancode, keyboard::scan_code> look_up = {

    {SDL_SCANCODE_UNKNOWN, 			keyboard::scan_code::unknown},
    {SDL_SCANCODE_A, 				keyboard::scan_code::a},
    {SDL_SCANCODE_B, 				keyboard::scan_code::b},
    {SDL_SCANCODE_C, 				keyboard::scan_code::c},
    {SDL_SCANCODE_D, 				keyboard::scan_code::d},
    {SDL_SCANCODE_E, 				keyboard::scan_code::e},
    {SDL_SCANCODE_F, 				keyboard::scan_code::f},
    {SDL_SCANCODE_G, 				keyboard::scan_code::g},
    {SDL_SCANCODE_H, 				keyboard::scan_code::h},
    {SDL_SCANCODE_I, 				keyboard::scan_code::i},
    {SDL_SCANCODE_J, 				keyboard::scan_code::j},
    {SDL_SCANCODE_K, 				keyboard::scan_code::k},
    {SDL_SCANCODE_L, 				keyboard::scan_code::l},
    {SDL_SCANCODE_M, 				keyboard::scan_code::m},
    {SDL_SCANCODE_N, 				keyboard::scan_code::n},
    {SDL_SCANCODE_O, 				keyboard::scan_code::o},
    {SDL_SCANCODE_P, 				keyboard::scan_code::p},
    {SDL_SCANCODE_Q, 				keyboard::scan_code::q},
    {SDL_SCANCODE_R, 				keyboard::scan_code::r},
    {SDL_SCANCODE_S, 				keyboard::scan_code::s},
    {SDL_SCANCODE_T, 				keyboard::scan_code::t},
    {SDL_SCANCODE_U, 				keyboard::scan_code::u},
    {SDL_SCANCODE_V, 				keyboard::scan_code::v},
    {SDL_SCANCODE_W, 				keyboard::scan_code::w},
    {SDL_SCANCODE_X, 				keyboard::scan_code::x},
    {SDL_SCANCODE_Y, 				keyboard::scan_code::y},
    {SDL_SCANCODE_Z, 				keyboard::scan_code::z},
    {SDL_SCANCODE_1, 				keyboard::scan_code::num1},
    {SDL_SCANCODE_2, 				keyboard::scan_code::num2},
    {SDL_SCANCODE_3, 				keyboard::scan_code::num3},
    {SDL_SCANCODE_4, 				keyboard::scan_code::num4},
    {SDL_SCANCODE_5, 				keyboard::scan_code::num5},
    {SDL_SCANCODE_6, 				keyboard::scan_code::num6},
    {SDL_SCANCODE_7, 				keyboard::scan_code::num7},
    {SDL_SCANCODE_8, 				keyboard::scan_code::num8},
    {SDL_SCANCODE_9, 				keyboard::scan_code::num9},
    {SDL_SCANCODE_0, 				keyboard::scan_code::num0},
    {SDL_SCANCODE_RETURN, 			keyboard::scan_code::enter},
    {SDL_SCANCODE_ESCAPE, 			keyboard::scan_code::escape},
    {SDL_SCANCODE_BACKSPACE, 		keyboard::scan_code::backspace},
    {SDL_SCANCODE_TAB, 				keyboard::scan_code::tab},
    {SDL_SCANCODE_SPACE, 				keyboard::scan_code::space},
    {SDL_SCANCODE_MINUS, 				keyboard::scan_code::hyphen},
    {SDL_SCANCODE_EQUALS, 				keyboard::scan_code::equals},
    {SDL_SCANCODE_LEFTBRACKET, 				keyboard::scan_code::lbracket},
    {SDL_SCANCODE_RIGHTBRACKET, 				keyboard::scan_code::rbracket},
    {SDL_SCANCODE_BACKSLASH, 				keyboard::scan_code::backslash},
    // {SDL_SCANCODE_NONUSHASH, 				keyboard::scan_code::nonushash},
    {SDL_SCANCODE_SEMICOLON, 				keyboard::scan_code::semicolon},
    {SDL_SCANCODE_APOSTROPHE, 				keyboard::scan_code::apostrophe},
    {SDL_SCANCODE_GRAVE, 				keyboard::scan_code::grave},
    {SDL_SCANCODE_COMMA, 				keyboard::scan_code::comma},
    {SDL_SCANCODE_PERIOD, 				keyboard::scan_code::period},
    {SDL_SCANCODE_SLASH, 				keyboard::scan_code::slash},
    {SDL_SCANCODE_CAPSLOCK, 				keyboard::scan_code::capslock},
    {SDL_SCANCODE_F1, 				keyboard::scan_code::f1},
    {SDL_SCANCODE_F2, 				keyboard::scan_code::f2},
    {SDL_SCANCODE_F3, 				keyboard::scan_code::f3},
    {SDL_SCANCODE_F4, 				keyboard::scan_code::f4},
    {SDL_SCANCODE_F5, 				keyboard::scan_code::f5},
    {SDL_SCANCODE_F6, 				keyboard::scan_code::f6},
    {SDL_SCANCODE_F7, 				keyboard::scan_code::f7},
    {SDL_SCANCODE_F8, 				keyboard::scan_code::f8},
    {SDL_SCANCODE_F9, 				keyboard::scan_code::f9},
    {SDL_SCANCODE_F10, 				keyboard::scan_code::f10},
    {SDL_SCANCODE_F11, 				keyboard::scan_code::f11},
    {SDL_SCANCODE_F12, 				keyboard::scan_code::f12},
    {SDL_SCANCODE_PRINTSCREEN, 				keyboard::scan_code::printscreen},
    {SDL_SCANCODE_SCROLLLOCK, 				keyboard::scan_code::scrolllock},
    {SDL_SCANCODE_PAUSE, 				keyboard::scan_code::pause},
    {SDL_SCANCODE_INSERT, 				keyboard::scan_code::insert},
    {SDL_SCANCODE_HOME, 				keyboard::scan_code::home},
    {SDL_SCANCODE_PAGEUP, 				keyboard::scan_code::pageup},
    {SDL_SCANCODE_DELETE, 				keyboard::scan_code::del},
    {SDL_SCANCODE_END, 				keyboard::scan_code::end},
    {SDL_SCANCODE_PAGEDOWN, 				keyboard::scan_code::pagedown},
    {SDL_SCANCODE_RIGHT, 				keyboard::scan_code::right},
    {SDL_SCANCODE_LEFT, 				keyboard::scan_code::left},
    {SDL_SCANCODE_DOWN, 				keyboard::scan_code::down},
    {SDL_SCANCODE_UP, 				keyboard::scan_code::up},
    {SDL_SCANCODE_NUMLOCKCLEAR, 				keyboard::scan_code::numlockclear},
    {SDL_SCANCODE_KP_DIVIDE, 				keyboard::scan_code::numpaddivide},
    {SDL_SCANCODE_KP_MULTIPLY, 				keyboard::scan_code::numpadmultiply},
    {SDL_SCANCODE_KP_MINUS, 				keyboard::scan_code::numpadminus},
    {SDL_SCANCODE_KP_PLUS, 				keyboard::scan_code::numpadplus},
    {SDL_SCANCODE_KP_ENTER, 				keyboard::scan_code::numpadenter},
    {SDL_SCANCODE_KP_1, 				keyboard::scan_code::numpad1},
    {SDL_SCANCODE_KP_2, 				keyboard::scan_code::numpad2},
    {SDL_SCANCODE_KP_3, 				keyboard::scan_code::numpad3},
    {SDL_SCANCODE_KP_4, 				keyboard::scan_code::numpad4},
    {SDL_SCANCODE_KP_5, 				keyboard::scan_code::numpad5},
    {SDL_SCANCODE_KP_6, 				keyboard::scan_code::numpad6},
    {SDL_SCANCODE_KP_7, 				keyboard::scan_code::numpad7},
    {SDL_SCANCODE_KP_8, 				keyboard::scan_code::numpad8},
    {SDL_SCANCODE_KP_9, 				keyboard::scan_code::numpad9},
    {SDL_SCANCODE_KP_0, 				keyboard::scan_code::numpad0},
    {SDL_SCANCODE_KP_PERIOD, 				keyboard::scan_code::numpaddecimaldel},
    {SDL_SCANCODE_NONUSBACKSLASH, 				keyboard::scan_code::nonusbackslash},
    {SDL_SCANCODE_APPLICATION, 				keyboard::scan_code::application},
    // {SDL_SCANCODE_POWER, 				keyboard::scan_code::power},
    {SDL_SCANCODE_KP_EQUALS, 				keyboard::scan_code::numpadequal},
    {SDL_SCANCODE_F13, 				keyboard::scan_code::f13},
    {SDL_SCANCODE_F14, 				keyboard::scan_code::f14},
    {SDL_SCANCODE_F15, 				keyboard::scan_code::f15},
    {SDL_SCANCODE_F16, 				keyboard::scan_code::f16},
    {SDL_SCANCODE_F17, 				keyboard::scan_code::f17},
    {SDL_SCANCODE_F18, 				keyboard::scan_code::f18},
    {SDL_SCANCODE_F19, 				keyboard::scan_code::f19},
    {SDL_SCANCODE_F20, 				keyboard::scan_code::f20},
    {SDL_SCANCODE_F21, 				keyboard::scan_code::f21},
    {SDL_SCANCODE_F22, 				keyboard::scan_code::f22},
    {SDL_SCANCODE_F23, 				keyboard::scan_code::f23},
    {SDL_SCANCODE_F24, 				keyboard::scan_code::f24},
    {SDL_SCANCODE_EXECUTE, 				keyboard::scan_code::execute},
    {SDL_SCANCODE_HELP, 				keyboard::scan_code::help},
    {SDL_SCANCODE_MENU, 				keyboard::scan_code::menu},
    {SDL_SCANCODE_SELECT, 				keyboard::scan_code::select},
    {SDL_SCANCODE_STOP, 				keyboard::scan_code::stop},
    // {SDL_SCANCODE_AGAIN, 				keyboard::scan_code::again},
    {SDL_SCANCODE_UNDO, 				keyboard::scan_code::undo},
    {SDL_SCANCODE_CUT, 				keyboard::scan_code::cut},
    {SDL_SCANCODE_COPY, 				keyboard::scan_code::copy},
    {SDL_SCANCODE_PASTE, 				keyboard::scan_code::paste},
    // {SDL_SCANCODE_FIND, 				keyboard::scan_code::find},
    {SDL_SCANCODE_MUTE, 				keyboard::scan_code::volumemute},
    {SDL_SCANCODE_VOLUMEUP, 				keyboard::scan_code::volumeup},
    {SDL_SCANCODE_VOLUMEDOWN, 				keyboard::scan_code::volumedown},
    // {SDL_SCANCODE_KP_COMMA, 				keyboard::scan_code::kp_comma},
    // {SDL_SCANCODE_KP_EQUALSAS400, 				keyboard::scan_code::kp_equalsas400},
    // {SDL_SCANCODE_INTERNATIONAL1, 				keyboard::scan_code::international1},
    // {SDL_SCANCODE_INTERNATIONAL2, 				keyboard::scan_code::international2},
    // {SDL_SCANCODE_INTERNATIONAL3, 				keyboard::scan_code::international3},
    // {SDL_SCANCODE_INTERNATIONAL4, 				keyboard::scan_code::international4},
    // {SDL_SCANCODE_INTERNATIONAL5, 				keyboard::scan_code::international5},
    // {SDL_SCANCODE_INTERNATIONAL6, 				keyboard::scan_code::international6},
    // {SDL_SCANCODE_INTERNATIONAL7, 				keyboard::scan_code::international7},
    // {SDL_SCANCODE_INTERNATIONAL8, 				keyboard::scan_code::international8},
    // {SDL_SCANCODE_INTERNATIONAL9, 				keyboard::scan_code::international9},
    // {SDL_SCANCODE_LANG1, 				keyboard::scan_code::lang1},
    // {SDL_SCANCODE_LANG2, 				keyboard::scan_code::lang2},
    // {SDL_SCANCODE_LANG3, 				keyboard::scan_code::lang3},
    // {SDL_SCANCODE_LANG4, 				keyboard::scan_code::lang4},
    // {SDL_SCANCODE_LANG5, 				keyboard::scan_code::lang5},
    // {SDL_SCANCODE_LANG6, 				keyboard::scan_code::lang6},
    // {SDL_SCANCODE_LANG7, 				keyboard::scan_code::lang7},
    // {SDL_SCANCODE_LANG8, 				keyboard::scan_code::lang8},
    // {SDL_SCANCODE_LANG9, 				keyboard::scan_code::lang9},
    // {SDL_SCANCODE_ALTERASE, 				keyboard::scan_code::alterase},
    // {SDL_SCANCODE_SYSREQ, 				keyboard::scan_code::sysreq},
    // {SDL_SCANCODE_CANCEL, 				keyboard::scan_code::cancel},
    // {SDL_SCANCODE_CLEAR, 				keyboard::scan_code::clear},
    // {SDL_SCANCODE_PRIOR, 				keyboard::scan_code::prior},
    // {SDL_SCANCODE_RETURN2, 				keyboard::scan_code::return2},
    // {SDL_SCANCODE_SEPARATOR, 				keyboard::scan_code::separator},
    // {SDL_SCANCODE_OUT, 				keyboard::scan_code::out},
    // {SDL_SCANCODE_OPER, 				keyboard::scan_code::oper},
    // {SDL_SCANCODE_CLEARAGAIN, 				keyboard::scan_code::clearagain},
    // {SDL_SCANCODE_CRSEL, 				keyboard::scan_code::crsel},
    // {SDL_SCANCODE_EXSEL, 				keyboard::scan_code::exsel},
    // {SDL_SCANCODE_KP_00, 				keyboard::scan_code::kp_00},
    // {SDL_SCANCODE_KP_000, 				keyboard::scan_code::kp_000},
    // {SDL_SCANCODE_THOUSANDSSEPARATOR, 				keyboard::scan_code::thousandsseparatOR},
    // {SDL_SCANCODE_DECIMALSEPARATOR, 				keyboard::scan_code::decimalseparator},
    // {SDL_SCANCODE_CURRENCYUNIT, 				keyboard::scan_code::currencyunit},
    // {SDL_SCANCODE_CURRENCYSUBUNIT, 				keyboard::scan_code::currencysubunit},
    // {SDL_SCANCODE_KP_LEFTPAREN, 				keyboard::scan_code::kp_leftparen},
    // {SDL_SCANCODE_KP_RIGHTPAREN, 				keyboard::scan_code::kp_rightparen},
    // {SDL_SCANCODE_KP_LEFTBRACE, 				keyboard::scan_code::kp_leftbrace},
    // {SDL_SCANCODE_KP_RIGHTBRACE, 				keyboard::scan_code::kp_rightbrace},
    // {SDL_SCANCODE_KP_TAB, 				keyboard::scan_code::kp_tab},
    // {SDL_SCANCODE_KP_BACKSPACE, 				keyboard::scan_code::kp_backspace},
    // {SDL_SCANCODE_KP_A, 				keyboard::scan_code::kp_a},
    // {SDL_SCANCODE_KP_B, 				keyboard::scan_code::kp_b},
    // {SDL_SCANCODE_KP_C, 				keyboard::scan_code::kp_c},
    // {SDL_SCANCODE_KP_D, 				keyboard::scan_code::kp_d},
    // {SDL_SCANCODE_KP_E, 				keyboard::scan_code::kp_e},
    // {SDL_SCANCODE_KP_F, 				keyboard::scan_code::kp_f},
    // {SDL_SCANCODE_KP_XOR, 				keyboard::scan_code::kp_xor},
    // {SDL_SCANCODE_KP_POWER, 				keyboard::scan_code::kp_power},
    // {SDL_SCANCODE_KP_PERCENT, 				keyboard::scan_code::kp_percent},
    // {SDL_SCANCODE_KP_LESS, 				keyboard::scan_code::kp_less},
    // {SDL_SCANCODE_KP_GREATER, 				keyboard::scan_code::kp_greater},
    // {SDL_SCANCODE_KP_AMPERSAND, 				keyboard::scan_code::kp_ampersand},
    // {SDL_SCANCODE_KP_DBLAMPERSAND, 				keyboard::scan_code::kp_dblampersand},
    // {SDL_SCANCODE_KP_VERTICALBAR, 				keyboard::scan_code::kp_verticalbar},
    // {SDL_SCANCODE_KP_DBLVERTICALBAR, 				keyboard::scan_code::kp_dblverticalbaR},
    // {SDL_SCANCODE_KP_COLON, 				keyboard::scan_code::kp_colon},
    // {SDL_SCANCODE_KP_HASH, 				keyboard::scan_code::kp_hash},
    // {SDL_SCANCODE_KP_SPACE, 				keyboard::scan_code::kp_space},
    // {SDL_SCANCODE_KP_AT, 				keyboard::scan_code::kp_at},
    // {SDL_SCANCODE_KP_EXCLAM, 				keyboard::scan_code::kp_exclam},
    // {SDL_SCANCODE_KP_MEMSTORE, 				keyboard::scan_code::kp_memstore},
    // {SDL_SCANCODE_KP_MEMRECALL, 				keyboard::scan_code::kp_memrecall},
    // {SDL_SCANCODE_KP_MEMCLEAR, 				keyboard::scan_code::kp_memclear},
    // {SDL_SCANCODE_KP_MEMADD, 				keyboard::scan_code::kp_memadd},
    // {SDL_SCANCODE_KP_MEMSUBTRACT, 				keyboard::scan_code::kp_memsubtract},
    // {SDL_SCANCODE_KP_MEMMULTIPLY, 				keyboard::scan_code::kp_memmultiply},
    // {SDL_SCANCODE_KP_MEMDIVIDE, 				keyboard::scan_code::kp_memdivide},
    // {SDL_SCANCODE_KP_PLUSMINUS, 				keyboard::scan_code::kp_plusminus},
    // {SDL_SCANCODE_KP_CLEAR, 				keyboard::scan_code::kp_clear},
    // {SDL_SCANCODE_KP_CLEARENTRY, 				keyboard::scan_code::kp_clearentry},
    // {SDL_SCANCODE_KP_BINARY, 				keyboard::scan_code::kp_binary},
    // {SDL_SCANCODE_KP_OCTAL, 				keyboard::scan_code::kp_octal},
    // {SDL_SCANCODE_KP_DECIMAL, 				keyboard::scan_code::kp_decimal},
    // {SDL_SCANCODE_KP_HEXADECIMAL, 				keyboard::scan_code::kp_hexadecimal},
    {SDL_SCANCODE_LCTRL, 				keyboard::scan_code::lcontrol},
    {SDL_SCANCODE_LSHIFT, 				keyboard::scan_code::lshift},
    {SDL_SCANCODE_LALT, 				keyboard::scan_code::lalt},
    // {SDL_SCANCODE_LGUI, 				keyboard::scan_code::lgui},
    {SDL_SCANCODE_RCTRL, 				keyboard::scan_code::rcontrol},
    {SDL_SCANCODE_RSHIFT, 				keyboard::scan_code::rshift},
    {SDL_SCANCODE_RALT, 				keyboard::scan_code::ralt},
    // {SDL_SCANCODE_RGUI, 				keyboard::scan_code::rgui},
    // {SDL_SCANCODE_MODE, 				keyboard::scan_code::mode},
    {SDL_SCANCODE_AUDIONEXT, 				keyboard::scan_code::medianext},
    {SDL_SCANCODE_AUDIOPREV, 				keyboard::scan_code::mediaprevious},
    {SDL_SCANCODE_AUDIOSTOP, 				keyboard::scan_code::mediastop},
    {SDL_SCANCODE_AUDIOPLAY, 				keyboard::scan_code::mediaplaypause},
    // {SDL_SCANCODE_AUDIOMUTE, 				keyboard::scan_code::mediamute},
    // {SDL_SCANCODE_MEDIASELECT, 				keyboard::scan_code::mediaselect},
    // {SDL_SCANCODE_WWW, 				keyboard::scan_code::www},
    // {SDL_SCANCODE_MAIL, 				keyboard::scan_code::mail},
    // {SDL_SCANCODE_CALCULATOR, 				keyboard::scan_code::calculator},
    // {SDL_SCANCODE_COMPUTER, 				keyboard::scan_code::computer},
    // {SDL_SCANCODE_AC_SEARCH, 				keyboard::scan_code::ac_search},
    // {SDL_SCANCODE_AC_HOME, 				keyboard::scan_code::ac_home},
    // {SDL_SCANCODE_AC_BACK, 				keyboard::scan_code::ac_back},
    // {SDL_SCANCODE_AC_FORWARD, 				keyboard::scan_code::ac_forward},
    // {SDL_SCANCODE_AC_STOP, 				keyboard::scan_code::ac_stop},
    // {SDL_SCANCODE_AC_REFRESH, 				keyboard::scan_code::ac_refresh},
    // {SDL_SCANCODE_AC_BOOKMARKS, 				keyboard::scan_code::ac_bookmarks},
    // {SDL_SCANCODE_BRIGHTNESSDOWN, 				keyboard::scan_code::brightnessdown},
    // {SDL_SCANCODE_BRIGHTNESSUP, 				keyboard::scan_code::brightnessup},
    // {SDL_SCANCODE_DISPLAYSWITCH, 				keyboard::scan_code::displayswitch},
    // {SDL_SCANCODE_KBDILLUMTOGGLE, 				keyboard::scan_code::kbdillumtoggle},
    // {SDL_SCANCODE_KBDILLUMDOWN, 				keyboard::scan_code::kbdillumdown},
    // {SDL_SCANCODE_KBDILLUMUP, 				keyboard::scan_code::kbdillumup},
    // {SDL_SCANCODE_EJECT, 				keyboard::scan_code::eject},
    // {SDL_SCANCODE_SLEEP, 				keyboard::scan_code::sleep},
    // {SDL_SCANCODE_APP1, 				keyboard::scan_code::app1},
    // {SDL_SCANCODE_APP2, 				keyboard::scan_code::app2},
    // {SDL_SCANCODE_SOFTLEFT, 				keyboard::scan_code::softleft},
    // {SDL_SCANCODE_SOFTRIGHT, 				keyboard::scan_code::softright},
    // {SDL_SCANCODE_CALL, 				keyboard::scan_code::call},
    // {SDL_SCANCODE_ENDCALL, 				keyboard::scan_code::endcall},
    };
    return cpaf::map_get(look_up, sdl_sc, keyboard::scan_code::unknown);

}





} // END namespace cpaf::gui::events

