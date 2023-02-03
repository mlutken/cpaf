#ifndef CPAF_GRAPHICS_SYSTEM_H
#define CPAF_GRAPHICS_SYSTEM_H

#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/graphics/cpaf_graphics_types.h>

#if (CPAF_GRAPHICS_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/graphics/cpaf_graphics_system__sdl2.h>
#endif
namespace cpaf::graphics {

class system : public system_platform {
public:
	using system_platform::system_platform;	// "Import" constructors to scope

	static system& instance();
};



} //END namespace cpaf::graphics


#endif // CPAF_GRAPHICS_SYSTEM_H


