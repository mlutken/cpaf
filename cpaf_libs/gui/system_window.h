#pragma once

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/gui/base/system_window__sdl2.h>
#endif
namespace cpaf::gui {

class system_window : public system_window_platform {
public:
    using system_window_platform::system_window_platform;	// "Import" constructors to scope

    template <typename NATIVE_WINDOW>
    NATIVE_WINDOW*      native_window       ()      { return static_cast<NATIVE_WINDOW*>(get_native_window());   }

    template <typename NATIVE_RENDER>
    NATIVE_RENDER*      native_renderer     ()      { return static_cast<NATIVE_RENDER*>(get_native_renderer()); }

};



} //END namespace cpaf::gui



