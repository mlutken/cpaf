#pragma once

#include <chrono>
#include <thread>
#include <memory>
#include <atomic>

#include <concurrent/srsw_fifo_s.hpp>
#include <cpaf_libs/video/av_util.h>

namespace cpaf::video {
class av_format_context;
class audio_resampler_thread;
class audio_render_thread;
class video_render_thread;

class packet_reader_thread
{
public:
    packet_reader_thread(const std::atomic_bool& threads_running, const std::atomic_bool& threads_paused);
    void                    format_context_set      (av_format_context* ctx)    { format_context_ptr_ = ctx; }
    void                    format_context_set      (av_format_context& ctx)    { format_context_ptr_ = &ctx; }

    void                    start                   ();
    pipeline_index_t        seek_position           (const std::chrono::microseconds& stream_pos, seek_dir dir);
    pipeline_index_t        seek_position           (const std::chrono::microseconds& stream_pos);

private:
    using flush_queue_t = estl::srsw_fifo_s<pipeline_index_t, 32>;

    void                    read_packets_thread_fn  ();
    void                    check_seek_position     ();

    av_format_context&      format_context          () { return *format_context_ptr_; }
    const std::atomic_bool& threads_running         () const { return threads_running_; }
    const std::atomic_bool& threads_paused          () const { return threads_paused_; }

    const std::atomic_bool&         threads_running_;
    const std::atomic_bool&         threads_paused_;

    flush_queue_t                   flush_queue_;
    std::atomic_bool                seek_requested_                 = false;
    std::chrono::microseconds       seek_position_requested_;
    seek_dir                        seek_direction_                 = seek_dir::forward;
    av_format_context*              format_context_ptr_             = nullptr;
    audio_resampler_thread*         audio_resampler_thread_             = nullptr;
    audio_render_thread*            audio_render_thread_             = nullptr;
    video_render_thread*            video_render_thread_             = nullptr;

    std::chrono::microseconds       read_packets_yield_time_        = std::chrono::milliseconds(1);
    uint32_t                        primary_queue_fill_level_       = 30;
    std::atomic<pipeline_index_t>   flush_to_index_requested_index_ = 0;
    std::unique_ptr<std::thread>    read_packets_thread_;

};

} // namespace cpaf::video

