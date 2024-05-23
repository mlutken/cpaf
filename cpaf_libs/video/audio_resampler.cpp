
#include "audio_resampler.h"

#include <iostream>

extern "C"
{
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
}

#include <cpaf_libs/audio/cpaf_audio_device.h>
#include <cpaf_libs/video/av_codec_context.h>
#include <cpaf_libs/video/audio_types_convert.h>


/// @see libavutil/samplefmt.h (AVSampleFormat)


using namespace std;
namespace cpaf::video {

// ----------------------
// --- Free functions ---
// ----------------------

AVSampleFormat to_av_sample_format(audio::sample_format_t device_audio_format)
{

    // TODO: Detect platform endiannes and act accordingly!
    // This code assumes platform is Little Endian
    switch (device_audio_format) {

    case audio::sample_format_t::u8: return AV_SAMPLE_FMT_U8; break;
    case audio::sample_format_t::s8: return AV_SAMPLE_FMT_NONE; break;
    case audio::sample_format_t::u16: return AV_SAMPLE_FMT_NONE; break;
    case audio::sample_format_t::s16: return AV_SAMPLE_FMT_S16; break;
    case audio::sample_format_t::u16lsb: return AV_SAMPLE_FMT_NONE; break;
    case audio::sample_format_t::s16lsb: return AV_SAMPLE_FMT_S16; break;
    case audio::sample_format_t::u16msb: return AV_SAMPLE_FMT_NONE; break;
    case audio::sample_format_t::s16msb: return AV_SAMPLE_FMT_NONE; break;
    case audio::sample_format_t::s32lsb: return AV_SAMPLE_FMT_S32; break;
    case audio::sample_format_t::s32msb: return AV_SAMPLE_FMT_NONE; break;
    case audio::sample_format_t::f32lsb: return AV_SAMPLE_FMT_FLT; break;
    case audio::sample_format_t::f32msb: return AV_SAMPLE_FMT_NONE; break;
    default:
        return AV_SAMPLE_FMT_NONE;
    }
    return AV_SAMPLE_FMT_NONE;
}

// -----------------------------
// --- Class audio_resampler ---
// -----------------------------

audio_resampler::audio_resampler()
{
}

audio_resampler::~audio_resampler()
{
}

bool audio_resampler::init()
{
    swr_context_.realloc();
    resampled_data_ = NULL;
    swr_context_.set("in_channel_layout",           in_channel_layout_);
    swr_context_.set("in_sample_rate",              in_sample_rate_);
    swr_context_.set_sample_fmt("in_sample_fmt",    in_sample_format_);

    swr_context_.set("out_channel_layout",          out_channel_layout_);
    swr_context_.set("out_sample_rate",             out_sample_rate_);
    swr_context_.set_sample_fmt("out_sample_fmt",   out_sample_format_);

    int ret = swr_context_.init();
    if (ret < 0) {
        printf("Failed to initialize the resampling context.\n");
        return false;
    }
    return true;

}

void audio_resampler::in_formats_set(const av_codec_context& audio_codec_ctx)
{
    if (!audio_codec_ctx.is_valid()) {
        return;
    }
    // Set input channel layout
    const int32_t codec_channels = audio_codec_ctx.channels();
    const uint64_t codec_channel_layout = audio_codec_ctx.channel_layout();
    const uint64_t default_channel_layout = static_cast<uint64_t>(av_get_default_channel_layout(codec_channels));
    const bool channels_equal = audio_codec_ctx.channels() == av_get_channel_layout_nb_channels(codec_channel_layout);

    in_channel_layout_  = channels_equal ? codec_channel_layout : default_channel_layout;
    in_sample_rate_     = audio_codec_ctx.sample_rate();
    in_sample_format_   = audio_codec_ctx.sample_format();
}

void audio_resampler::in_formats_set(const ff_audio_format_t& ff_audio_format)
{
    in_channel_layout_  = ff_audio_format.channel_layout;
    in_sample_rate_     = ff_audio_format.sample_rate;
    in_sample_format_   = ff_audio_format.sample_format;
}

bool audio_resampler::out_formats_set(const audio::device& audio_device)
{
    out_channel_layout_set_from_channels_count(audio_device.channels_count());
    out_sample_rate_ = audio_device.sample_frequency();
    return out_sample_format_set(audio_device.sample_format());
}

bool audio_resampler::out_formats_set(const ff_audio_format_t& ff_audio_format)
{
    out_channel_layout_  = ff_audio_format.channel_layout;
    out_sample_rate_     = ff_audio_format.sample_rate;
    out_sample_format_   = ff_audio_format.sample_format;
    return out_sample_format_ != AV_SAMPLE_FMT_NONE;
}

bool audio_resampler::out_sample_format_set(audio::sample_format_t format)
{
    out_sample_format_ = to_av_sample_format(format);
    return out_sample_format_ != AV_SAMPLE_FMT_NONE;
}

/** @todo This could really be factored/delegated into a more general covert function */
int32_t audio_resampler::out_channel_count() const
{
    if (out_channel_count_ != 0) return out_channel_count_;
    return ff_channel_layout_to_count(out_channel_layout_);
}

void audio_resampler::out_channel_layout_set(uint64_t layout) {
    out_channel_layout_ = layout;
////   out_nb_channels_ = av_get_channel_layout_nb_channels(out_channel_layout_);
}

/** @todo This could really be factored/delegated into a more general covert function */
void audio_resampler::out_channel_layout_set_from_channels_count(int32_t channels_count)
{
    out_channel_count_ = channels_count;
    if (channels_count == 1) {
        out_channel_layout_ = AV_CH_LAYOUT_MONO;
    }
    else if (channels_count == 2){
        out_channel_layout_ = AV_CH_LAYOUT_STEREO;
    }
    else {
        out_channel_layout_ = AV_CH_LAYOUT_5POINT1;
    }
}

int audio_resampler::audio_resampling(const av_frame& decoded_audio_frame, uint8_t* out_buf)
{

    // retrieve number of audio samples (per channel)
    const auto in_nb_samples = decoded_audio_frame.ff_frame()->nb_samples;
    if (in_nb_samples <= 0)	{
        printf("in_nb_samples error.\n");
        return -1;
    }


    auto out_nb_samples = av_rescale_rnd(
                in_nb_samples,
                out_sample_rate_,
                in_sample_rate_,
                AV_ROUND_UP
                );

    auto max_out_nb_samples = out_nb_samples;
    // check rescaling was successful
    if (max_out_nb_samples <= 0) {
        printf("av_rescale_rnd error.\n");
        return -1;
    }

    // get number of output audio channels
    const auto out_nb_channels = av_get_channel_layout_nb_channels(out_channel_layout_);

    int out_linesize = 0;

    // allocate data pointers array for resampled_data and fill data
    // pointers and linesize accordingly
    const auto alloc_ret_val = av_samples_alloc_array_and_samples(
                &resampled_data_,
                &out_linesize,
                out_nb_channels,
                out_nb_samples,
                out_sample_format_,
                0
                );

    // check memory allocation for the resampled data was successful
    if (alloc_ret_val < 0) {
        printf("av_samples_alloc_array_and_samples() error: Could not allocate destination samples.\n");
        return -1;
    }

    // retrieve output samples number taking into account the progressive delay
    const auto samples_delay = swr_context_.delay(in_sample_rate_);
    out_nb_samples = av_rescale_rnd(
                samples_delay + in_nb_samples,
                out_sample_rate_,
                in_sample_rate_,
                AV_ROUND_UP
                );

    // check output samples number was correctly rescaled
    if (out_nb_samples <= 0) {
        printf("av_rescale_rnd error\n");
        return -1;
    }

    if (out_nb_samples > max_out_nb_samples) {
        // free memory block and set pointer to NULL
        av_freep(resampled_data_[0]);

        // Allocate a samples buffer for out_nb_samples samples
        const auto alloc_ret_val = av_samples_alloc(
                    resampled_data_,
                    &out_linesize,
                    out_nb_channels,
                    out_nb_samples,
                    out_sample_format_,
                    1
                    );

        // check samples buffer correctly allocated
        if (alloc_ret_val < 0) {
            printf("av_samples_alloc failed.\n");
            return -1;
        }
    }

    // do the actual audio data resampling
    const auto samples_per_channel = swr_context_.convert(
                resampled_data_,
                out_nb_samples,
                (const uint8_t **) decoded_audio_frame.ff_frame()->data,
                decoded_audio_frame.nb_samples()
                );

    // check audio conversion was successful
    if (samples_per_channel < 0) {
        printf("swr_convert_error.\n");
        return -1;
    }

    // get the required buffer size for the given audio parameters
    const auto resampled_data_size = av_samples_get_buffer_size(
                &out_linesize,
                out_nb_channels,
                samples_per_channel,
                out_sample_format_,
                1
                );

    // check audio buffer size
    if (resampled_data_size < 0) {
        printf("av_samples_get_buffer_size error.\n");
        return -1;
    }

    // copy the resampled data to the output buffer
    memcpy(out_buf, resampled_data_[0], static_cast<size_t>(resampled_data_size));

    /*
     * Memory Cleanup.
     */
    if (resampled_data_)	{
        // free memory block and set pointer to NULL
        av_freep(&resampled_data_[0]);
    }

    av_freep(&resampled_data_);
    resampled_data_ = nullptr;

    return resampled_data_size;

}

av_samples_buffer audio_resampler::audio_resampling(const av_frame& decoded_audio_frame)
{
    av_samples_buffer buf;
    // retrieve number of audio samples (per channel)
    const auto in_nb_samples = decoded_audio_frame.ff_frame()->nb_samples;
    if (in_nb_samples <= 0)	{
        std::cerr << "RESAMPLE ERROR in_nb_samples error.\n";
        return av_samples_buffer();
    }


    auto out_nb_samples = av_rescale_rnd(
                in_nb_samples,
                out_sample_rate_,
                in_sample_rate_,
                AV_ROUND_UP
                );

    auto max_out_nb_samples = out_nb_samples;
    // check rescaling was successful
    if (max_out_nb_samples <= 0) {
        std::cerr << "RESAMPLE ERROR av_rescale_rnd error.\n";
        return av_samples_buffer();
    }

    // get number of output audio channels
    const auto out_nb_channels = av_get_channel_layout_nb_channels(out_channel_layout_);

    int out_linesize = 0;

    // allocate data pointers array for resampled_data and fill data
    // pointers and linesize accordingly
    const auto alloc_ret_val = buf.samples_alloc_array_and_samples(
                &out_linesize,
                out_nb_channels,
                out_nb_samples,
                out_sample_format_,
                0
                );

    // check memory allocation for the resampled data was successful
    if (alloc_ret_val < 0) {
        std::cerr << "RESAMPLE ERROR av_samples_alloc_array_and_samples() error: Could not allocate destination samples.\n";
        return av_samples_buffer();
    }

    // retrieve output samples number taking into account the progressive delay
    const auto samples_delay = swr_context_.delay(in_sample_rate_);
    out_nb_samples = av_rescale_rnd(
                samples_delay + in_nb_samples,
                out_sample_rate_,
                in_sample_rate_,
                AV_ROUND_UP
                );

    // check output samples number was correctly rescaled
    if (out_nb_samples <= 0) {
        std::cerr << "RESAMPLE ERROR av_rescale_rnd error\n";
        return av_samples_buffer();
    }

    if (out_nb_samples > max_out_nb_samples) {
        // Allocate a samples buffer for out_nb_samples samples
        const auto alloc_ret_val = buf.samples_alloc(
                    &out_linesize,
                    out_nb_channels,
                    out_nb_samples,
                    out_sample_format_,
                    1
                    );

        // check samples buffer correctly allocated
        if (alloc_ret_val < 0) {
            std::cerr << "RESAMPLE ERROR av_samples_alloc failed.\n";
            return av_samples_buffer();
        }
    }

    // do the actual audio data resampling
    const auto samples_per_channel = swr_context_.convert(
                buf.raw_buffer(),
                out_nb_samples,
                (const uint8_t **) decoded_audio_frame.ff_frame()->data,
                decoded_audio_frame.nb_samples()
                );

    // check audio conversion was successful
    if (samples_per_channel < 0) {
        std::cerr << "RESAMPLE ERROR swr_convert_error.\n";
        return av_samples_buffer();
    }

    // get the required buffer size for the given audio parameters
    const auto resampled_data_size = av_samples_get_buffer_size(
                &out_linesize,
                out_nb_channels,
                samples_per_channel,
                out_sample_format_,
                1
                );

    // check audio buffer size
    if (resampled_data_size < 0) {
        std::cerr << "RESAMPLE ERROR av_samples_get_buffer_size error.\n";
        return av_samples_buffer();
    }

    // Copy fields from original frame to samples buffer ("frame")
    buf.initial_size_set(resampled_data_size);
    buf.presentation_time_set(decoded_audio_frame.presentation_time());
    buf.bytes_per_microsecond_set(out_bytes_per_microsecond());
    buf.bytes_per_sample_set(out_bytes_per_sample());
    return buf;
}

int32_t audio_resampler::out_bytes_per_sample_channel() const
{
    return sample_format_byte_count(out_sample_format_);
}

float audio_resampler::out_bytes_per_microsecond() const
{
    const auto bytes_per_second = out_sample_rate() * out_channel_count() * out_bytes_per_sample_channel() ;
    return bytes_per_second / 1'000'000.0f;
}

string audio_resampler::dbg_characteristics() const
{
    std::stringstream ss;
    ss  << "--- Resampler characteristics ---\n"
        << "  Sample rate    in => out : " << in_sample_rate() << " => " << out_sample_rate() << "\n"
        << "  Sample format  in => out : " << to_string(in_sample_format()) << " => " << to_string(out_sample_format()) << "\n"
        << "  Channel layout in => out : " << ff_channel_layout_to_string(in_channel_layout()) << " => " << ff_channel_layout_to_string(out_channel_layout()) << "\n"
        ;
    return ss.str();
}


} //END namespace cpaf::video
