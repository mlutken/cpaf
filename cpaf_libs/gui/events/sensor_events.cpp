#include "sensor_events.h"
#include <fmt/format.h>

namespace cpaf::gui::events {

std::string events::sensor::to_name(sensor::type t)
{
    switch (t) {
    case sensor::type::accelero_meter   : return "accelero_meter";  break;
    case sensor::type::gyroscope        : return "gyroscope";       break;
    case sensor::type::magneto_meter    : return "magneto_meter";   break;
    case sensor::type::gravity          : return "gravity";         break;
    case sensor::type::acceleration     : return "acceleration";    break;
    case sensor::type::orientation      : return "orientation";     break;

    default:
        break;
    }
    return "unknown";

}

std::string sensor::to_string(to_str_mode /*mode*/) const
{
    auto s = name() + "|" + sensor::to_name(tp);
    s += fmt::format(" x, y, z: ({}, {}, {}) ", x, y, z);
    return s;
}

} //END namespace cpaf::gui::events
