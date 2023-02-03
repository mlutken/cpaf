#ifndef CPAF_GRAPHICS_MOUSE_SDL2_H
#define CPAF_GRAPHICS_MOUSE_SDL2_H

#include <SDL2/SDL_video.h>
#include <config/cpaf_platform_definitions.h>
#include <graphics/input/mouse_base.h>

#if (CPAF_GRAPHICS_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/graphics/input/mouse_base.h when CPAF_GRAPHICS_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

namespace cpaf::graphics {

class mouse_platform : public mouse_base {
public:


private:
    // --- Platform overrides ---

    // --- PRIVATE: Helpers  ---
};


} //END namespace cpaf::graphics


#endif // CPAF_GRAPHICS_MOUSE_SDL2_H


