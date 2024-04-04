#include "system_window__sdl2.h"
#include <unordered_map>
#include <fmt/format.h>

#include <cpaf_libs/gui/system_render.h>

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
    // if (sdl_window_renderer_) {
    //     SDL_DestroyRenderer(sdl_window_renderer_);
    // }
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

    render_ = std::shared_ptr<system_render>(new system_render(sdl_window_));
}

size_2d system_window_platform::do_size() const
{
    if (!sdl_window_) {return {-1,-1}; }
    size_2d size;
    SDL_GetWindowSize(sdl_window_, &size[0], &size[1]);
    return size;
}

size_2d system_window_platform::do_display_size() const
{
    if (!sdl_window_) {return {-1,-1}; }

    SDL_DisplayMode mode;

    if (SDL_GetCurrentDisplayMode(do_display_index(), &mode) == 0) {
        return {mode.w, mode.h};
    }
    return {-1,-1};
}

std::string system_window_platform::do_get_title() const
{
    return "Get Title FIXMENM TODO";
}

int32_t system_window_platform::do_display_index() const
{
    return SDL_GetWindowDisplayIndex(sdl_window_);
}

void* system_window_platform::do_get_native_window()
{
    return sdl_window_;
}

void* system_window_platform::do_get_native_renderer()
{
    return render_->get_native_renderer();
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

