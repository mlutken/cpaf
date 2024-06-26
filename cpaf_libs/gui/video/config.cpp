#include "config.h"
#include <cpaf_libs/utils/cpaf_json_utils.h>

namespace cpaf::gui::video {

static const auto default_config = nlohmann::json::parse(
    R"(
    {
        "controls": {
            "buttons_relative_ypos": 0.8,
            "buttons_relative_x_dist": 0.25,
            "play_buttons_size": 84,
            "menu_buttons_size": 48,
            "buttons_color": [0.282353, 0.239216, 0.545098, 0.5],
            "menu_close_button_color": [0.8, 0.1, 0.1, 1.0],

            "slider_relative_ypos": 1.0,
            "slider_height": 8,
            "slider_font_name": "abeezee_regular",

            "time_font_name": "abeezee_regular",
            "time_font_size": 16,
            "time_text_color": [1,1,1,1],

            "stream_state_font_name": "manrope",
            "stream_state_font_size": 48,

            "skip_time_small": 15,
            "show_ui_time_out": 4
        },
        "network": {
            "torrent_time_out": "120"
        },
        "subtitles": {
            "font_name": "manrope",
            "font_size": 18,
            "font_color": [1,1,1,1],
            "bg_color": [0,0,0,1],
            "font_scale": 1.0,
            "relative_ypos": 0.92,
            "has_background": true,
            "show": true,
            "language_code": "da"
        },
        "ui": {
            "language_code": "en"
        },
        "video": {
            "keep_aspect_ratio": true
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

color config::color_rgba(const std::string& group, const std::string& id) const
{
    const auto jo_arr = cpaf::json_value_array(jo_[group], id, nlohmann::json::array({0,0,0,0}));
    return cpaf::gui::color::rgba(jo_arr[0],jo_arr[1],jo_arr[2],jo_arr[3]);
}

std::chrono::seconds config::time_s(const std::string& group, const std::string& id) const
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

void config::bool_set(const std::string& group, const std::string& id, bool val)
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

void config::time_s_set(const std::string& group, const std::string& id, std::chrono::seconds val)
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

void config::dbg_print() const
{
    std::cerr << dbg_string() << "\n";
}

void config::signal_changed() const
{
//    std::cerr << "TODO config::signal_changed() crashing. Fix!\n";
    // Reason: We keep adding in the render_base::render_base constructor whic is caleed on every new file opened
    // render_base::render_base(player& owning_player, config& cfg)
    // config_.connect_for_changes([this]() {on_configuration_changed();});

    for (auto& cb: on_changed_callbacks_) {
        cb();
    }
}

} // namespace cpaf::gui::video
