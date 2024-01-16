#pragma once

#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/gui_types.h>

#if (CPAF_GRAPHICS_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/gui/platform_utils/sdl_convert.h when CPAF_GRAPHICS_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

#include <SDL2/SDL.h>

namespace cpaf::gui {
SDL_Rect            to_sdl_rect             (render_geometry_t geom);
render_geometry_t   to_render_geometry      (SDL_Rect rect);


} // namespace cpaf::gui

