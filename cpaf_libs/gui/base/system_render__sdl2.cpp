#include "system_render__sdl2.h"
#include <fmt/format.h>


namespace cpaf::gui {


// --------------------------
// --- Platform overrides ---
// --------------------------

system_render_platform::~system_render_platform()
{
    fmt::println("system_render_platform::DESTRUCTOR()");
    if (sdl_renderer_) {
        SDL_DestroyRenderer(sdl_renderer_);
    }
}

system_render_platform::system_render_platform(SDL_Window* sdl_window)
{
    auto renderer_flags{static_cast<SDL_RendererFlags>(SDL_RENDERER_ACCELERATED)};
    sdl_renderer_ = SDL_CreateRenderer(sdl_window, -1, renderer_flags);
}


void* system_render_platform::do_get_native_renderer()
{
    return sdl_renderer_;
}


// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui

