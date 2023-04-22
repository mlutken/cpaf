#include "cpaf_audio_types.h"



namespace cpaf::audio{


std::string to_string(channel_layout_t channel_layout)
{
    switch (channel_layout) {
    case channel_layout_t::mono:		return "mono"; break;
    case channel_layout_t::stereo:		return "stereo"; break;
    case channel_layout_t::surround2_1:	return "surround 2.1"; break;
    case channel_layout_t::quad:		return "quad"; break;
    case channel_layout_t::quad_center:	return "quad+center"; break;
    case channel_layout_t::surround5_1:	return "surround 5.1"; break;
    case channel_layout_t::surround6_1:	return "surround 6.1"; break;
    case channel_layout_t::surround7_1:	return "surround 7.1"; break;
    case channel_layout_t::SIZE:        return "SIZE"; break;
    case channel_layout_t::INVALID:     return "INVALID"; break;
    }
    return "";
}


std::string to_string(sample_format_t audio_format)
{
	switch (audio_format) {
	case sample_format_t::u8:		return "u8"; break;
	case sample_format_t::s8:		return "s8"; break;
	case sample_format_t::u16:		return "u16"; break;
	case sample_format_t::s16:		return "s16"; break;
	case sample_format_t::u16lsb:	return "u16lsb"; break;
	case sample_format_t::s16lsb:	return "s16lsb"; break;
	case sample_format_t::u16msb:	return "u16msb"; break;
	case sample_format_t::s16msb:	return "s16msb"; break;
	case sample_format_t::s32lsb:	return "s32lsb"; break;
	case sample_format_t::s32msb:	return "s32msb"; break;
	case sample_format_t::f32lsb:	return "f32lsb"; break;
	case sample_format_t::f32msb:	return "f32msb"; break;
	case sample_format_t::SIZE:	return "SIZE"; break;
	case sample_format_t::INVALID:	return "INVALID"; break;
	}
    return "";
}

std::string to_string(audio_format_t audio_format)
{
    std::string s = "Layout: " + to_string(audio_format.channel_layout);
    s.append( ", Format: " + to_string(audio_format.sample_format));
    s.append( ", Sample rate: " + std::to_string(audio_format.sample_rate));
    return s;
}




} //END namespace cpaf::audio
