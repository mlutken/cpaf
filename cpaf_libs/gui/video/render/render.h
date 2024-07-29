#pragma once

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GRAPHICS_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/gui/video/render/render__sdl2.h>
#elif (CPAF_GRAPHICS_SYSTEM_ID == CPAF_SYSTEM_ID_GODOT)
#include <cpaf_libs/gui/video/render/render__godot.h>
#endif

namespace cpaf::gui::video {

class render : public render_platform
{
public:
    using render_platform::render_platform;	// "Import" constructors to scope

};

} //END namespace cpaf::gui::video
