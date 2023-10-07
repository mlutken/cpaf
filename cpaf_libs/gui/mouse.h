#pragma once

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/gui/base/mouse__sdl2.h>
#endif
namespace cpaf::gui {

class mouse : public mouse_platform {
public:
    using mouse_platform::mouse_platform;	// "Import" constructors to scope

};



} //END namespace cpaf::gui



