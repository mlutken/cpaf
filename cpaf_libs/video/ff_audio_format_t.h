#pragma once

#include <cstdint>

extern "C"
{
#include <libswresample/swresample.h>
}

#include <cpaf_libs/audio/cpaf_audio_types.h>

namespace cpaf {
namespace video {

struct ff_audio_format_t
{
    uint64_t				channel_layout		= AV_CH_LAYOUT_STEREO;
    AVSampleFormat          sample_format       = AV_SAMPLE_FMT_S16;
    int32_t                 sample_rate         = 44100;
};

audio::channel_layout_t to_audio_channel_layout     (uint64_t ff_channel_layout);
audio::sample_format_t  to_audio_sample_format      (AVSampleFormat ff_sample_format);
audio::audio_format_t   to_audio_format             (const ff_audio_format_t& ff_audio_format);

uint64_t                to_ff_audio_channel_layout  (audio::channel_layout_t channel_layout);
AVSampleFormat          to_ff_audio_sample_format   (audio::sample_format_t sample_format);
ff_audio_format_t       to_ff_audio_format          (const audio::audio_format_t& audio_format);

} // namespace video
} // namespace cpaf

