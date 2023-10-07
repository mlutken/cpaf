#pragma once

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/gui/base/system_window__sdl2.h>
#endif
namespace cpaf::gui {

class system_window : public system_window_platform {
public:
    using system_window_platform::system_window_platform;	// "Import" constructors to scope

};



} //END namespace cpaf::gui



