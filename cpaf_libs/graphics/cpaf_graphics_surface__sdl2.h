#ifndef CPAF_GRAPHICS_DEVICE_SDL2_H
#define CPAF_GRAPHICS_DEVICE_SDL2_H

#include <SDL2/SDL_video.h>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/graphics/cpaf_graphics_surface_base.h>

#if (CPAF_GRAPHICS_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/graphics/cpaf_graphics_surface__sdl2.h when CPAF_GRAPHICS_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

namespace cpaf::graphics {

// https://codereview.stackexchange.com/questions/202012/simple-c-sdl2-wrapper-for-small-game
// https://github.com/libSDL2pp/libSDL2pp/tree/master/SDL2pp
// https://discourse.libsdl.org/

class surface_platform : public surface_base {
public:


private:
    // --- Platform overrides ---

    // --- PRIVATE: Helpers  ---
};


} //END namespace cpaf::graphics


#endif // CPAF_GRAPHICS_DEVICE_SDL2_H


