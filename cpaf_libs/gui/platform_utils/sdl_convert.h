#pragma once

#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/gui_types.h>

#if (CPAF_GRAPHICS_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/gui/platform_utils/sdl_convert.h when CPAF_GRAPHICS_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

#include <SDL2/SDL.h>

namespace cpaf::gui {
SDL_Rect            to_sdl_rect             (rect geom);
rect                to_rect                 (SDL_Rect rect);

SDL_BlendMode       to_sdl_blendmode        (blend_mode_t blendmode);

uint32_t            to_sdl_window_mode      (window_mode_t wm);
window_mode_t       to_window_mode          (uint32_t sdl_window_mode);


inline uint32_t     pixel_from_rgba         (uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    return static_cast<uint32_t>((r << 24) | (g << 16) | (b << 8) | a);
#else
    return static_cast<uint32_t>((a << 24) | (b << 16) | (g << 8) | r);
#endif
}

struct bitmasks {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    static constexpr uint32_t rmask = 0xff000000;
    static constexpr uint32_t gmask = 0x00ff0000;
    static constexpr uint32_t bmask = 0x0000ff00;
    static constexpr uint32_t amask = 0x000000ff;
#else
    static constexpr uint32_t rmask = 0x000000ff;
    static constexpr uint32_t gmask = 0x0000ff00;
    static constexpr uint32_t bmask = 0x00ff0000;
    static constexpr uint32_t amask = 0xff000000;
#endif
};


} // namespace cpaf::gui

