#pragma once

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/gui/base/app__sdl2.h>
#endif
namespace cpaf::gui {

class app : public app_platform {
public:
    using app_platform::app_platform;	// "Import" constructors to scope

};



} //END namespace cpaf::gui



