#include "app_base.h"

#include <sstream>
#include <cpaf_libs/filesystem/cpaf_special_dirs.h>
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/events/event.h>

using namespace std;

namespace cpaf::gui {

app_base::app_base() :
    last_mouse_or_touch_event_time_(std::chrono::steady_clock::now())
{

}

exit_status_t app_base::run()
{
    do_platform_start_run();
    start_run();
    while (is_running_) {
        process_events();
        do_platform_pre_frame_update();  // FIXMENM
        pre_frame_update();
        do_platform_frame_update();
        frame_update();
        do_platform_post_frame_update(); // FIXMENM
        post_frame_update();
    }
    return exit_status_;
}

void app_base::add_fonts(const std::string& font_name, const std::vector<uint32_t>& sizes_in_points)
{
    do_add_fonts(font_name, sizes_in_points);
}

void app_base::add_fonts(const std::string& font_name,
                         float size_adjust_factor,
                         uint32_t range_min,
                         uint32_t range_max,
                         const std::vector<uint32_t>& sizes_in_points)
{
    do_add_fonts(font_name, size_adjust_factor, range_min, range_max, sizes_in_points);
}

void app_base::add_fonts (const std::string& font_name,
                          const std::string& merge_font_name,
                          float merge_size_adjust_factor,
                          uint32_t range_min,
                          uint32_t range_max,
                          const std::vector<uint32_t>& sizes_in_points)
{
    do_add_fonts(font_name, merge_font_name, merge_size_adjust_factor, range_min, range_max, sizes_in_points);
}

void app_base::set_default_font(const std::string& font_name, uint32_t size_points)
{
    do_set_default_font(font_name, size_points);
}

std::filesystem::path app_base::config_path() const
{
    if (!config_path_.empty()) { return config_path_; }

    return cpaf::filesystem::special_dirs::app_data_local() / ("."s + app_name_);
}

// -------------------------------------------------------------
// --- PROTECTED: User virtual functions for derived classes ---
// -------------------------------------------------------------
// Default implementations
events::is_handled app_base::event_handler(const events::event& evt)
{
    // fmt::println("FIXMENM app_base::event_handler(): {} ", evt.to_string(events::to_str_mode::normal));
    // std::cout << std::flush;

    using namespace events;
    if (auto app = evt.as<application>()) {
        if (app->tp == application::type::user_quit) {
            stop();
            return is_handled::yes;
        }
    }
    else if (auto win = evt.as<window>()) {
        if (win->tp == window::type::close) {
            stop();
            return is_handled::yes;
        }
    }

    return is_handled::no;
}


// -------------------------------------------------------
// --- PROTECTED: Helper functions for derived classes ---
// -------------------------------------------------------



// ---------------------------------
// --- PRIVATE: Helper functions ---
// ---------------------------------
void app_base::process_events()
{
    while (true) {
        events::event evt = do_platform_get_event();
        if (evt.is<events::none>()) {
            break;
        }
        update_mouse_touch_time(evt);
        event_handler(evt);
    }
}

void app_base::update_mouse_touch_time(const events::event& evt)
{
    if (evt.is<events::mouse>() || evt.is<events::touch>()) {
        last_mouse_or_touch_event_time_ = std::chrono::steady_clock::now();
    }
}



} // END namespace cpaf::gui

