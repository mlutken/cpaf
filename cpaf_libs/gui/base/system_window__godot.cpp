#include "system_window__godot.h"
#include <fmt/format.h>

#include <cpaf_libs/gui/system_render.h>

namespace cpaf::gui {



// --------------------------
// --- Platform overrides ---
// --------------------------

system_window_platform::~system_window_platform()
{
    fmt::println("system_window_platform::DESTRUCTOR ({})", do_get_title());
}

// TODO_GODOT
system_window_platform::system_window_platform(const size_2d& size, std::string_view title)
{
    fmt::println("system_window_platform::CONSTRUCTOR ({}) {}, {}", title, size.width(), size.height() );

    // render_ = std::shared_ptr<system_render>(new system_render(sdl_window_));
}

// TODO_GODOT
size_2d system_window_platform::do_size() const
{
    if (!godot_window_) {return {-1,-1}; }
    size_2d size{-1,-1};
    return size;
}

// TODO_GODOT
size_2d system_window_platform::do_display_size() const
{
    if (!godot_window_) {return {-1,-1}; }

    return {-1,-1};
}

std::string system_window_platform::do_get_title() const
{
    return "Get Title FIXMENM TODO";
}

// TODO_GODOT
int32_t system_window_platform::do_display_index() const
{
    // return SDL_GetWindowDisplayIndex(godot_window_);
    return -1;
}


// TODO_GODOT
bool system_window_platform::do_window_mode_set(window_mode_t wm)
{
    // return SDL_SetWindowFullscreen(godot_window_, to_sdl_window_mode(wm)) == 0;
    return true;
}

void* system_window_platform::do_get_native_window()
{
    return godot_window_;
}

// TODO_GODOT
void* system_window_platform::do_get_native_renderer()
{
    // return render_->get_native_renderer();
    return nullptr;
}

system_render& system_window_platform::do_renderer()
{
    return *render_;
}

std::shared_ptr<system_render> system_window_platform::do_renderer_shared() const
{
    return render_;
}


// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui

