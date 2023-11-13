#include "system_window__sdl2.h"
#include <unordered_map>
#include <fmt/format.h>


namespace cpaf::gui {


//// Friend function/static callback from SDL
//void sdl_native_audio_callback	(void* userdata, uint8_t* buffer, int buffer_max_len)
//{
//    device_platform* device_sdl = static_cast<device_platform*>(userdata);
//    device_sdl->execute_play_callback(buffer, buffer_max_len);
//}



// --------------------------
// --- Platform overrides ---
// --------------------------

system_window_platform::~system_window_platform()
{
    fmt::println("system_window_platform::DESTRUCTOR ({})", do_get_title());
    if (sdl_window_renderer_) {
        SDL_DestroyRenderer(sdl_window_renderer_);
    }
    if(sdl_window_) {
        SDL_DestroyWindow(sdl_window_);
    }
}

system_window_platform::system_window_platform(const size_2d& size, std::string_view title)
{
    fmt::println("system_window_platform::CONSTRUCTOR ({}) {}, {}", title, size.width(), size.height() );
    const uint32_t window_flags{SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI};

    sdl_window_ = SDL_CreateWindow(
       title.data(),
       SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED,
       size.width(),
       size.height(),
       window_flags
    );

    auto renderer_flags{static_cast<SDL_RendererFlags>(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE)};
    sdl_window_renderer_ = SDL_CreateRenderer(sdl_window_, -1, renderer_flags);
}

size_2d system_window_platform::do_get_size() const
{
    return {0,0};
}

std::string system_window_platform::do_get_title() const
{
    return "Get Title FIXMENM TODO";
}

void* system_window_platform::do_get_native_window()
{
    return sdl_window_;
}

void* system_window_platform::do_get_native_renderer()
{
    return sdl_window_renderer_;
}


// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui

