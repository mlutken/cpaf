#pragma once

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/gui/base/system_render__sdl2.h>
#endif
namespace cpaf::gui {

class system_render : public system_render_platform {
public:
    using system_render_platform::system_render_platform;	// "Import" constructors to scope

    template <typename NATIVE_RENDER>
    NATIVE_RENDER*      native_renderer     ()      { return static_cast<NATIVE_RENDER*>(get_native_renderer()); }

};



} //END namespace cpaf::gui



