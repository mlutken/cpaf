#ifndef CPAF_GRAPHICS_MOUSE_H
#define CPAF_GRAPHICS_MOUSE_H

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GRAPHICS_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <graphics/input/mouse__sdl2.h>
#endif
namespace cpaf::graphics {

class mouse : public mouse_platform {
public:
    using mouse_platform::mouse_platform;	// "Import" constructors to scope

};



} //END namespace cpaf::graphics


#endif // CPAF_GRAPHICS_MOUSE_H


