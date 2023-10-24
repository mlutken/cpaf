#include "touch_events.h"

namespace cpaf::gui::events {


std::string events::touch::to_name(touch::type t)
{
    switch (t) {
    case touch::type::down  : return "down";  break;
    case touch::type::up    : return "up";    break;
    case touch::type::move  : return "move";  break;

    default:
        break;
    }
    return "unknown";
}


} //END namespace cpaf::gui::events
