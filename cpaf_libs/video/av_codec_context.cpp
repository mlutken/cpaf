
#include "av_codec_context.h"

extern "C"
{
#include <libavutil/imgutils.h>
}

#include <utility>
#include <ios>
#include <libswscale/swscale.h>

#include <cpaf_libs/video/av_packet.h>
#include <cpaf_libs/video/av_format_context.h>

using namespace std;
using namespace std::chrono;
namespace cpaf::video {

// -------------------------
// --- Constructors etc. ---
// -------------------------

av_codec_context::av_codec_context(AVCodecContext* ff_codec_context,
                                   size_t stream_index,
                                   const AVRational& stream_time_base)
    : ff_codec_context_(ff_codec_context),
      stream_index_(stream_index),
      stream_time_base_(stream_time_base)
{
}

av_codec_context::av_codec_context(AVCodec* ff_codec,
                                   AVCodecParameters* ff_codec_params,
                                   size_t stream_index,
                                   const AVRational& stream_time_base)
    : ff_codec_(ff_codec),
      ff_codec_parameters_(ff_codec_params),
      stream_index_(stream_index),
      stream_time_base_(stream_time_base)
{
    if (ff_codec == nullptr) { // TODO:: || ff_codec_params == nullptr ??
        printf("Unsupported codec!\n");
        return;
    }

    // retrieve codec context
    ff_codec_context_ = avcodec_alloc_context3(ff_codec);
    int ret = avcodec_parameters_to_context(ff_codec_context_, ff_codec_params);
    if (ret != 0) {
        printf("Could not copy parameters to codec context.\n");
        return;
    }

    // initialize the AVCodecContext to use the given AVCodec
    ret = avcodec_open2(ff_codec_context_, ff_codec, nullptr);
    if (ret < 0) {
        printf("Could not open codec.\n");
    }

}

av_codec_context::~av_codec_context()
{
    sws_freeContext(ff_sws_ctx_);
    ff_sws_ctx_ = nullptr;
    if (ff_codec_context_){
        avcodec_free_context(&ff_codec_context_);
        ff_codec_context_ = nullptr;
    }
}

void av_codec_context::swap(av_codec_context& src) noexcept
{
    std::swap(ff_codec_, src.ff_codec_);
    std::swap(ff_codec_context_, src.ff_codec_context_);
    std::swap(ff_codec_parameters_, src.ff_codec_parameters_);
    std::swap(ff_sws_ctx_, src.ff_sws_ctx_);
    std::swap(stream_index_, src.stream_index_);
    std::swap(stream_time_base_, src.stream_time_base_);
    std::swap(ff_dst_pixel_format_, src.ff_dst_pixel_format_);
    std::swap(dst_image_align_, src.dst_image_align_);
    std::swap(dst_width_, src.dst_width_);
    std::swap(dst_height_, src.dst_height_);
    std::swap(get_packet_fun_, src.get_packet_fun_);
    std::swap(media_type_, src.media_type_);
    std::swap(scaling_context_initialized_, src.scaling_context_initialized_);
}

// -------------------------
// --- Scaling functions ---
// -------------------------
bool av_codec_context::init_scaling_context(AVPixelFormat ff_dst_pixel_format,
                                            int32_t dst_width,
                                            int32_t dst_height,
                                            int32_t flags,
                                            int32_t dst_image_align)
{
    return init_scaling_context(ff_dst_pixel_format, surface_dimensions_t{dst_width, dst_height}, flags, dst_image_align);
}

bool av_codec_context::init_scaling_context(AVPixelFormat ff_dst_pixel_format, surface_dimensions_t dst_dimensions, int32_t flags, int32_t dst_image_align)
{
    if (!ff_codec_context_) {
        return false;
    }

    dst_dimensions = scale_surface_dimensions(dimensions(), dst_dimensions);
    dst_width_  = dst_dimensions.x();
    dst_height_ = dst_dimensions.y();

    sws_freeContext(ff_sws_ctx_);
    ff_dst_pixel_format_ = ff_dst_pixel_format;
    dst_image_align_ = dst_image_align;

    ff_sws_ctx_ = sws_getContext(
                ff_codec_context_->width,
                ff_codec_context_->height,
                ff_codec_context_->pix_fmt,
                dst_width_,
                dst_height_,
        ff_dst_pixel_format_,
        flags,
        NULL,
        NULL,
        NULL
    );

    scaling_context_initialized_ = ff_sws_ctx_ != nullptr;
    return scaling_context_initialized_;
}

void av_codec_context::scale_video_frame(av_frame& dst_frame, const av_frame& src_frame) const
{
    sws_scale(
        ff_sws_ctx_,
        (uint8_t const * const *)src_frame.ff_frame()->data,
        src_frame.ff_frame()->linesize,
        0,
        this->height(),
        dst_frame.ff_frame()->data,
        dst_frame.ff_frame()->linesize
    );
    dst_frame.ff_frame()->width = dst_width_;
    dst_frame.ff_frame()->height = dst_height_;
    dst_frame.ff_frame()->best_effort_timestamp = src_frame.ff_frame()->best_effort_timestamp;
    dst_frame.stream_index_set(src_frame.stream_index());
    dst_frame.presentation_time_set(src_frame.presentation_time());
    dst_frame.ff_frame()->pts = src_frame.ff_frame()->pts;
}

av_frame av_codec_context::create_scaling_dst_frame() const
{
    av_frame dst_frame = av_frame::create_alloc();
    dst_frame.init_scaling(dst_image_buffer_size(), ff_dst_pixel_format(),
                           dst_width(), dst_height(), dst_image_align());
    return dst_frame;
}

av_frame av_codec_context::scale_video_frame(const av_frame& src_frame) const
{
    av_frame dst_frame = av_frame::create_alloc();
    dst_frame.init_scaling(dst_image_buffer_size(), ff_dst_pixel_format(),
                           dst_width(), dst_height(), dst_image_align());

    scale_video_frame(dst_frame, src_frame);
    return dst_frame;
}

size_t av_codec_context::dst_image_buffer_size() const
{
    return static_cast<size_t>(av_image_get_buffer_size(
                ff_dst_pixel_format_,
                dst_width_,
                dst_height_,
                dst_image_align_
              ));
}

int av_codec_context::dst_image_buffer_size(AVPixelFormat pixel_format, int32_t dst_image_align) const
{
    return av_image_get_buffer_size(
                pixel_format,
                dst_width_,
                dst_height_,
                dst_image_align
                );
}

void av_codec_context::get_packet_function_set(get_packet_fun&& get_packet)
{
    get_packet_fun_ = std::move(get_packet);
}

/** Read packets until we have a full frame.
    Reading packet is done via the get_packet_fun get_packet_fun_, which you set through
    "get_packet_function_set()". This function will typically be a simple function pulling
    packets from the corresponding queue in the parent av_codec_context. So this could be
    video, audio or subtitle queue that we pull packets from using get_packet_fun_.
*/
av_frame av_codec_context::read_frame() const
{
//    static int FIXMENM_audio__read_frame__counter = 0;
//    if ((this->media_type_get() == media_type::audio)) {
//        FIXMENM_audio__read_frame__counter++;
//    }

    auto packet = get_packet_fun_();
    if ((this->media_type_get() == media_type::audio)) {
        if (!packet.is_valid()) {
            packet = get_packet_fun_();
        }
    }

    av_frame frame = av_frame::create_alloc();
    while (packet.is_valid()) {
        int ret = send_packet(packet);

        if (ret < 0) {
            std::cerr << "ERROR decoding frame\n";
            return av_frame(); // Error sending packet for decoding
        }

        while (ret >= 0) {
            ret = receive_frame(frame);

            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                break;
            }
            else if (ret < 0) {
                std::cerr << "ERROR recieve_frame\n";
                return av_frame();
            }

            // We have a valid frame
            return frame;
        }

        packet = get_packet_fun_();
    }

    if ((this->media_type_get() == media_type::audio)) {
//        std::cerr << "ERROR getting audio frame\n";
    }
    if ((this->media_type_get() == media_type::video)) {
        std::cerr << "ERROR getting video frame\n";
    }
    return av_frame();
}

// -----------------------
// --- Frame functions ---
// -----------------------
int av_codec_context::send_packet(const av_packet& packet) const
{
    if (!ff_codec_context_) {
        return AVERROR(EINVAL);
    }

    return avcodec_send_packet(ff_codec_context_, packet.ff_packet());
}


int av_codec_context::receive_frame(av_frame& frame) const
{
    if (!ff_codec_context_) {
        return AVERROR(EINVAL);
    }

    const int ret_val = avcodec_receive_frame(ff_codec_context_, frame.ff_frame());
    if (ret_val == 0) {
        // Got full frame!
        frame.stream_index_set(stream_index());
        frame.presentation_time_set(time_from_stream_time(frame.ff_frame()->pts));
    }
    return ret_val;
}

std::vector<subtitle_frame> av_codec_context::read_subtitles() const
{
    if (media_type_get() != media_type::subtitle){
        return std::vector<subtitle_frame>();
    }
    if (!ff_codec_context_){
        return std::vector<subtitle_frame>();;
    }


    std::vector<subtitle_frame> subtitles;
    // https://stackoverflow.com/questions/54125207/dump-subtitle-from-avsubtitle-in-the-file
    auto packet = get_packet_fun_();
    if (packet.is_valid()) {
        int got_sub = 0;
        auto ff_subtitle_ptr = std::make_unique<AVSubtitle>();
        AVSubtitle& sub = *ff_subtitle_ptr; // sub.rects[0].type = SUBTITLE_BITMAT, SUBTITLE_TEXT, SUBTITLE_ASS
        avcodec_decode_subtitle2(ff_codec_context_, &sub, &got_sub, packet.ff_packet());
        if (got_sub) {
            subtitles.emplace_back(std::move(ff_subtitle_ptr));
            subtitles.front().presentation_time = packet.presentation_time() + milliseconds(sub.start_display_time);
            subtitles.front().presentation_time_end = packet.presentation_time() + milliseconds(sub.end_display_time);
        }

        // Flush
        // Note: The loop is merely so that we exit for sure even if avcodec_decode_subtitle2 should set got_sub to 1 forever
        AVPacket ff_flush_pkt;
        ff_flush_pkt.data = nullptr;
        ff_flush_pkt.size = 0;
        for (auto i = 100u; 0u < i; --i) {
            int got_sub = 0;
            auto ff_subtitle_ptr = std::make_unique<AVSubtitle>();
            AVSubtitle& sub = *ff_subtitle_ptr; // sub.rects[0].type = SUBTITLE_BITMAT, SUBTITLE_TEXT, SUBTITLE_ASS

            avcodec_decode_subtitle2(ff_codec_context_, &sub, &got_sub, &ff_flush_pkt);
            if (!got_sub) { break; }
            subtitles.emplace_back(std::move(ff_subtitle_ptr));
            subtitles.front().presentation_time = packet.presentation_time() + milliseconds(sub.start_display_time);
            subtitles.front().presentation_time_end = packet.presentation_time() + milliseconds(sub.end_display_time);
        }
    }

    return subtitles;
}

// ----------------------
// --- Time functions ---
// ----------------------

std::chrono::microseconds av_codec_context::time_from_stream_time(int64_t stream_time_duration) const
{
    const int64_t av_time_point = av_rescale_q(stream_time_duration, stream_time_base_, AV_TIME_BASE_Q);

    av_base_duration av_duration(av_time_point);

    return std::chrono::duration_cast<std::chrono::microseconds>(av_duration);
}

//int av_codec_context::receive_frame() const
//{
//	return receive_frame(src_frame_.ff());
//}

// ---------------------
// --- XXX functions ---
// ---------------------


// -----------------------
// --- Debug functions ---
// -----------------------

void av_codec_context::dump() const
{
}

// --------------------------------
// --- Private helper functions ---
// --------------------------------

int av_codec_context::receive_frame(AVFrame* ff_frame) const
{
    if (!ff_codec_context_) {
        return AVERROR(EINVAL);
    }
    return avcodec_receive_frame(ff_codec_context_, ff_frame);
}

int av_codec_context::send_packet(const AVPacket* ff_packet) const
{
    return avcodec_send_packet(ff_codec_context_, ff_packet);
}



} //END namespace cpaf::video
