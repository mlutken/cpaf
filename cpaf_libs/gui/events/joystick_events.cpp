#include "joystick_events.h"

namespace cpaf::gui::events {

std::string joystick::to_name(joystick::type t)
{
    switch (t) {
    case joystick::type::axis           : return "axis";            break;
    case joystick::type::ball           : return "ball";            break;
    case joystick::type::hat            : return "hat";             break;
    case joystick::type::button         : return "button";          break;
    case joystick::type::connected      : return "connected";       break;
    case joystick::type::disconnected   : return "disconnected";    break;

    default:
        break;
    }
    return "unknown";
}

std::string joystick::to_name(joystick::hat_pos pos)
{
    switch (pos) {
    case joystick::hat_pos::centered	: return "centered";	break;
    case joystick::hat_pos::up          : return "up";          break;
    case joystick::hat_pos::right_up	: return "right_up";	break;
    case joystick::hat_pos::right       : return "right";       break;
    case joystick::hat_pos::right_down  : return "right_down";	break;
    case joystick::hat_pos::down        : return "down";        break;
    case joystick::hat_pos::left_down	: return "left_down";   break;
    case joystick::hat_pos::left        : return "left";        break;
    case joystick::hat_pos::left_up     : return "left_up";     break;

    default:
        break;
    }
    return "unknown";
}

std::string joystick::to_name(joystick::button_action action)
{
    switch (action) {
    case joystick::button_action::down  : return "down";	break;
    case joystick::button_action::up	: return "up";      break;
    default:
        break;
    }
    return "unknown";
}

} //END namespace cpaf::gui::events
