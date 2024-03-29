#include "config.h"

namespace cpaf::gui::video {



static const auto default_config = nlohmann::json::parse(
    R"(
    {
        "controls": {
            "font_name": "abeezee_regular",
            "text_color": [1,1,1,1],
            "background_color": [0,0,0,1],
            "border_color": [0,0,0,1],
            "time_font_size": 13,
            "slider_height": 8,
            "buttons_size": 38,
            "relative_ypos": 0.7,
            "skip_time_small": 15
        },
        "user": {
            "ui_language_code": "en",
            "subtitles_font_name": "abeezee_regular",
            "subtitles_font_size": 22,
            "subtitles_font_color": [1,1,1,1],
            "subtitles_bg_color": [0,0,0,1],
            "subtitles_font_scale": 1.0,
            "subtitles_relative_ypos": 0.95,
            "subtitles_has_background": true,
            "subtitles_show": true
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

std::string config::str(const std::string& group, const std::string& id) const
{
    return cpaf::json_value_str(jo_[group], id, "");
}

int32_t config::int32(const std::string& group, const std::string& id) const
{
    return cpaf::json_value_int32(jo_[group], id, 0);
}

bool config::bool_val(const std::string& group, const std::string& id) const
{
    return cpaf::json_value_bool(jo_[group], id, 0);
}

float config::float_val(const std::string& group, const std::string& id) const
{
    return cpaf::json_value_float(jo_[group], id, 0);
}

color config::color(const std::string& group, const std::string& id) const
{
    const auto jo_arr = cpaf::json_value_array(jo_[group], id, nlohmann::json::array({0,0,0,0}));
    return cpaf::gui::color(jo_arr);
}

std::chrono::seconds config::seconds(const std::string& group, const std::string& id) const
{
    return std::chrono::seconds(cpaf::json_value_int32(jo_[group], id, 0));
}

void config::str_set(const std::string& group, const std::string& id, const std::string& val)
{
    jo_[group][id] = val;
    signal_changed();
}

void config::int32_set(const std::string& group, const std::string& id, int32_t val)
{
    jo_[group][id] = val;
    signal_changed();
}

void config::float_set(const std::string& group, const std::string& id, float val)
{
    jo_[group][id] = val;
    signal_changed();
}

void config::color_set(const std::string& group, const std::string& id, cpaf::gui::color val)
{
    jo_[group][id] = val.to_json();
    signal_changed();
}

void config::seconds_set(const std::string& group, const std::string& id, std::chrono::seconds val)
{
    jo_[group][id] = val.count();
    signal_changed();
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
