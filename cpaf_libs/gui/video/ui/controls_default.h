#pragma once

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GRAPHICS_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/gui/video/ui/controls_default__sdl2.h>
#elif (CPAF_GRAPHICS_SYSTEM_ID == CPAF_SYSTEM_ID_GODOT)
#include <cpaf_libs/gui/video/ui/controls_default__godot.h>
#endif

namespace cpaf::gui::video {

class controls_default : public controls_default_platform
{
public:
    using controls_default_platform::controls_default_platform;	// "Import" constructors to scope

};

} //END namespace cpaf::gui::video
