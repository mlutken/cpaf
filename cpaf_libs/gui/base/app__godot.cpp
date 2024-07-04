#include "app__godot.h"
#include <unordered_map>
#include <fmt/format.h>

#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/base/system_window__godot.h>
#include <cpaf_libs/gui/fonts/font_size.h>
#include <cpaf_libs/gui/system_render.h>

using namespace std;

namespace cpaf::gui {

app_platform::app_platform()
{
    fmt::println("app_platform::CONSTRUCTOR GODOT!");

//    main_window_ = create_system_window({300, 200}, "Hello GUI");
}

app_platform::~app_platform()
{
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
    // imgui_fonts::instance().init(main_window().renderer().maximum_texture_dimensions());
}

events::event app_platform::do_platform_get_event() const
{
    return events::event::create_none();
}

void app_platform::do_platform_process_events()
{
}

/// @todo imgui_fonts::instance().add_pending_requested_fonts(); still crashes! :(
void app_platform::do_platform_pre_frame_update()
{
}

void app_platform::do_platform_frame_update()
{
}



void app_platform::do_platform_post_frame_update()
{
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
}

void app_platform::do_add_fonts(const std::string& font_name,
                                float size_adjust_factor,
                                uint32_t range_min,
                                uint32_t range_max,
                                const std::vector<uint32_t>& sizes_in_points)
{
}

void app_platform::do_add_fonts(const std::string& font_name,
                                const std::string& merge_font_name,
                                float merge_size_adjust_factor,
                                uint32_t range_min,
                                uint32_t range_max,
                                const std::vector<uint32_t>& sizes_in_points)
{
}


void app_platform::do_set_default_font(const std::string& font_name, uint32_t size_points)
{
}

std::unique_ptr<system_window> app_platform::do_create_system_window(size_2d size, std::string_view title) const
{
    return std::unique_ptr<system_window>(new system_window(size, title));
}

void app_platform::initialize()
{
    fmt::println("app_platform::initialize()");

    main_window_ptr_ = do_create_system_window(initial_window_size_, window_title());
}


// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui

