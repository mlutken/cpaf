#include "mouse_events.h"

namespace cpaf::gui::events {

std::string events::mouse::to_name(type t)
{
    switch (t) {
    case mouse::type::move              : return "move";            break;
    case mouse::type::button_down       : return "button_down";     break;
    case mouse::type::button_up         : return "button_up";       break;
    case mouse::type::clicked           : return "clicked";         break;
    case mouse::type::double_clicked    : return "double_clicked";  break;
    case mouse::type::wheel             : return "wheel";           break;
    case mouse::type::entered           : return "entered";         break;
    case mouse::type::left              : return "left";            break;
    default:
        break;
    }
    return "unknown";
}

std::string mouse::to_name(button b)
{
    switch (b) {
    case mouse::button::left        : return "left";        break;
    case mouse::button::right       : return "right";       break;
    case mouse::button::middle      : return "middle";      break;
    case mouse::button::xbutton1    : return "xbutton1";    break;
    case mouse::button::xbutton2    : return "xbutton2";    break;

    default:
        break;
    }
    return "unknown";
}


std::string mouse::to_name(wheel w)
{
    switch (w) {
    case mouse::wheel::vertical     : return "vertical";    break;
    case mouse::wheel::horizontal   : return "horizontal";  break;

    default:
        break;
    }
    return "unknown";
}

} //END namespace cpaf::gui::events
