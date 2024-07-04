#include "system_render__godot.h"
#include <fmt/format.h>
#include <cpaf_libs/gui/platform_utils/sdl_convert.h>
#include <cpaf_libs/gui/texture.h>


namespace cpaf::gui {


// --------------------------
// --- Platform overrides ---
// --------------------------

system_render_platform::~system_render_platform()
{
    fmt::println("system_render_platform::DESTRUCTOR()");
}

// system_render_platform::system_render_platform(SDL_Window* sdl_window)
// {
// }


void* system_render_platform::do_get_native_renderer()
{
    return godot_renderer_;
}

// TODO_GODOT
void system_render_platform::do_set_color(color col)
{
}

// TODO_GODOT
void system_render_platform::do_fill_rect(rect rect) const
{
}

// TODO_GODOT
bool system_render_platform::do_render_texture(const cpaf::gui::texture& tex, rect src_rect, rect dst_rect)
{
}

// TODO_GODOT
math::v2i32 system_render_platform::do_maximum_texture_dimensions() const
{
    return {-1, -1};
}

// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui

