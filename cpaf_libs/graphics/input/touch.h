#ifndef CPAF_GRAPHICS_TOUCH_H
#define CPAF_GRAPHICS_TOUCH_H

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GRAPHICS_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <graphics/input/touch__sdl2.h>
#endif
namespace cpaf::graphics {

class touch : public touch_platform {
public:
    using touch_platform::touch_platform;	// "Import" constructors to scope

};



} //END namespace cpaf::graphics


#endif // CPAF_GRAPHICS_TOUCH_H


