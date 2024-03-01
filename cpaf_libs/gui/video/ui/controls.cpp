#include "controls.h"
#include <cpaf_libs/gui/video/player.h>
#include <cpaf_libs/gui/video/config.h>

namespace cpaf::gui::video {

controls::controls(player& parent_player, config& cfg)
    : player_(parent_player)
    , config_(cfg)
{}

void controls::render()
{
    if (!visible()) {
        return;
    }
    do_render();

}

std::string controls::font_name() const {
    return config_.controls_str("font_name");
}

color controls::text_color() const {
    return config_.controls_color("text_color");
}

color controls::border_color() const {
    return config_.controls_color("border_color");
}

color controls::background_color() const {
    return config_.controls_color("background_color");
}

uint16_t controls::time_font_size() const {
    return config_.controls_int32("time_font_size");
}

uint16_t controls::slider_height() const {
    return config_.controls_int32("slider_height");
}

uint16_t controls::buttons_size() const {
    return config_.controls_int32("buttons_size");
}

float controls::relative_ypos() const {
    return config_.controls_float("relative_ypos");
}

} // namespace cpaf::gui::video

