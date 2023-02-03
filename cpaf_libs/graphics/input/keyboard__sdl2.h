#ifndef CPAF_GRAPHICS_KEYBOARD_SDL2_H
#define CPAF_GRAPHICS_KEYBOARD_SDL2_H

#include <SDL2/SDL_video.h>
#include <config/cpaf_platform_definitions.h>
#include <graphics/input/keyboard_base.h>

#if (CPAF_GRAPHICS_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/graphics/input/keyboard_base.h when CPAF_GRAPHICS_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

namespace cpaf::graphics {

class keyboard_platform : public keyboard_base {
public:


private:
    // --- Platform overrides ---

    // --- PRIVATE: Helpers  ---
};


} //END namespace cpaf::graphics


#endif // CPAF_GRAPHICS_KEYBOARD_SDL2_H


