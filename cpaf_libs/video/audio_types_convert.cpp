
#include "audio_types_convert.h"

#include <iostream>
#include <array>

extern "C"
{
#include <libavutil/channel_layout.h>
}


using namespace std;
namespace cpaf::video {



int32_t sample_format_byte_count(AVSampleFormat format)
{
    switch (format) {
    case AV_SAMPLE_FMT_NONE :	return 0; break;
    case AV_SAMPLE_FMT_U8 :		return 1; break;
    case AV_SAMPLE_FMT_S16 :	return 2; break;
    case AV_SAMPLE_FMT_S32 :	return 4; break;
    case AV_SAMPLE_FMT_FLT :	return 4; break;
    case AV_SAMPLE_FMT_DBL :	return 8; break;
    case AV_SAMPLE_FMT_U8P :	return 1; break;
    case AV_SAMPLE_FMT_S16P :	return 2; break;
    case AV_SAMPLE_FMT_S32P :	return 4; break;
    case AV_SAMPLE_FMT_FLTP :	return 4; break;
    case AV_SAMPLE_FMT_DBLP :	return 8; break;
    case AV_SAMPLE_FMT_S64 :	return 8; break;
    case AV_SAMPLE_FMT_S64P :	return 8; break;
    case AV_SAMPLE_FMT_NB :		return 0; break;
    }
    return 0;
}

/**
@sa https://thehometheaterdiy.com/surround-sound-channels-explained/
*/
int32_t ff_channel_layout_to_count(const uint64_t channel_layout)
{
    switch (channel_layout) {
    case AV_CH_LAYOUT_MONO: return 1; break;
    case AV_CH_LAYOUT_STEREO: return 2; break;
    case AV_CH_LAYOUT_2POINT1: return 3; break;
    case AV_CH_LAYOUT_2_1: return 3; break;
    case AV_CH_LAYOUT_SURROUND: return 3; break;
    case AV_CH_LAYOUT_3POINT1: return 4; break;
    case AV_CH_LAYOUT_4POINT0: return 4; break;
    case AV_CH_LAYOUT_4POINT1: return 5; break;
    case AV_CH_LAYOUT_2_2: return 4; break;
    case AV_CH_LAYOUT_QUAD: return 4; break;
    case AV_CH_LAYOUT_5POINT0: return 5; break;
    case AV_CH_LAYOUT_5POINT1: return 6; break;
    case AV_CH_LAYOUT_5POINT0_BACK: return 5; break;
    case AV_CH_LAYOUT_5POINT1_BACK: return 6; break;
    case AV_CH_LAYOUT_6POINT0: return 6; break;
    case AV_CH_LAYOUT_6POINT0_FRONT: return 6; break;
    case AV_CH_LAYOUT_HEXAGONAL: return 6; break;
    case AV_CH_LAYOUT_6POINT1: return 7; break;
    case AV_CH_LAYOUT_6POINT1_BACK: return 7; break;
    case AV_CH_LAYOUT_6POINT1_FRONT: return 7; break;
    case AV_CH_LAYOUT_7POINT0: return 7; break;
    case AV_CH_LAYOUT_7POINT0_FRONT: return 7; break;
    case AV_CH_LAYOUT_7POINT1: return 8; break;
    case AV_CH_LAYOUT_7POINT1_WIDE: return 8; break;
    case AV_CH_LAYOUT_7POINT1_WIDE_BACK: return 8; break;
    case AV_CH_LAYOUT_OCTAGONAL: return 8; break;
    case AV_CH_LAYOUT_HEXADECAGONAL: return 10; break;  /// @todo  No clue how many channels HEXADECAGONAL refers to!
    case AV_CH_LAYOUT_STEREO_DOWNMIX: return 2; break;
    default:
        return 2;
        std::cerr << "ERROR: ff_channel_layout_to_count, unknown channel_layout '" << channel_layout << "'\n";
        break;
    }
    return 0;
}

string to_string(AVSampleFormat format)
{
    switch (format) {
    case AV_SAMPLE_FMT_NONE :	return "AV_SAMPLE_FMT_NONE"; break;
    case AV_SAMPLE_FMT_U8 :		return "AV_SAMPLE_FMT_U8"; break;
    case AV_SAMPLE_FMT_S16 :	return "AV_SAMPLE_FMT_S16"; break;
    case AV_SAMPLE_FMT_S32 :	return "AV_SAMPLE_FMT_S32"; break;
    case AV_SAMPLE_FMT_FLT :	return "AV_SAMPLE_FMT_FLT"; break;
    case AV_SAMPLE_FMT_DBL :	return "AV_SAMPLE_FMT_DBL"; break;
    case AV_SAMPLE_FMT_U8P :	return "AV_SAMPLE_FMT_U8P"; break;
    case AV_SAMPLE_FMT_S16P :	return "AV_SAMPLE_FMT_S16P"; break;
    case AV_SAMPLE_FMT_S32P :	return "AV_SAMPLE_FMT_S32P"; break;
    case AV_SAMPLE_FMT_FLTP :	return "AV_SAMPLE_FMT_FLTP"; break;
    case AV_SAMPLE_FMT_DBLP :	return "AV_SAMPLE_FMT_DBLP"; break;
    case AV_SAMPLE_FMT_S64 :	return "AV_SAMPLE_FMT_S64"; break;
    case AV_SAMPLE_FMT_S64P :	return "AV_SAMPLE_FMT_S64P"; break;
    case AV_SAMPLE_FMT_NB :		return "AV_SAMPLE_FMT_NB"; break;
    }
    return "";
}

string ff_channel_layout_name(const uint64_t channel_layout, int32_t nb_channels)
{
    std::array<char, 200> chars;
    chars[0] = '\0';
    av_get_channel_layout_string(&chars[0], chars.size(), nb_channels, channel_layout);
    return (&chars[0]);
}

string ff_channel_layout_to_string(const uint64_t channel_layout)
{
    switch (channel_layout) {
    case AV_CH_LAYOUT_MONO: return "AV_CH_LAYOUT_MONO"; break;
    case AV_CH_LAYOUT_STEREO: return "AV_CH_LAYOUT_STEREO"; break;
    case AV_CH_LAYOUT_2POINT1: return "AV_CH_LAYOUT_2POINT1"; break;
    case AV_CH_LAYOUT_2_1: return "AV_CH_LAYOUT_2_1"; break;
    case AV_CH_LAYOUT_SURROUND: return "AV_CH_LAYOUT_SURROUND"; break;
    case AV_CH_LAYOUT_3POINT1: return "AV_CH_LAYOUT_3POINT1"; break;
    case AV_CH_LAYOUT_4POINT0: return "AV_CH_LAYOUT_4POINT0"; break;
    case AV_CH_LAYOUT_4POINT1: return "AV_CH_LAYOUT_4POINT1"; break;
    case AV_CH_LAYOUT_2_2: return "AV_CH_LAYOUT_2_2"; break;
    case AV_CH_LAYOUT_QUAD: return "AV_CH_LAYOUT_QUAD"; break;
    case AV_CH_LAYOUT_5POINT0: return "AV_CH_LAYOUT_5POINT0"; break;
    case AV_CH_LAYOUT_5POINT1: return "AV_CH_LAYOUT_5POINT1"; break;
    case AV_CH_LAYOUT_5POINT0_BACK: return "AV_CH_LAYOUT_5POINT0_BACK"; break;
    case AV_CH_LAYOUT_5POINT1_BACK: return "AV_CH_LAYOUT_5POINT1_BACK"; break;
    case AV_CH_LAYOUT_6POINT0: return "AV_CH_LAYOUT_6POINT0"; break;
    case AV_CH_LAYOUT_6POINT0_FRONT: return "AV_CH_LAYOUT_6POINT0_FRONT"; break;
    case AV_CH_LAYOUT_HEXAGONAL: return "AV_CH_LAYOUT_HEXAGONAL"; break;
    case AV_CH_LAYOUT_6POINT1: return "AV_CH_LAYOUT_6POINT1"; break;
    case AV_CH_LAYOUT_6POINT1_BACK: return "AV_CH_LAYOUT_6POINT1_BACK"; break;
    case AV_CH_LAYOUT_6POINT1_FRONT: return "AV_CH_LAYOUT_6POINT1_FRONT"; break;
    case AV_CH_LAYOUT_7POINT0: return "AV_CH_LAYOUT_7POINT0"; break;
    case AV_CH_LAYOUT_7POINT0_FRONT: return "AV_CH_LAYOUT_7POINT0_FRONT"; break;
    case AV_CH_LAYOUT_7POINT1: return "AV_CH_LAYOUT_7POINT1"; break;
    case AV_CH_LAYOUT_7POINT1_WIDE: return "AV_CH_LAYOUT_7POINT1_WIDE"; break;
    case AV_CH_LAYOUT_7POINT1_WIDE_BACK: return "AV_CH_LAYOUT_7POINT1_WIDE_BACK"; break;
    case AV_CH_LAYOUT_OCTAGONAL: return "AV_CH_LAYOUT_OCTAGONAL"; break;
    case AV_CH_LAYOUT_HEXADECAGONAL: return "AV_CH_LAYOUT_HEXADECAGONAL"; break;
    case AV_CH_LAYOUT_STEREO_DOWNMIX: return "AV_CH_LAYOUT_STEREO_DOWNMIX"; break;

//    case AV_CH_LAYOUT_22POINT2: return "AV_CH_LAYOUT_22POINT2"; break; // Not present on Ubuntu 20.04!
    }
    return "";
}


} //END namespace cpaf::video
