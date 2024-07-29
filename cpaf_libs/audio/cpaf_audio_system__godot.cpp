#include "cpaf_audio_system__godot.h"


namespace cpaf::audio {



// --------------------------
// --- Platform overrides ---
// --------------------------

int32_t system_platform::do_init()
{
    return -1;
}

int32_t system_platform::do_play_devices_count() const
{
	constexpr int iscapture = 0;
	return SDL_GetNumAudioDevices(iscapture);
}

int32_t system_platform::do_capture_devices_count() const
{
	constexpr int iscapture = 1;
	return SDL_GetNumAudioDevices(iscapture);
}

std::string system_platform::do_play_device_name(int32_t device_index) const
{
	constexpr int iscapture = 0;
	const char* name = SDL_GetAudioDeviceName(device_index, iscapture);
	if (name == nullptr) return "";
	return name;
}

std::string system_platform::do_capture_device_name(int32_t device_index) const
{
	constexpr int iscapture = 1;
	const char* name = SDL_GetAudioDeviceName(device_index, iscapture);
	if (name == nullptr) return "";
	return name;
}

// ------------------------
// --- PRIVATE: Helpers ---
// ------------------------
void system_platform::populate_device_lists()
{
    play_device_names_.clear();
    capture_device_names_.clear();
	const int32_t COUNT_PLAY = play_devices_count();
	for (int32_t n = 0; n < COUNT_PLAY; ++n) {
		play_device_names_.push_back(play_device_name(n));
	}

	const int32_t COUNT_CAPTURE = capture_devices_count();
	for (int32_t n = 0; n < COUNT_CAPTURE; ++n) {
		capture_device_names_.push_back(capture_device_name(n));
	}
}


} // END namespace cpaf::audio

