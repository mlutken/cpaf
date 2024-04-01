#pragma once

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/gui/base/texture__sdl2.h>
#endif
namespace cpaf::gui {

class texture : public texture_platform {
public:
    using texture_platform::texture_platform;	// "Import" constructors to scope

//    template <typename NATIVE_WINDOW>
//    NATIVE_WINDOW*      native_window       ()      { return static_cast<NATIVE_WINDOW*>(get_native_window());   }

//    template <typename NATIVE_RENDER>
//    NATIVE_RENDER*      native_renderer     ()      { return static_cast<NATIVE_RENDER*>(get_native_renderer()); }

};



} //END namespace cpaf::gui



