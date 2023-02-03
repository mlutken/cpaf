#include "cpaf_audio_system_base.h"

#include <sstream>

namespace cpaf::audio {


// -----------------------
// --- Debug functions ---
// -----------------------

std::string system_base::dbg_play_devices_str(const std::string& caption) const
{
	std::stringstream ss;
	const size_t SIZE = play_device_names_.size();
	if (!caption.empty()) ss << "--- " << caption << " ---\n";
	for (size_t n = 0; n < SIZE; ++n ) {
		ss << "[" << n << "]"
		   << " : " << play_device_names_[n]
		   << "\n";
	}

	return ss.str();
}

std::string system_base::dbg_capture_devices_str(const std::string& caption) const
{
	std::stringstream ss;
	const size_t SIZE = capture_device_names_.size();
	if (!caption.empty()) ss << "--- " << caption << " ---\n";
	for (size_t n = 0; n < SIZE; ++n ) {
		ss << "[" << n << "]"
		   << " : " << capture_device_names_[n]
		   << "\n";
	}

	return ss.str();
}





} // END namespace cpaf::audio

