#ifndef CPAF_AUDIO_DEVICE_H
#define CPAF_AUDIO_DEVICE_H

#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/audio/cpaf_audio_types.h>

#if (CPAF_AUDIO_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/audio/cpaf_audio_device__sdl2.h>
#endif
namespace cpaf::audio {

class device : public device_platform {
public:
	using device_platform::device_platform;	// "Import" constructors to scope

};



} //END namespace cpaf::audio


#endif // CPAF_AUDIO_DEVICE_H


