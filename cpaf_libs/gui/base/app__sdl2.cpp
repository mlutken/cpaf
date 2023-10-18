#include "app__sdl2.h"
#include <unordered_map>
#include <fmt/format.h>
#include <SDL2/SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>

#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/base/system_window__sdl2.h>

using namespace std;

namespace cpaf::gui {

app_platform::app_platform()
{
    fmt::println("app_platform::CONSTRUCTOR");

//    main_window_ = create_system_window({300, 200}, "Hello GUI");
}

app_platform::~app_platform()
{
//  ImGui_ImplSDLRenderer_Shutdown();
//  ImGui_ImplSDL2_Shutdown();
//  ImGui::DestroyContext();

  SDL_Quit();
}


//// Friend function/static callback from SDL
//void sdl_native_audio_callback	(void* userdata, uint8_t* buffer, int buffer_max_len)
//{
//    device_platform* device_sdl = static_cast<device_platform*>(userdata);
//    device_sdl->execute_play_callback(buffer, buffer_max_len);
//}



// --------------------------
// --- Platform overrides ---
// --------------------------
void app_platform::do_run()
{
    fmt::println("app_platform::do_run");
    initialize();
//    main_window_ = create_system_window({300, 200}, "Hejsa");
    //    fmt::println("main_window: {}", (void*)main_window_.get());
}

void app_platform::do_pre_frame_update()
{

}

void app_platform::do_frame_update()
{

}

void app_platform::do_post_frame_update()
{

}

std::unique_ptr<system_window_base> app_platform::do_create_system_window(size_2d size, std::string_view title) const
{
    return std::make_unique<system_window_platform>(size, title);
}

void app_platform::initialize()
{
    fmt::println("app_platform::initialize()");
    const unsigned int init_flags{SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS};
    if (SDL_Init(init_flags) != 0) {
        exit_status_ = exit_status_t::failure;
    }

}


// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui

