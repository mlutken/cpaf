#include "app_base.h"

#include <sstream>
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/events/event.h>

namespace cpaf::gui {

exit_status_t app_base::run()
{
    do_start_run();
    while (is_running_) {
        process_events();
        do_pre_frame_update();
        do_frame_update();
        do_post_frame_update();
    }
    return exit_status_;
}

// -------------------------------------------------------------
// --- PROTECTED: User virtual functions for derived classes ---
// -------------------------------------------------------------
// Default implementations
events::is_handled app_base::event_handler(const events::event& evt)
{
    fmt::println("FIXMENM app_base::event_handler(): {} ", evt.to_string(events::to_str_mode::normal));
    std::cout << std::flush;

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
        events::event evt = do_get_event();
        if (evt.is<events::none>()) {
            break;
        }
        event_handler(evt);
    }
}



} // END namespace cpaf::gui

