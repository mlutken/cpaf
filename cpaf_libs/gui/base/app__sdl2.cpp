#include "app__sdl2.h"
#include <unordered_map>
#include <fmt/format.h>
#include <SDL2/SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>

#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/base/system_window__sdl2.h>
#include <cpaf_libs/gui/fonts/font_size.h>
#include <cpaf_libs/gui/assets/fonts/imgui_fonts.h>
#include <cpaf_libs/gui/system_render.h>

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
    // if (main_window_) {
    //     SDL_DestroyWindow(main_window_);
    // }
    // if (main_renderer_) {
    //     SDL_DestroyRenderer(main_renderer_);
    // }
    // SDL_Quit();
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
void app_platform::do_platform_start_run()
{
    fmt::println("app_platform::do_platform_start_run()");
    initialize();
    is_running_ = true;
    imgui_fonts::instance().init(main_window().renderer().maximum_texture_dimensions());
}

events::event app_platform::do_platform_get_event() const
{
    SDL_Event sdl_event{};
    if (SDL_PollEvent(&sdl_event) == 1) {

        ImGui_ImplSDL2_ProcessEvent(&sdl_event);
        // TODO: Only create and return events that ImGui does not handle!
        //       io.WantCaptureMouse, io.WantCaptureKeyboard

        return events_converter_.convert_event(sdl_event);
    }

    return events::event::create_none();
}

void app_platform::do_platform_process_events()
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

/// @todo imgui_fonts::instance().add_pending_requested_fonts(); still crashes! :(
void app_platform::do_platform_pre_frame_update()
{
    if (clear_window_before_frame_update_) {
        SDL_RenderClear(main_window().native_renderer<SDL_Renderer>());
    }
    /// imgui_fonts::instance().add_pending_requested_fonts(); // Still crashes :(
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void app_platform::do_platform_frame_update()
{
    // ImGui::ShowDemoWindow();
}



void app_platform::do_platform_post_frame_update()
{
    ImGui::Render();
    /// imgui_fonts::instance().add_pending_requested_fonts(); // Hmm crashes for some reason
    // ImGui::UpdatePlatformWindows();
    // ImGui::RenderPlatformWindowsDefault();
    // SDL_SetRenderDrawColor(main_window().native_renderer<SDL_Renderer>(), 100, 100, 100, 255);

    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(main_window().native_renderer<SDL_Renderer>());
}

/// @todo implement me!!
size_2d app_platform::do_platform_main_window_size() const
{
    if (!main_window_ptr_) { return {-1,-1}; }
    return main_window_ptr_->size();
}

system_window& app_platform::do_main_window()
{
    return *main_window_ptr_;
}

std::shared_ptr<system_window> app_platform::do_main_window_shared() const
{
    return main_window_ptr_;
}

void app_platform::do_add_fonts(const std::string& font_name, const std::vector<uint32_t>& sizes_in_points)
{
    for (auto size_points: sizes_in_points) {
        const auto size_pixels = font_size::to_pixels_display(size_points, main_window().display_index());
        imgui_fonts::instance().add(font_name, size_pixels);
    }
}

void app_platform::do_add_fonts(const std::string& font_name,
                                float size_adjust_factor,
                                uint32_t range_min,
                                uint32_t range_max,
                                const std::vector<uint32_t>& sizes_in_points)
{
    for (auto size_points: sizes_in_points) {
        const auto size_pixels = font_size::to_pixels_display(size_points, main_window().display_index());
        imgui_fonts::instance().add(font_name, size_pixels, size_adjust_factor, range_min, range_max);
    }
}

void app_platform::do_add_fonts(const std::string& font_name,
                                const std::string& merge_font_name,
                                float merge_size_adjust_factor,
                                uint32_t range_min,
                                uint32_t range_max,
                                const std::vector<uint32_t>& sizes_in_points)
{
    for (auto size_points: sizes_in_points) {
        const auto size_pixels = font_size::to_pixels_display(size_points, main_window().display_index());
        imgui_fonts::instance().add(font_name, size_pixels);
        imgui_fonts::instance().merge_with_previous(merge_font_name, size_pixels, merge_size_adjust_factor, range_min, range_max);
    }
}


void app_platform::do_set_default_font(const std::string& font_name, uint32_t size_points)
{
    const auto size_pixels = font_size::to_pixels_display(size_points, main_window().display_index());
    imgui_fonts::instance().set_default(font_name, size_pixels);
}

std::unique_ptr<system_window> app_platform::do_create_system_window(size_2d size, std::string_view title) const
{
    return std::unique_ptr<system_window>(new system_window(size, title));
}

void app_platform::initialize()
{
    fmt::println("app_platform::initialize()");


    // const uint32_t window_flags{SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI};

    // sdl_window = SDL_CreateWindow("Video playground 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_dimensions.x(), window_dimensions.y(), SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

    // sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    // video_frame_render_texture = SDL_CreateTexture(
    //     sdl_renderer,
    //     SDL_PIXELFORMAT_YV12,
    //     SDL_TEXTUREACCESS_STREAMING,
    //     window_dimensions.x(),
    //     window_dimensions.y()
    //     );

    main_window_ptr_ = do_create_system_window(initial_window_size_, window_title());

    // //SDL_WINDOWPOS_UNDEFINED,
    // main_window_ = SDL_CreateWindow(window_title().data(),
    //                             SDL_WINDOWPOS_CENTERED,
    //                             SDL_WINDOWPOS_CENTERED,
    //                             initial_window_size_.width(),
    //                             initial_window_size_.height(),
    //                             window_flags);


    // auto renderer_flags{
    //                     static_cast<SDL_RendererFlags>(SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED)};
    // main_renderer_ = SDL_CreateRenderer(main_window_, -1, renderer_flags);

    // auto renderer_flags{
    //                     static_cast<SDL_RendererFlags>(SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED)};
    // m_renderer = SDL_CreateRenderer(m_window, -1, renderer_flags);

    // if (m_renderer == nullptr) {
    //     //    APP_ERROR("Error creating SDL_Renderer!");
    //     return;
    // }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io{ImGui::GetIO()};
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable |
                      ImGuiConfigFlags_ViewportsEnable;

////    imgui_fonts::instance().add(default_font(), {8,11,14,18,22,28,36,48,96});
////    imgui_fonts::instance().set_default(default_font(), 18);

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(main_window().native_window<SDL_Window>(), main_window().native_renderer<SDL_Renderer>());
    ImGui_ImplSDLRenderer_Init(main_window().native_renderer<SDL_Renderer>());

}


// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui

