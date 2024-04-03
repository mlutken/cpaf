#include "texture__sdl2.h"
#include <unordered_map>
#include <fmt/format.h>

#include <cpaf_libs/gui/system_render.h>
#include <cpaf_libs/gui/platform_utils/sdl_convert.h>
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

void texture_platform::destroy()
{
    if (sdl_texture_) {
        SDL_DestroyTexture(sdl_texture_);
        sdl_texture_ = nullptr;
    }
    if (sdl_surface_) {
        SDL_FreeSurface(sdl_surface_);
        sdl_surface_ = nullptr;
    }
}

void texture_platform::do_update_blendmode()
{
    if (sdl_texture_) {
        SDL_SetTextureBlendMode(sdl_texture_, to_sdl_blendmode(blendmode_));
    }
}

void* texture_platform::do_get_native_texture() const
{
    return sdl_texture_;
}

size_2d texture_platform::do_get_size() const
{
    if (sdl_surface_) {
        return {sdl_surface_->w, sdl_surface_->h};
    }

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

/*
 * @see https://wiki.libsdl.org/SDL2_image/IMG_LoadTextureTyped_RW
 * @see https://wiki.libsdl.org/SDL2_image/IMG_LoadTexture
 * @see https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurfaceFrom
 *
 */
bool texture_platform::do_load_from_file(const std::filesystem::path& local_path)
{
    if (!render_) { return false; }
    if (sdl_surface_) { SDL_FreeSurface(sdl_surface_);  sdl_surface_ = nullptr; }

    const auto file_ext = local_path.extension();
    if (file_ext == ".png") {
        png_image png;
        png.open_local(local_path);
        start_surface_pixel_access(png.size());
        png.copy_pixels_out(pixel_data_raw_ptr());
        end_surface_pixel_access();
    }
    else if (file_ext == ".bmp") {
        auto surface = SDL_LoadBMP(local_path.string().c_str());
        sdl_texture_ = SDL_CreateTextureFromSurface(render_->native_renderer<SDL_Renderer>(), surface);
        SDL_FreeSurface(surface);
    }
    else {
        std::cerr << "LOG_ERR Unknown texture::load_from_file unsupported format '" << file_ext << "'\n";
    }
    do_update_blendmode();

    return sdl_texture_ != nullptr;
}

bool texture_platform::do_png_from_memory(const unsigned char* const data_ptr, size_t data_size)
{
    if (!render_) { return false; }
    png_image png;
    png.open_from_memory(data_ptr, data_size);
    start_surface_pixel_access(png.size());
    png.copy_pixels_out(pixel_data_raw_ptr());
    end_surface_pixel_access();

    do_update_blendmode();

    return sdl_texture_ != nullptr;
}

// @see https://wiki.libsdl.org/SDL2/SDL_CreateRGBSurface
bool texture_platform::do_start_surface_pixel_access(size_2d size)
{
    if (sdl_surface_) { SDL_FreeSurface(sdl_surface_);  sdl_surface_ = nullptr; }
    constexpr uint32_t flags = 0; // Must be zero accordning to docs
    constexpr int32_t depth = 32; // For now we support 32 bit only!
    sdl_surface_ = SDL_CreateRGBSurface(flags, size.width(), size.height(), depth,  bitmasks::rmask, bitmasks::gmask, bitmasks::bmask, bitmasks::amask);

    return sdl_surface_ != nullptr;
}

bool texture_platform::do_end_surface_pixel_access()
{
    if (!sdl_surface_) {
        return false;
    }
    sdl_texture_ = SDL_CreateTextureFromSurface(render_->native_renderer<SDL_Renderer>(), sdl_surface_);
    SDL_FreeSurface(sdl_surface_);
    sdl_surface_ = nullptr;
    do_update_blendmode();

    return sdl_texture_ != nullptr;
}

uint32_t* texture_platform::do_pixel_data_raw_ptr()
{
    if (!sdl_surface_) {
        return nullptr;
    }
    return static_cast<uint32_t*>(sdl_surface_->pixels);
}

/// @todo This can most likely be optimized!
void texture_platform::do_set_pixel(int32_t x, int32_t y, color pixel_color)
{
    const auto u8col = pixel_color.to_u8color();
    const auto col = pixel_from_rgba(u8col.r(), u8col.g(), u8col.b(), u8col.a());
    do_set_pixel(x, y, col);
}

void texture_platform::do_set_pixel(int32_t x, int32_t y, uint32_t pixel_color)
{
    uint32_t* const target_pixel = (uint32_t *) ((uint8_t *) sdl_surface_->pixels
                                             + y * sdl_surface_->pitch
                                             + x * sdl_surface_->format->BytesPerPixel);
    *target_pixel = pixel_color;
}

void texture_platform::do_set_pixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    do_set_pixel(x, y, pixel_from_rgba(r, g, b, a));
}

void texture_platform::do_draw_rect(const rect& dst_rect, const color& rect_color)
{
    const auto u8col = rect_color.to_u8color();
    const auto col = pixel_from_rgba(u8col.r(), u8col.g(), u8col.b(), u8col.a());
    const auto r = rect_i32(dst_rect);

    const auto x_end = r.lower_right().x();
    const auto y_end = r.lower_right().y();
    for (auto y = r.top_left().y(); y < y_end; ++y) {
        for (auto x = r.top_left().x(); x < x_end; ++x) {
            do_set_pixel(x, y, col);
        }
    }
}



// -------------------------
// --- PRIVATE: Helpers  ---
// -------------------------




} // END namespace cpaf::gui

