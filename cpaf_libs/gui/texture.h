#pragma once

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/gui/base/texture__sdl2.h>
#elif (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_GODOT)
#include <cpaf_libs/gui/base/texture__godot.h>
#endif
namespace cpaf::gui {

class texture : public texture_platform {
public:
    using texture_platform::texture_platform;	// "Import" constructors to scope


    template <typename NATIVE_TEXTURE>
    NATIVE_TEXTURE*     native_texture    ()  const    { return static_cast<NATIVE_TEXTURE*>(get_native_texture()); }

//    template <typename NATIVE_TEXTURE>
//    const NATIVE_TEXTURE*     native_texture    () const{ return static_cast<NATIVE_TEXTURE*>(get_native_texture()); }

};



} //END namespace cpaf::gui



