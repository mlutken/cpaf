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

void system_render_platform::do_set_color(color col)
{
    const auto c = col.to_u8color();
    SDL_SetRenderDrawColor(sdl_renderer_, c.r(), c.g(), c.b(), c.a());
}

void system_render_platform::do_fill_rect(rect rect) const
{
    SDL_RenderFillRectF(sdl_renderer_, reinterpret_cast<SDL_FRect*>(&rect));
}

math::v2i32 system_render_platform::do_maximum_texture_dimensions() const
{
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(sdl_renderer_, &info) == 0) {
        return {info.max_texture_width, info.max_texture_height};
    }
    return {-1, -1};
}

// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui

