#ifndef CPAF_GRAPHICS_DEVICE_H
#define CPAF_GRAPHICS_DEVICE_H

#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/graphics/cpaf_graphics_types.h>

#if (CPAF_GRAPHICS_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/graphics/cpaf_graphics_surface__sdl2.h>
#endif
namespace cpaf::graphics {

class surface : public surface_platform {
public:
	using surface_platform::surface_platform;	// "Import" constructors to scope

};



} //END namespace cpaf::graphics


#endif // CPAF_GRAPHICS_DEVICE_H


