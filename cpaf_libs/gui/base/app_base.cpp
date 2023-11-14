#include "app_base.h"

#include <sstream>
#include <cpaf_libs/filesystem/cpaf_special_dirs.h>
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/events/event.h>

using namespace std;

namespace cpaf::gui {

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
        event_handler(evt);
    }
}



} // END namespace cpaf::gui

