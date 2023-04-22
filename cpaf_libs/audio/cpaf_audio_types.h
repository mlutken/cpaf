#pragma once

#include <functional>
#include <string>

namespace cpaf::audio {

/** \unit cpaf_audio_types.h
 *  Basic audio related types and enums
\author Martin Lutken
*/


enum class channel_layout_t {
    mono,
    stereo,
    surround2_1,
    quad,
    quad_center,
    surround5_1,
    surround6_1,
    surround7_1,
    SIZE,
    INVALID
};

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


struct audio_format_t
{
    channel_layout_t		channel_layout		= channel_layout_t::stereo;
    sample_format_t         sample_format       = sample_format_t::s16;
    int32_t                 sample_rate         = 44100;
};



std::string to_string   (channel_layout_t channel_layout);
std::string to_string   (sample_format_t audio_format);
std::string to_string   (audio_format_t audio_format);

} //END namespace cpaf::audio

