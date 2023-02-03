#ifndef CPAF_GRAPHICS_SYSTEM_SDL2_H
#define CPAF_GRAPHICS_SYSTEM_SDL2_H

#include <SDL2/SDL_video.h>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/graphics/cpaf_graphics_system_base.h>

#if (CPAF_GRAPHICS_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/graphics/cpaf_graphics_system__sdl2.h when CPAF_GRAPHICS_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

namespace cpaf::graphics {


class system_platform : public system_base {
public:


private:
	// --- Platform overrides ---

	// --- PRIVATE: Helpers  ---
};


} //END namespace cpaf::graphics


#endif // CPAF_GRAPHICS_SYSTEM_SDL2_H


