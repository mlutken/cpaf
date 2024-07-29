#pragma once

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/gui/base/keyboard__sdl2.h>
#elif (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_GODOT)
#include <cpaf_libs/gui/base/keyboard__godot.h>
#endif

namespace cpaf::gui {

class keyboard : public keyboard_platform {
public:
    using keyboard_platform::keyboard_platform;	// "Import" constructors to scope

};



} //END namespace cpaf::gui



