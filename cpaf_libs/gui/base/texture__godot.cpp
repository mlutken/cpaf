#include "texture__godot.h"
#include <fmt/format.h>

#include <cpaf_libs/gui/system_render.h>
#include <cpaf_libs/gui/images/png_image.h>


namespace cpaf::gui {


// --------------------------
// --- Platform overrides ---
// --------------------------

texture_platform::~texture_platform()
{
    fmt::println("texture_platform::DESTRUCTOR ()");
    destroy();
}

texture_platform::texture_platform(const size_2d& /*size*/)
    : texture_base()
{
}

// TODO_GODOT
void texture_platform::destroy()
{
}

// TODO_GODOT
void texture_platform::do_update_blendmode()
{
}

// TODO_GODOT
void* texture_platform::do_get_native_texture() const
{
    return nullptr;
}

// TODO_GODOT
size_2d texture_platform::do_get_size() const
{
    return {-1, -1};
}

// TODO_GODOT
bool texture_platform::do_load_from_file(const std::filesystem::path& local_path)
{
    return false;
}

// TODO_GODOT
bool texture_platform::do_png_from_memory(const unsigned char* const data_ptr, size_t data_size)
{
    return false;
}

// TODO_GODOT
bool texture_platform::do_start_surface_pixel_access(size_2d size)
{
    return false;
}

// TODO_GODOT
bool texture_platform::do_end_surface_pixel_access()
{
    return false;
}

// TODO_GODOT
uint32_t* texture_platform::do_pixel_data_raw_ptr()
{
    return nullptr;
}

// TODO_GODOT
void texture_platform::do_set_pixel(int32_t x, int32_t y, color pixel_color)
{
}

// TODO_GODOT
void texture_platform::do_set_pixel(int32_t x, int32_t y, uint32_t pixel_color)
{
}

// TODO_GODOT
void texture_platform::do_set_pixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
}

// TODO_GODOT
void texture_platform::do_draw_rect(const rect& dst_rect, const color& rect_color)
{
}



// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui

