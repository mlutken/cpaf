#pragma once
#include <config/cpaf_platform_definitions.h>

#if (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/gui/base/system__sdl2.h>
#elif (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_GODOT)
#include <cpaf_libs/gui/base/system__godot.h>
#endif
namespace cpaf::gui {

class system : public system_platform {
public:
	using system_platform::system_platform;	// "Import" constructors to scope

	static system& instance();
};



} //END namespace cpaf::gui




