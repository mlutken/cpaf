#ifndef CPAF_AUDIO_SYSTEM_SDL2_H
#define CPAF_AUDIO_SYSTEM_SDL2_H

#include <SDL2/SDL_audio.h>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/audio/cpaf_audio_system_base.h>

#if (CPAF_AUDIO_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/audio/cpaf_audio_system__sdl2.h when CPAF_AUDIO_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

namespace cpaf::audio {


class system_platform : public system_base {
public:


private:
	// --- Platform overrides ---
	int32_t			do_init						() override;
	int32_t			do_play_devices_count		() const override;
	int32_t			do_capture_devices_count	() const override;

	std::string		do_play_device_name			(int32_t device_index) const override;
	std::string		do_capture_device_name		(int32_t device_index) const override;

	// --- PRIVATE: Helpers  ---
	void			populate_device_lists		();
};


} //END namespace cpaf::audio


#endif // CPAF_AUDIO_SYSTEM_SDL2_H


