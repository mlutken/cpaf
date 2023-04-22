#include "ff_audio_format_t.h"
#include <cpaf_libs/system/cpaf_system_info.h>

namespace cpaf {
namespace video {


audio::channel_layout_t to_audio_channel_layout(uint64_t ff_channel_layout)
{
    audio::channel_layout_t dst_layout;
    if      (ff_channel_layout == AV_CH_LAYOUT_MONO)        return audio::channel_layout_t::mono;
    else if (ff_channel_layout == AV_CH_LAYOUT_STEREO)      return audio::channel_layout_t::stereo;
    else if (ff_channel_layout == AV_CH_LAYOUT_SURROUND)    return audio::channel_layout_t::surround2_1;
    else if (ff_channel_layout == AV_CH_LAYOUT_QUAD)        return audio::channel_layout_t::quad;
    else if (ff_channel_layout == AV_CH_LAYOUT_4POINT0)     return audio::channel_layout_t::quad_center;    // TODO: Is this the same?
    else if (ff_channel_layout == AV_CH_LAYOUT_5POINT1)     return audio::channel_layout_t::surround5_1;    // TODO: Is this the same?
    else if (ff_channel_layout == AV_CH_LAYOUT_6POINT1)     return audio::channel_layout_t::surround6_1;    // TODO: Is this the same?
    else if (ff_channel_layout == AV_CH_LAYOUT_7POINT1)     return audio::channel_layout_t::surround7_1;    // TODO: Is this the same?
    return audio::channel_layout_t::INVALID;
    return dst_layout;
}

audio::sample_format_t to_audio_sample_format(AVSampleFormat ff_sample_format)
{
    switch (ff_sample_format) {
    case AV_SAMPLE_FMT_NONE:    return audio::sample_format_t::INVALID; break;
    case AV_SAMPLE_FMT_U8:      return audio::sample_format_t::u8; break;
    case AV_SAMPLE_FMT_S16:     return audio::sample_format_t::s16; break;
    case AV_SAMPLE_FMT_S32:
    {
        if constexpr (system::is_little_endian())   return audio::sample_format_t::s32lsb;
        else                                        return audio::sample_format_t::s32msb;
        break;
    }
    case AV_SAMPLE_FMT_FLT:
        if constexpr (system::is_little_endian())   return audio::sample_format_t::f32lsb;
        else                                        return audio::sample_format_t::f32msb;
        break;
    case AV_SAMPLE_FMT_DBL:     return audio::sample_format_t::INVALID; break;

    case AV_SAMPLE_FMT_U8P:     return audio::sample_format_t::INVALID; break;
    case AV_SAMPLE_FMT_S16P:    return audio::sample_format_t::INVALID; break;
    case AV_SAMPLE_FMT_S32P:    return audio::sample_format_t::INVALID; break;
    case AV_SAMPLE_FMT_FLTP:    return audio::sample_format_t::INVALID; break;
    case AV_SAMPLE_FMT_DBLP:    return audio::sample_format_t::INVALID; break;
    case AV_SAMPLE_FMT_S64:     return audio::sample_format_t::INVALID; break;
    case AV_SAMPLE_FMT_S64P:    return audio::sample_format_t::INVALID; break;
    case AV_SAMPLE_FMT_NB:      return audio::sample_format_t::INVALID; break;
    }

    return audio::sample_format_t::INVALID;
}

audio::audio_format_t to_audio_format(const ff_audio_format_t& ff_audio_format)
{
    audio::audio_format_t dst_fmt;
    dst_fmt.channel_layout = to_audio_channel_layout(ff_audio_format.channel_layout);
    dst_fmt.sample_format = to_audio_sample_format(ff_audio_format.sample_format);
    dst_fmt.sample_rate = ff_audio_format.sample_rate;
    return dst_fmt;
}




uint64_t to_ff_audio_channel_layout(audio::channel_layout_t channel_layout)
{
    switch (channel_layout) {
    case audio::channel_layout_t::mono:         return AV_CH_LAYOUT_MONO; break;
    case audio::channel_layout_t::stereo:		return AV_CH_LAYOUT_STEREO; break;
    case audio::channel_layout_t::surround2_1:	return AV_CH_LAYOUT_SURROUND; break;
    case audio::channel_layout_t::quad:         return AV_CH_LAYOUT_QUAD; break;
    case audio::channel_layout_t::quad_center:	return AV_CH_LAYOUT_4POINT0; break;
    case audio::channel_layout_t::surround5_1:	return AV_CH_LAYOUT_5POINT1; break;
    case audio::channel_layout_t::surround6_1:	return AV_CH_LAYOUT_6POINT1; break;
    case audio::channel_layout_t::surround7_1:	return AV_CH_LAYOUT_7POINT1; break;

    case audio::channel_layout_t::SIZE:
    case audio::channel_layout_t::INVALID:      return 0; break;
    }
    return 0;
}


AVSampleFormat to_ff_audio_sample_format(audio::sample_format_t sample_format)
{
    switch (sample_format) {
    case audio::sample_format_t::u8:        return AV_SAMPLE_FMT_U8; break;
    case audio::sample_format_t::s8:        return AV_SAMPLE_FMT_NONE; break;
    case audio::sample_format_t::u16:       return AV_SAMPLE_FMT_NONE; break;
    case audio::sample_format_t::s16:       return AV_SAMPLE_FMT_S16; break;
    case audio::sample_format_t::u16lsb:    return AV_SAMPLE_FMT_NONE; break;
    case audio::sample_format_t::s16lsb:
    {
        if constexpr (system::is_little_endian())   return AV_SAMPLE_FMT_S16;
        else                                        return AV_SAMPLE_FMT_NONE;
        break;
    }
    case audio::sample_format_t::u16msb:    return AV_SAMPLE_FMT_NONE; break;
    case audio::sample_format_t::s16msb:
    {
        if constexpr (system::is_little_endian())   return AV_SAMPLE_FMT_NONE;
        else                                        return AV_SAMPLE_FMT_S16;
        break;
    }
    case audio::sample_format_t::s32lsb:
    {
        if constexpr (system::is_little_endian())   return AV_SAMPLE_FMT_S32;
        else                                        return AV_SAMPLE_FMT_NONE;
        break;
    }
    case audio::sample_format_t::s32msb:
    {
        if constexpr (system::is_little_endian())   return AV_SAMPLE_FMT_NONE;
        else                                        return AV_SAMPLE_FMT_S32;
        break;
    }
    case audio::sample_format_t::f32lsb:
    {
        if constexpr (system::is_little_endian())   return AV_SAMPLE_FMT_FLT;
        else                                        return AV_SAMPLE_FMT_NONE;
        break;
    }
    case audio::sample_format_t::f32msb:
    {
        if constexpr (system::is_little_endian())   return AV_SAMPLE_FMT_NONE;
        else                                        return AV_SAMPLE_FMT_FLT;
        break;
    }
    case audio::sample_format_t::SIZE:
    case audio::sample_format_t::INVALID:   return AV_SAMPLE_FMT_NONE; break;
    }

    return AV_SAMPLE_FMT_NONE;
}

ff_audio_format_t to_ff_audio_format(const audio::audio_format_t& audio_format)
{
    ff_audio_format_t dst_fmt;
    dst_fmt.channel_layout = to_ff_audio_channel_layout(audio_format.channel_layout);
    dst_fmt.sample_format = to_ff_audio_sample_format(audio_format.sample_format);
    dst_fmt.sample_rate = audio_format.sample_rate;
    return dst_fmt;
}

} // namespace video
} // namespace cpaf
