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


/** Note this is not running it's own thread, but supplies a callback
    for a cpaf::audio_device */
class audio_render_thread
{
public:
    using play_callback_t  = cpaf::audio::device_base::play_callback_t;
    audio_render_thread() = default;

    void                    format_context_set      (av_format_context* ctx)    { format_context_ptr_ = ctx; }
    void                    format_context_set      (av_format_context& ctx)    { format_context_ptr_ = &ctx; }
    void                    audio_samples_queue_set (av_samples_queue_t* queue) { audio_samples_queue_ptr_ = queue; }
    void                    audio_samples_queue_set (av_samples_queue_t& queue) { audio_samples_queue_ptr_ = &queue; }
    void                    current_media_time_set  (media_stream_time* mts)    { current_media_time_ptr_ = mts; }
    void                    current_media_time_set  (media_stream_time& mts)    { current_media_time_ptr_ = &mts; }

    void                    start                   ();
    bool                    flush_to_index          (const pipeline_index_t& pipeline_index);
    void                    audio_queue_flush_start ()                          { audio_queue_flush_in_progress_ = true; }
    void                    audio_queue_flush_done  ()                          { audio_queue_flush_in_progress_ = false; audio_queue_flushed_ = true; }
    bool                    state_matches           (pipeline_state_t desired_state, const pipeline_index_t& desired_index) const;


    play_callback_t         audio_callback_get      ();

    const atomic_pipeline_index_t&      current_pipeline_index  () const { return current_pipeline_index_; }

private:
    using flush_queue_t = estl::srsw_fifo_s<pipeline_index_t, 32>;

    void                    audio_callback_function (uint8_t* stream, int32_t length);
    pipeline_index_t        get_flush_to_index      ();
    void                    switch_state            ();
    void                    state__normal_flow      (uint8_t* stream, int32_t length);
    void                    state__flush_in_progress(uint8_t* stream, int32_t length);
    void                    render_audio_silence    (uint8_t* stream, int32_t length);
    void                    debug_audio_callback    (uint8_t* stream, int32_t length);

    av_format_context&      format_context          () { return *format_context_ptr_; }
    av_samples_queue_t&     audio_samples_queue     () { return *audio_samples_queue_ptr_; }
    media_stream_time&      current_media_time      () { return *current_media_time_ptr_; }

    flush_queue_t                   flush_queue_;
    av_format_context*              format_context_ptr_             = nullptr;
    av_codec_context*               audio_codec_ctx_ptr_            = nullptr;
    av_samples_queue_t*             audio_samples_queue_ptr_        = nullptr;
    media_stream_time*              current_media_time_ptr_         = nullptr;

    std::chrono::microseconds       sync_ok_interval                = std::chrono::milliseconds(15);
    pipeline_index_t                prev_pipeline_index_            = std::numeric_limits<pipeline_index_t>::max();
    pipeline_index_t                flush_in_progress_index_        = 0;
    pipeline_state_t                pipeline_state_                 = pipeline_state_t::normal_flow;
    atomic_pipeline_index_t         current_pipeline_index_         = 0;

    int                             audio_callback_dbg_counter_     = 0;
    std::atomic_bool                audio_queue_flush_in_progress_  = false;
    std::atomic_bool                audio_queue_flushed_            = false;
    bool                            empty_audio_buffer_dbg_         = false;

};

} // namespace cpaf::video

