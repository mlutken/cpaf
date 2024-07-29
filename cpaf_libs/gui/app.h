#pragma once

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/gui/base/app__sdl2.h>
#elif (CPAF_GUI_SYSTEM_ID == CPAF_SYSTEM_ID_GODOT)
#include <cpaf_libs/gui/base/app__godot.h>
#endif
#include <cpaf_libs/gui/system.h>
#include <cpaf_libs/audio/cpaf_audio_system.h>


namespace cpaf::gui {


class app : public app_platform {
public:
    using app_platform::app_platform;	// "Import" constructors to scope

    app(const app&) = delete;
    app(app&&) = delete;
    app& operator=(const app& other) = delete;
    app& operator=(app&& other) = delete;

};



} //END namespace cpaf::gui



