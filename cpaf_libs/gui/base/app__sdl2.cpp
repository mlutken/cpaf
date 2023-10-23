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

    // SDL_DestroyTexture(video_frame_render_texture);
    // SDL_DestroyRenderer(sdl_renderer);
    if (main_window_) {
        SDL_DestroyWindow(main_window_);
    }
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
void app_platform::do_start_run()
{
    fmt::println("app_platform::do_run");
    initialize();
    is_running_ = true;
}

events::event app_platform::do_get_event() const
{
    SDL_Event sdl_event{};
    if (SDL_PollEvent(&sdl_event) == 1) {


        return events_converter_.convert_event(sdl_event);
    }

    return events::event::create_none();
}

void app_platform::do_process_events()
{
    SDL_Event event{};
    while (SDL_PollEvent(&event) == 1) {
        //      APP_PROFILE_SCOPE("EventPolling");

        // ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT) {
            is_running_ = false;
            break;
        }

        // if (event.type == SDL_WINDOWEVENT &&
        //     event.window.windowID == SDL_GetWindowID(m_window->get_native_window())) {
        //     on_event(event.window);
        // }
    }
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

/// @todo implement me!!
size_2d app_platform::do_main_window_size() const
{
    return size_2d{1,1};
}

std::unique_ptr<system_window_base> app_platform::do_create_system_window(size_2d size, std::string_view title) const
{
    return std::make_unique<system_window_platform>(size, title);
}

void app_platform::initialize()
{
    fmt::println("app_platform::initialize()");
    const uint32_t init_flags{SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS};
    if (SDL_Init(init_flags) != 0) {
        exit_status_ = exit_status_t::failure;
    }

    const uint32_t window_flags{SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI};

    // sdl_window = SDL_CreateWindow("Video playground 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_dimensions.x(), window_dimensions.y(), SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

    // sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    // video_frame_render_texture = SDL_CreateTexture(
    //     sdl_renderer,
    //     SDL_PIXELFORMAT_YV12,
    //     SDL_TEXTUREACCESS_STREAMING,
    //     window_dimensions.x(),
    //     window_dimensions.y()
    //     );

    //SDL_WINDOWPOS_UNDEFINED,
    main_window_ = SDL_CreateWindow(window_title().data(),
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                initial_window_size_.width(),
                                initial_window_size_.height(),
                                window_flags);

    // auto renderer_flags{
    //                     static_cast<SDL_RendererFlags>(SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED)};
    // m_renderer = SDL_CreateRenderer(m_window, -1, renderer_flags);

    // if (m_renderer == nullptr) {
    //     //    APP_ERROR("Error creating SDL_Renderer!");
    //     return;
    // }

}


// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui

