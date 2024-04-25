#include "system_window_base.h"

#include <sstream>

namespace cpaf::gui {

void system_window_base::toggle_full_screen_mode()
{
    if (window_mode_ == window_mode_t::window) {
        window_mode_set(default_window_full_screen_mode);
    }
    else {
        window_mode_set(window_mode_t::window);
    }
}

void system_window_base::window_mode_set(window_mode_t wm)
{
    if (do_window_mode_set(wm)) {
        window_mode_ = wm;
    }
}

// -----------------------------
// --- Static Info functions ---
// -----------------------------


#if (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_DESKTOP)
const window_mode_t system_window_base::default_window_mode = window_mode_t::window;
const window_mode_t system_window_base::default_window_full_screen_mode = window_mode_t::full_screen_desktop;
#elif (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_EMBEDDED)
const window_mode_t system_window_base::default_window_mode = window_mode_t::full_screen;
const window_mode_t system_window_base::default_window_full_screen_mode = window_mode_t::full_screen;
#elif (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_DESKTOP_WEB)
const window_mode_t system_window_base::default_window_mode = window_mode_t::full_screen;
const window_mode_t system_window_base::default_window_full_screen_mode = window_mode_t::full_screen;
#elif (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_MOBILE)
const window_mode_t system_window_base::default_window_mode = window_mode_t::full_screen;
const window_mode_t system_window_base::default_window_full_screen_mode = window_mode_t::full_screen;
#elif (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_TABLET)
const window_mode_t system_window_base::default_window_mode = window_mode_t::full_screen;
const window_mode_t system_window_base::default_window_full_screen_mode = window_mode_t::full_screen;
#elif (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_TABLET_WEB)
const window_mode_t system_window_base::default_window_mode = window_mode_t::full_screen;
const window_mode_t system_window_base::default_window_full_screen_mode = window_mode_t::full_screen;
#elif (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_MOBILE_WEB)
const window_mode_t system_window_base::default_window_mode = window_mode_t::full_screen;
const window_mode_t system_window_base::default_window_full_screen_mode = window_mode_t::full_screen;
#endif



// system_window_base::system_window_base(const size_2d& /*size*/, std::string_view /*title*/)
// {

// }



// -------------------------------------------------------
// --- PROTECTED: Helper functions for derived classes ---
// -------------------------------------------------------




} // END namespace cpaf::gui

