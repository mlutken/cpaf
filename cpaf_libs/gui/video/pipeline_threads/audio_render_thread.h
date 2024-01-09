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
//    bool                    flush_to_index          (const cpaf::video::pipeline_index_t& pipeline_index);
//    void                    audio_queue_flush_start ()                          { audio_queue_flush_in_progress_ = true; }
//    void                    audio_queue_flush_done  ()                          { audio_queue_flush_in_progress_ = false; audio_queue_flushed_ = true; }
///    bool                    state_matches           (cpaf::video::pipeline_state_t desired_state, const cpaf::video::pipeline_index_t& desired_index) const;


    audio_play_callback_t   audio_callback_get      ();

///    const cpaf::video::atomic_pipeline_index_t&      current_pipeline_index  () const { return current_pipeline_index_; }

private:
    using flush_queue_t = estl::srsw_fifo_s<cpaf::video::pipeline_index_t, 32>;

    void                    audio_callback_function (uint8_t* stream, int32_t length);
///    cpaf::video::pipeline_index_t        get_flush_to_index      ();
///    void                    switch_state            ();
///    void                    state__normal_flow      (uint8_t* stream, int32_t length);
//    void                    state__flush_in_progress(uint8_t* stream, int32_t length);
    void                    render_audio_silence    (uint8_t* stream, int32_t length);
    void                    debug_audio_callback    (uint8_t* stream, int32_t length);

    cpaf::video::av_format_context&      format_context          () { return *format_context_ptr_; }
    cpaf::video::av_samples_queue&       audio_samples_queue     () { return *audio_samples_queue_ptr_; }
    cpaf::video::media_stream_time&      current_media_time      () { return *current_media_time_ptr_; }

    pipeline_threads&                           pipeline_threads_;
    std::atomic<cpaf::video::seek_state_t>&     seek_state_;
    flush_queue_t                               flush_queue_;
    cpaf::video::av_format_context*             format_context_ptr_             = nullptr;
    cpaf::video::av_codec_context*              audio_codec_ctx_ptr_            = nullptr;
    cpaf::video::av_samples_queue*              audio_samples_queue_ptr_        = nullptr;
    cpaf::video::media_stream_time*             current_media_time_ptr_         = nullptr;

    std::chrono::microseconds       sync_ok_interval                = std::chrono::milliseconds(15);
//    cpaf::video::pipeline_index_t                prev_pipeline_index_            = std::numeric_limits<cpaf::video::pipeline_index_t>::max();
//    cpaf::video::pipeline_index_t                flush_in_progress_index_        = 0;
//    cpaf::video::pipeline_state_t                pipeline_state_                 = cpaf::video::pipeline_state_t::normal_flow;
//    cpaf::video::atomic_pipeline_index_t         current_pipeline_index_         = 0;

    int                             audio_callback_dbg_counter_     = 0;
///    std::atomic_bool                audio_queue_flush_in_progress_  = false;
///    std::atomic_bool                audio_queue_flushed_            = false;

};

} // namespace cpaf::gui::video

