#include "config.h"

namespace cpaf::gui::video {



static auto default_config = nlohmann::json::parse(
    R"(
    {
        "controls": {
            "font_name": "manrope",
            "text_color": [1,1,1,1],
            "background_color": [0,0,0,1],
            "border_color": [0,0,0,1],
            "time_font_size": 13,
            "slider_height": 18,
            "buttons_size": 38,
            "relative_ypos": 0.8,
            "skip_time_small": 15
        }
    }
    )");


config::config() {}

void config::connect_for_changes(changed_cb callback)
{
    on_changed_callbacks_.push_back(callback);
}

void config::signal_changed() const
{
    for (auto& cb: on_changed_callbacks_) {
        cb();
    }
}

} // namespace cpaf::gui::video
