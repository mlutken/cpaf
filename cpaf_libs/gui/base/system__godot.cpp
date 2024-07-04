#include "system__godot.h"

#include <fmt/format.h>


namespace cpaf::gui {



// --------------------------
// --- Platform overrides ---
// --------------------------

system_platform::~system_platform()
{
    fmt::println("system_platform::DESTRUCTOR GODOT");
}

// TODO_GODOT
int32_t system_platform::do_init()
{
    const int32_t retval = 1;
    return retval;
}

// TODO_GODOT
int32_t system_platform::do_display_count() const
{
    return 1;
}

// TODO_GODOT
float system_platform::do_dpi(int32_t display_index) const
{
    float dpi = 0;
    return dpi;
}


// ------------------------
// --- PRIVATE: Helpers ---
// ------------------------


} // END namespace cpaf::gui

