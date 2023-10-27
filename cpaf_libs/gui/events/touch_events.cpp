#include "touch_events.h"
#include <fmt/format.h>

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

std::string touch::to_string(to_str_mode /*mode*/) const
{
    auto s = name() + "|" + touch::to_name(tp);
    s += fmt::format(" pos: ({}, {}) delta: ({} {}))", x, y, delta_x, delta_y);
    s += fmt::format(" pressure: {} finger: {} pointer type: {}))", pressure, finger, events::to_name(event_src_type));
    return s;
}


} //END namespace cpaf::gui::events
