#pragma once

#include <chrono>
#include <thread>
#include <memory>
#include <atomic>

#include <concurrent/srsw_fifo_s.hpp>
#include <cpaf_libs/video/av_util.h>

namespace cpaf::video {
class av_format_context;

};

namespace cpaf::gui::video {
class audio_resampler_thread;
class audio_render_thread;
class video_render_thread;
class pipeline_threads;


class packet_reader_thread
{
public:
    packet_reader_thread(const std::atomic_bool& threads_running,
                         const std::atomic_bool& threads_paused,
                         std::atomic<cpaf::video::seek_state_t>& seek_state);
    void                    pipeline_threads_set    (pipeline_threads* ptr)     { pipeline_threads_ptr_ = ptr; }
    void                    format_context_set      (cpaf::video::av_format_context* ctx)    { format_context_ptr_ = ctx; }
    void                    format_context_set      (cpaf::video::av_format_context& ctx)    { format_context_ptr_ = &ctx; }

    void                    start                   ();
    cpaf::video::pipeline_index_t        seek_position           (const std::chrono::microseconds& stream_pos, cpaf::video::seek_dir dir);
    cpaf::video::pipeline_index_t        seek_position           (const std::chrono::microseconds& stream_pos);

private:
    using flush_queue_t = estl::srsw_fifo_s<cpaf::video::pipeline_index_t, 32>;

    void                    read_packets_thread_fn  ();
    void                    check_seek_position     ();
    void                    flush_queues            ();
    void                    signal_flush_start      ();
    void                    signal_flush_done       ();


    cpaf::video:: av_format_context&      format_context          () { return *format_context_ptr_; }
    const std::atomic_bool& threads_running         () const { return threads_running_; }
//    const std::atomic_bool& threads_paused          () const { return threads_paused_; }

    const std::atomic_bool&                 threads_running_;
    const std::atomic_bool&                 threads_paused_;
    std::atomic<cpaf::video::seek_state_t>& seek_state_;

    flush_queue_t                           flush_queue_;
    std::atomic_bool                        seek_requested_                 = false;
    std::atomic<bool>                       seek_in_progress_               = false;
    std::chrono::microseconds               seek_position_requested_;
    cpaf::video::seek_dir                   seek_direction_                 = cpaf::video::seek_dir::forward;
    pipeline_threads*                       pipeline_threads_ptr_           = nullptr;
    cpaf::video::av_format_context*         format_context_ptr_             = nullptr;

    std::chrono::microseconds               read_packets_yield_time_        = std::chrono::milliseconds(1);
    uint32_t                                primary_queue_fill_level_       = 30;
    std::atomic<cpaf::video::pipeline_index_t>   flush_to_index_requested_index_ = 0;
    std::unique_ptr<std::thread>            read_packets_thread_;
    std::chrono::nanoseconds                seek_throttle_time_        = std::chrono::milliseconds(200);
    std::chrono::steady_clock::time_point   last_seek_start_time_{};

};

} // namespace cpaf::gui::video


// Excessive padding in 'class cpaf::video::packet_reader_thread' (186 padding bytes, where 58 is optimal).
//          Optimal fields order:
//     flush_queue_,
//     threads_running_,
//     threads_paused_,
//     seek_position_requested_,
//     pipeline_threads_ptr_,
//     format_context_ptr_,
//     read_packets_yield_time_,
//     flush_to_index_requested_index_,
//     read_packets_thread_,
//     primary_queue_fill_level_,
//     seek_requested_,
//     seek_direction_,

//     consider reordering the fields or adding explicit padding members [clang-analyzer-optin.performance.Padding]
