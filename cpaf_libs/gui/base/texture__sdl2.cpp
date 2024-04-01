#include "texture__sdl2.h"
#include <unordered_map>
#include <fmt/format.h>

#include <cpaf_libs/gui/system_render.h>

namespace cpaf::gui {



// --------------------------
// --- Platform overrides ---
// --------------------------

texture_platform::~texture_platform()
{
    fmt::println("texture_platform::DESTRUCTOR ()");
    destroy();
    // if (sdl_window_renderer_) {
    //     SDL_DestroyRenderer(sdl_window_renderer_);
    // }
//    if(sdl_window_) {
//        SDL_DestroyWindow(sdl_window_);
//    }
}

texture_platform::texture_platform(const size_2d& /*size*/)
    : texture_base()
{
//    fmt::println("texture_platform::CONSTRUCTOR [{}, {}]", size.width(), size.height() );
//    const uint32_t window_flags{SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI};

//    sdl_window_ = SDL_CreateWindow(
//       title.data(),
//       SDL_WINDOWPOS_CENTERED,
//       SDL_WINDOWPOS_CENTERED,
//       size.width(),
//       size.height(),
//       window_flags
//    );

    //    render_ = std::shared_ptr<system_render>(new system_render(sdl_window_));
}

void texture_platform::destroy()
{
    if (sdl_texture_) {
        SDL_DestroyTexture(sdl_texture_);
    }
}

size_2d texture_platform::do_get_size() const
{
    if (!sdl_texture_) {return {-1,-1}; }
    Uint32 format;
    int access;
    size_2d size;
    const int res = SDL_QueryTexture(sdl_texture_, &format, &access, &size[0], &size[1]);
    if (res == 0) {
        return size;
    }
    return {-1, -1};
}

bool texture_platform::do_load_from_file(const std::filesystem::__cxx11::path& local_path)
{

}


// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui

