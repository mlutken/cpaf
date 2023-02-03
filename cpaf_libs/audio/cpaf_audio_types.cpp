#include "cpaf_audio_types.h"



namespace cpaf::audio{


std::string to_string(format_t audio_format)
{
	switch (audio_format) {
	case format_t::u8:		return "u8"; break;
	case format_t::s8:		return "s8"; break;
	case format_t::u16:		return "u16"; break;
	case format_t::s16:		return "s16"; break;
	case format_t::u16lsb:	return "u16lsb"; break;
	case format_t::s16lsb:	return "s16lsb"; break;
	case format_t::u16msb:	return "u16msb"; break;
	case format_t::s16msb:	return "s16msb"; break;
	case format_t::s32lsb:	return "s32lsb"; break;
	case format_t::s32msb:	return "s32msb"; break;
	case format_t::f32lsb:	return "f32lsb"; break;
	case format_t::f32msb:	return "f32msb"; break;
	case format_t::SIZE:	return "SIZE"; break;
	case format_t::INVALID:	return "INVALID"; break;
	}
	return "";
}



} //END namespace cpaf::audio
