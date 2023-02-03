#ifndef CPAF_GRAPHICS_KEYBOARD_H
#define CPAF_GRAPHICS_KEYBOARD_H

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GRAPHICS_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <graphics/input/keyboard__sdl2.h>
#endif
namespace cpaf::graphics {

class keyboard : public keyboard_platform {
public:
    using keyboard_platform::keyboard_platform;	// "Import" constructors to scope

};



} //END namespace cpaf::graphics


#endif // CPAF_GRAPHICS_KEYBOARD_H


