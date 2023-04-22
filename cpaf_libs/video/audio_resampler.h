#pragma once

extern "C"
{
#include <libswresample/swresample.h>
}

#include <string>
#include <cpaf_libs/audio/cpaf_audio_types.h>
#include <cpaf_libs/video/av_frame.h>
#include <cpaf_libs/video/av_options.h>
#include <cpaf_libs/video/av_samples_buffer.h>

namespace cpaf::audio {
    class device;
}

namespace cpaf::video {
AVSampleFormat   to_av_sample_format    (cpaf::audio::format_t device_audio_format);

class av_codec_context;

class audio_resampler {
public:

    audio_resampler();
    ~audio_resampler();

    bool                    	init                        ();

    void                    	in_formats_set              (const av_codec_context& audio_codec_ctx);
    bool                    	out_formats_set             (const cpaf::audio::device& audio_device);

    int32_t                 	in_sample_rate              () const                { return in_sample_rate_;       }
    void                    	in_sample_rate_set          (int32_t frequency)     { in_sample_rate_ = frequency;  }
    int32_t                 	out_sample_rate             () const                { return out_sample_rate_;      }
    void                    	out_sample_rate_set         (int32_t frequency)     { out_sample_rate_ = frequency; }

    AVSampleFormat          	in_sample_format            () const                { return in_sample_format_;     }
    void                    	in_sample_format_set        (AVSampleFormat format) { in_sample_format_ = format;   }
    AVSampleFormat          	out_sample_format           () const                { return out_sample_format_;    }
    void                    	out_sample_format_set       (AVSampleFormat format) { out_sample_format_ = format;  }
    bool                    	out_sample_format_set       (cpaf::audio::format_t format);

    uint64_t                	in_channel_layout           () const                { return in_channel_layout_;    }
    void                        in_channel_layout_set       (uint64_t layout)       { in_channel_layout_ = layout;  }
    int32_t                     out_channel_count           () const;
    uint64_t                    out_channel_layout          () const                { return out_channel_layout_;   }
    void                        out_channel_layout_set      (uint64_t layout);
    void                        out_channel_layout_set_from_channels_count
                                                            (int32_t channels_count);

    int                         audio_resampling            (const av_frame& decoded_audio_frame, uint8_t* out_buf);
    av_samples_buffer           audio_resampling            (const av_frame& decoded_audio_frame);
    std::chrono::microseconds   time_between_out_samples    () const { return std::chrono::microseconds(1'000'000/out_sample_rate()); }
    int32_t                     out_bytes_per_sample_channel() const;
    int32_t                     out_bytes_per_sample        () const { return out_bytes_per_sample_channel() * out_channel_count(); }
    float                       out_bytes_per_microsecond   () const;

    std::string             	dbg_characteristics() const;
private:

    av_swr_context			swr_context_;
    uint64_t				in_channel_layout_		= AV_CH_LAYOUT_STEREO;
    uint64_t				out_channel_layout_		= AV_CH_LAYOUT_STEREO;
    int32_t                 in_sample_rate_         = 44100;
    int32_t                 out_sample_rate_        = 44100;
    AVSampleFormat          in_sample_format_       = AV_SAMPLE_FMT_S16;
    AVSampleFormat          out_sample_format_      = AV_SAMPLE_FMT_S16;
    int32_t                 out_channel_count_      = 0;

    uint8_t**				resampled_data_			= nullptr;

};

} //END namespace cpaf::video
