#pragma once

#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/audio/cpaf_audio_types.h>

#if (CPAF_AUDIO_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/audio/cpaf_audio_system__sdl2.h>
#elif (CPAF_AUDIO_SYSTEM_ID == CPAF_SYSTEM_ID_GODOT)
#include <cpaf_libs/audio/cpaf_audio_system__godot.h>
#endif
namespace cpaf::audio {

class system : public system_platform {
public:
	using system_platform::system_platform;	// "Import" constructors to scope

	static system& instance();
};



} //END namespace cpaf::audio


