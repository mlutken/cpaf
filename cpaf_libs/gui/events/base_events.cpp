#include "base_events.h"
#include <fmt/format.h>

using namespace std;

namespace cpaf::gui::events {

std::string to_name(pointer_type t)
{
    switch (t) {
    case pointer_type::mouse: return "mouse";   break;
    case pointer_type::touch: return "touch";   break;

    default:
        break;
    }
    return "unknown";
}

std::string to_name(is_handled handled)
{
    switch (handled) {
    case is_handled::yes    : return "yes";     break;
    case is_handled::no     : return "no";      break;

    default:
        break;
    }
    return "";
}

std::string application::to_name(type t)
{
    switch (t) {
    case application::type::user_quit               : return "user_quit";               break;
    case application::type::terminating             : return "terminating";             break;
    case application::type::low_memory              : return "low_memory";              break;
    case application::type::will_enter_background   : return "will_enter_background";	break;
    case application::type::did_enter_background    : return "did_enter_background";	break;
    case application::type::will_enter_foreground   : return "will_enter_foreground";   break;
    case application::type::did_enter_foreground    : return "did_enter_foreground";	break;
    default:
        break;
    }
    return "unknown";
}

std::string application::to_string(to_str_mode /*mode*/) const
{
    return name() + "|" + application::to_name(tp);
}

std::string window::to_name(type t)
{
    switch (t) {
    case window::type::shown            : return "shown";           break;
    case window::type::hidden           : return "hidden";          break;
    case window::type::exposed          : return "exposed";         break;
    case window::type::move             : return "move";            break;
    case window::type::resized          : return "resized";         break;
    case window::type::minimized        : return "minimized";       break;
    case window::type::maximized        : return "maximized";       break;
    case window::type::restored         : return "restored";        break;
    case window::type::mouse_entered    : return "mouse_entered";   break;
    case window::type::mouse_left       : return "mouse_left";      break;
    case window::type::focus_gained     : return "focus_gained";	break;
    case window::type::focus_lost       : return "focus_lost";      break;
    case window::type::close            : return "close";           break;
    default:
        break;
    }
    return "unknown";
}

string window::to_string(to_str_mode /*mode*/) const
{
    return fmt::format("{}|{} [size: ({}, {})]", name(), window::to_name(tp), width, height);
    // return name() + "|" + window::to_name(tp);
}

std::string display::to_name(display::orientation orient)
{
    switch (orient) {
    case display::orientation::horizontal   : return "horizontal";  break;
    case display::orientation::vertical     : return "vertical";    break;
    default:
        break;
    }
    return "unknown";
}

string display::to_string(to_str_mode /*mode*/) const
{
    return name() + "|" + display::to_name(orien);
}

} //END namespace cpaf::gui::events
