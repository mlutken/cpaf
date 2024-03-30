#include "controls.h"
#include <cpaf_libs/gui/video/player.h>

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

//std::string controls::font_name() const {
//    return config_.str("controls", "font_name");
//}

//color controls::time_text_color() const {
//    return config_.color("controls", "time_text_color");
//}

//color controls::border_color() const {
//    return config_.color("controls", "border_color");
//}

//color controls::bg_color() const {
//    return config_.color("controls", "bg_color");
//}

//int32_t controls::time_font_size() const {
//    return config_.int32("controls", "time_font_size");
//}

//int32_t controls::slider_height() const {
//    return config_.int32("controls", "slider_height");
//}

//int16_t controls::buttons_size() const {
//    return config_.int32("controls", "buttons_size");
//}

//float controls::relative_ypos() const {
//    return config_.float_val("controls", "relative_ypos");
//}

} // namespace cpaf::gui::video

