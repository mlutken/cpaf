#include "cpaf_audio_device_base.h"

#include <sstream>

namespace cpaf::audio {


std::string device_base::dbg_characteristics() const
{
    std::stringstream ss;
    ss  << "--- Audio device '" << device_name() << "' characteristics ---\n"
        << "  format          : " << to_string(sample_format()) << "\n"
        << "  sample_frequency: " << sample_frequency() << "\n"
        << "  channels_count  : " << channels_count() << "\n"
        << "  samples_count   : " << samples_count() << "\n"
        << "  buffer_size     : " << buffer_size()
        ;
    return ss.str();
}

// -------------------------------------------------------
// --- PROTECTED: Helper functions for derived classes ---
// -------------------------------------------------------

void device_base::execute_play_callback(uint8_t* buffer, int32_t buffer_max_len)
{
    play_callback_(buffer, static_cast<uint32_t>(buffer_max_len));
}


void device_base::changed_characteristics_reset()
{
    req_format_changed_				= false;
    req_sample_frequency_changed_	= false;
    req_channels_count_changed_		= false;
    req_samples_count_changed_		= false;
}





} // END namespace cpaf::audio

