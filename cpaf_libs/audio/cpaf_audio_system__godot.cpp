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
    return -1;
}

int32_t system_platform::do_capture_devices_count() const
{
    return -1;
}

std::string system_platform::do_play_device_name(int32_t /*device_index*/) const
{
    return "";
}

std::string system_platform::do_capture_device_name(int32_t /*device_index*/) const
{
    return "";
}

// ------------------------
// --- PRIVATE: Helpers ---
// ------------------------
// void system_platform::populate_device_lists()
// {
// }


} // END namespace cpaf::audio

