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
system_window_platform::system_window_platform(const size_2d& size)
    : system_window_base(size)
{
    fmt::println("system_window_platform::CONSTRUCTOR {}, {}", size.width(), size.height() );

}

system_window_platform::system_window_platform(const size_2d& size, std::string_view title)
    : system_window_base(size, title)
{
    fmt::println("system_window_platform::CONSTRUCTOR ({}) {}, {}", title, size.width(), size.height() );
}

size_2d system_window_platform::do_get_size()
{
    return {0,0};
}

std::string system_window_platform::do_get_title()
{
    return "Title FIXMENM TODO";
}


// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui

