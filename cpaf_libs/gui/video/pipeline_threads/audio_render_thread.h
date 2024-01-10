#pragma once

#include <atomic>
#include <concurrent/srsw_fifo_s.hpp>
#include <cpaf_libs/audio/cpaf_audio_device_base.h>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/av_samples_buffer.h>

namespace cpaf::video {
class av_format_context;
class av_codec_context;
class media_stream_time;
class av_samples_queue;
};

namespace cpaf::gui::video {

class pipeline_threads;

/** Note this is not running it's own thread, but supplies a callback
    for a cpaf::audio_device */
class audio_render_thread
{
public:
    using audio_play_callback_t  = cpaf::audio::device_base::play_callback_t;
    explicit audio_render_thread(pipeline_threads& pline_threads, std::atomic<cpaf::video::seek_state_t>& seek_state);

    void                    format_context_set      (cpaf::video::av_format_context* ctx)    { format_context_ptr_ = ctx; }
    void                    format_context_set      (cpaf::video::av_format_context& ctx)    { format_context_ptr_ = &ctx; }
    void                    audio_samples_queue_set (cpaf::video::av_samples_queue* queue)   { audio_samples_queue_ptr_ = queue; }
    void                    audio_samples_queue_set (cpaf::video::av_samples_queue& queue)   { audio_samples_queue_ptr_ = &queue; }
    void                    current_media_time_set  (cpaf::video::media_stream_time* mts)    { current_media_time_ptr_ = mts; }
    void                    current_media_time_set  (cpaf::video::media_stream_time& mts)    { current_media_time_ptr_ = &mts; }

    void                    start                   ();

    audio_play_callback_t   audio_callback_get      ();

private:
    void                    audio_callback_function (uint8_t* stream, int32_t length);
    void                    render_audio_silence    (uint8_t* stream, int32_t length);
    void                    debug_audio_callback    (uint8_t* stream, int32_t length);

    cpaf::video::av_format_context&      format_context          () { return *format_context_ptr_; }
    cpaf::video::av_samples_queue&       audio_samples_queue     () { return *audio_samples_queue_ptr_; }
    cpaf::video::media_stream_time&      current_media_time      () { return *current_media_time_ptr_; }

    pipeline_threads&                           pipeline_threads_;
    std::atomic<cpaf::video::seek_state_t>&     seek_state_;
    cpaf::video::av_format_context*             format_context_ptr_             = nullptr;
    cpaf::video::av_codec_context*              audio_codec_ctx_ptr_            = nullptr;
    cpaf::video::av_samples_queue*              audio_samples_queue_ptr_        = nullptr;
    cpaf::video::media_stream_time*             current_media_time_ptr_         = nullptr;
    std::chrono::microseconds                   sync_ok_interval                = std::chrono::milliseconds(15);
    int                                         audio_callback_dbg_counter_     = 0;
};

} // namespace cpaf::gui::video

