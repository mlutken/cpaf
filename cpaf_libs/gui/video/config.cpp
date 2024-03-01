#include "config.h"

namespace cpaf::gui::video {



static const auto default_config = nlohmann::json::parse(
    R"(
    {
        "controls": {
            "font_name": "manrope",
            "text_color": [1,1,1,1],
            "background_color": [0,0,0,1],
            "border_color": [0,0,0,1],
            "time_font_size": 13,
            "slider_height": 8,
            "buttons_size": 38,
            "relative_ypos": 0.7,
            "skip_time_small": 15
        }
    }
    )");


config::config()
    : jo_(default_config)
{}

void config::set(const nlohmann::json& jo)
{
    jo_ = jo;
    signal_changed();
}

void config::controls_set(const std::string& id, const nlohmann::json& val)
{
    jo_["controls"][id] = val;
    signal_changed();
}

color config::controls_color(const std::string& id) const
{
    const auto jo_arr = cpaf::json_value_array(jo_["controls"], id, nlohmann::json::array({0,0,0,0}));
    return cpaf::gui::color(jo_arr);
}

std::string config::controls_str(const std::string& id) const
{
    return cpaf::json_value_str(jo_["controls"], id, "");
}

float config::controls_float(const std::string& id) const
{
    return cpaf::json_value_float(jo_["controls"], id, 0);
}

int32_t config::controls_int32(const std::string& id) const
{
    return cpaf::json_value_int32(jo_["controls"], id, 0);
}

std::chrono::seconds config::controls_seconds(const std::string& id) const
{
    return std::chrono::seconds(cpaf::json_value_int32(jo_["controls"], id, 0));
}

void config::connect_for_changes(changed_cb callback)
{
    on_changed_callbacks_.push_back(callback);
}

std::string config::dbg_string() const
{
    return jo_.dump(4);
}

void config::signal_changed() const
{
    for (auto& cb: on_changed_callbacks_) {
        cb();
    }
}

} // namespace cpaf::gui::video
