#pragma once

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include <string>
#include <functional>
#include <cpaf_libs/video/av_frame.h>
#include <cpaf_libs/video/subtitle_frame.h>
#include <cpaf_libs/video/av_util.h>

namespace cpaf::video {

class av_packet;
class av_format_context;

/**
 *
*  */
class av_codec_context
{
public:
    using get_packet_fun = std::function<av_packet()>;

    // --- Constructors etc. ---
    av_codec_context (const av_codec_context&) = delete;
    av_codec_context (av_codec_context&& moving) noexcept
    {
        moving.swap(*this);
    }

    av_codec_context& operator=(const av_codec_context&)  = delete;
    av_codec_context& operator=(av_codec_context&& moving) noexcept
    {
        moving.swap(*this);
        return *this;
    }

    av_codec_context() = default;

    av_codec_context(AVCodecContext* ff_codec_context,
                     size_t stream_index,
                     const AVRational& stream_time_base);
    av_codec_context(AVCodec* ff_codec,
                     AVCodecParameters* ff_codec_params,
                     size_t stream_index,
                     const AVRational& stream_time_base);
    ~av_codec_context();

    size_t                  stream_index                () const { return stream_index_; }

    void                    swap						(av_codec_context& src) noexcept;

    media_type_t            media_type_get              () const { return media_type_; }
    void                    media_type_set              (media_type_t mt) { media_type_ = mt; }

    // --- Video info functions ---
    AVCodecContext*         ff							() const { return ff_codec_context_; }
    int32_t                 width						() const { return ff_codec_context_ ? ff_codec_context_->width : 0; }
    int32_t                 height						() const { return ff_codec_context_ ? ff_codec_context_->height : 0; }
    surface_dimensions_t    dimensions                  () const { return {width(), height()}; }
    cpaf::math::v2f         dimensions_float            () const { return {static_cast<float>(width()), static_cast<float>(height())}; }
    int32_t                 dst_width					() const { return dst_width_ <= 0 ? width() : dst_width_; }
    int32_t                 dst_height					() const { return dst_height_ <= 0 ? height(): dst_height_; }
    surface_dimensions_t    dst_dimensions              () const { return {dst_width(), dst_height()}; }
    int32_t                 dst_image_align				() const { return dst_image_align_; }
    AVPixelFormat           ff_dst_pixel_format			() const { return ff_dst_pixel_format_; }
    bool                    scaling_context_initialized () const {return scaling_context_initialized_; }

    // -- Audio info functions ---
    AVSampleFormat          ff_audio_sample_format		() const { return ff_codec_context_->sample_fmt; }
    AVSampleFormat          sample_format               () const { return ff_codec_context_->sample_fmt; }
    int32_t                 sample_rate                 () const { return ff_codec_context_->sample_rate; }
    int32_t                 audio_sample_rate			() const { return ff_codec_context_->sample_rate; }
    int32_t                 audio_channels				() const { return ff_codec_context_->channels; } // ch_layout.nb_channels
    int32_t                 channels                    () const { return ff_codec_context_->channels; } // ch_layout.nb_channels
    uint64_t                channel_layout				() const { return ff_codec_context_->channel_layout; }  // TODO: Use AVChannelLayout instead


    bool                    is_valid	() const { return ff_codec_context_ != nullptr; }

    // --- Scaling functions ---
    bool                    init_scaling_context        (AVPixelFormat ff_dst_pixel_format,
                                                         int32_t dst_width = surface_dimension_auto,
                                                         int32_t dst_height = surface_dimension_auto,
                                                         int32_t flags = SWS_BILINEAR,
                                                         int32_t dst_image_align = 32);
    bool                    init_scaling_context        (AVPixelFormat ff_dst_pixel_format,
                                                         surface_dimensions_t dst_dimensions = {surface_dimension_auto,surface_dimension_auto},
                                                         int32_t flags = SWS_BILINEAR,
                                                         int32_t dst_image_align = 32);

    void                    scale_video_frame           (av_frame& dst_frame, const av_frame& src_frame) const;
    av_frame                create_scaling_dst_frame    () const;
    av_frame                scale_video_frame           (const av_frame& src_frame) const;
    size_t                  dst_image_buffer_size       () const;
    int                     dst_image_buffer_size       (AVPixelFormat pixel_format, int32_t dst_image_align = 32) const;

    // --- Frame functions ---
    void                    get_packet_function_set     (get_packet_fun&& get_packet);
    av_frame                read_frame                  () const;
    int                     send_packet                 (const av_packet& packet) const;
    int                     receive_frame               (av_frame& frame) const;

    // --- Subtitle functions ---
    std::vector<subtitle_frame> read_subtitles          () const;

    // --- Time functions ---
    std::chrono::microseconds   time_from_stream_time       (int64_t stream_time_duration) const;
    std::chrono::microseconds   time_from_stream_time       (const av_frame& frame) const;
    void                        set_frame_presentation_time (av_frame& frame) const;

    // --- Debug functions ---
    void                    dump                        () const;

private:
    // --- Private helper functions ---
    int                     receive_frame               (AVFrame* ff_frame) const;
    int                     send_packet                 (const AVPacket* ff_packet) const;

    AVCodec*                    ff_codec_                   = nullptr;
    AVCodecContext*             ff_codec_context_           = nullptr;
    AVCodecParameters*          ff_codec_parameters_        = nullptr;
    struct SwsContext*          ff_sws_ctx_                 = nullptr; // For scaling video
    size_t                      stream_index_               = illegal_stream_index();
    AVRational                  stream_time_base_;
    ///	uint8_t*                    dst_frame_image_buf_        = nullptr;
    AVPixelFormat               ff_dst_pixel_format_        = AV_PIX_FMT_NONE;
    int32_t                     dst_image_align_            = 32;
    int32_t                     dst_width_                  = -1;
    int32_t                     dst_height_                 = -1;
    get_packet_fun              get_packet_fun_;
    media_type_t                  media_type_                 = media_type_t::unknown;
    bool                        scaling_context_initialized_= false;
};


} //END namespace cpaf::video

