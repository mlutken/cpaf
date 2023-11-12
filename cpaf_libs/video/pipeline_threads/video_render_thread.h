#pragma once

#include <chrono>
#include <atomic>
#include <concurrent/srsw_fifo_s.hpp>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/av_packet.h>
#include <cpaf_libs/video/av_samples_buffer.h>
#include <video/av_frame.h>

namespace cpaf::gui::video {
class render;

};

namespace cpaf::video {
class av_format_context;
class av_codec_context;
class media_stream_time;
class av_samples_queue;

class video_render_thread
{
public:
    video_render_thread(const std::atomic_bool& threads_running, const std::atomic_bool& threads_paused);

    void                        format_context_set      (av_format_context* ctx)    { format_context_ptr_ = ctx; }
    void                        format_context_set      (av_format_context& ctx)    { format_context_ptr_ = &ctx; }
    void                        video_codec_ctx_set     (av_codec_context* ctx)     { video_codec_ctx_ptr_ = ctx; }
    void                        video_codec_ctx_set     (av_codec_context& ctx)     { video_codec_ctx_ptr_ = &ctx; }
    void                        audio_samples_queue_set (av_samples_queue* queue)   { audio_samples_queue_ptr_ = queue; }
    void                        audio_samples_queue_set (av_samples_queue& queue)   { audio_samples_queue_ptr_ = &queue; }
    void                        current_media_time_set  (media_stream_time* mts)    { current_media_time_ptr_ = mts; }
    void                        current_media_time_set  (media_stream_time& mts)    { current_media_time_ptr_ = &mts; }

    void                        start                   ();
    void                        terminate               ();
    bool                        video_frame_update      (av_frame& current_frame, cpaf::gui::video::render& video_render);
    void                        video_queue_flush_start ()                          { video_queue_flush_in_progress_ = true; }
    void                        video_queue_flush_done  ()                          { video_queue_flush_in_progress_ = false; video_queue_flushed_ = true; }

    const atomic_pipeline_index_t&      current_pipeline_index  () const { return current_pipeline_index_; }

private:
    bool                        video_frame_do_render   (av_frame& current_frame, cpaf::gui::video::render& video_render);
    void                        debug_video_frame_update(av_frame& current_frame, cpaf::gui::video::render& video_render);

    av_format_context&          format_context          () { return *format_context_ptr_; }
    const av_format_context&    format_context          () const { return *format_context_ptr_; }
    av_codec_context&           video_codec_ctx         () { return *video_codec_ctx_ptr_; }
    av_packet                   video_packet_queue_front();
    void                        video_packet_queue_pop  ();
    const packet_queue_t&       video_packet_queue      () const;
    const packet_queue_t&       video_packet_queue_const() const;
    const av_samples_queue&     audio_samples_queue     () const { return *audio_samples_queue_ptr_; }
    media_stream_time&          current_media_time      () { return *current_media_time_ptr_; }

    const std::atomic_bool&     threads_running         () const { return threads_running_; }
    const std::atomic_bool&     threads_paused          () const { return threads_paused_; }

    const std::atomic_bool&     threads_running_;
    const std::atomic_bool&     threads_paused_;

    av_format_context*          format_context_ptr_             = nullptr;
    av_codec_context*           video_codec_ctx_ptr_            = nullptr;
    av_samples_queue*           audio_samples_queue_ptr_        = nullptr;
    media_stream_time*          current_media_time_ptr_         = nullptr;
    pipeline_index_t            flush_in_progress_index_        = 0;
    atomic_pipeline_index_t     current_pipeline_index_         = 0;

    int                         video_frame_update_dbg_counter_ = 0;
    std::atomic_bool            video_queue_flush_in_progress_  = false;
    std::atomic_bool            video_queue_flushed_            = false;
};

} // namespace cpaf::video

