#pragma once

#include <functional>
#include <string>

namespace cpaf::audio {

/** \unit cpaf_audio_types.h
 *  Basic audio related types and enums
\author Martin Lutken
*/


enum class sample_format_t {
	  u8
	, s8
	, u16
	, s16
	, u16lsb
	, s16lsb
	, u16msb
	, s16msb
	, s32lsb
	, s32msb
	, f32lsb
	, f32msb
	, SIZE
	, INVALID
};

std::string to_string   (sample_format_t audio_format);

} //END namespace cpaf::audio

