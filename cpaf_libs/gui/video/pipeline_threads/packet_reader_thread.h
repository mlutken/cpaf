#pragma once

#include <chrono>
#include <thread>
#include <memory>
#include <atomic>

#include <concurrent/srsw_fifo_s.hpp>
#include <cpaf_libs/video/av_util.h>

namespace cpaf::video {
class av_format_context;
class media_stream_time;

};

namespace cpaf::gui::video {
class audio_resampler_thread;
class audio_render_thread;
class video_render_thread;
class pipeline_threads;
class player;



class packet_reader_thread
{
public:
    packet_reader_thread(player& owning_player,
                         const std::atomic_bool& threads_running,
                         const std::atomic_bool& threads_paused,
                         std::atomic<cpaf::video::seek_state_t>& seek_state);
    void                                pipeline_threads_set    (pipeline_threads* ptr)     { pipeline_threads_ptr_ = ptr; }
    void                                run                     ();
    bool                                seek_position           (const std::chrono::microseconds& stream_pos, cpaf::video::seek_dir dir);
    bool                                seek_position           (const std::chrono::microseconds& stream_pos);

    std::chrono::microseconds           seek_from_position      () const { return seek_from_position_; }
    std::chrono::microseconds           seek_position_requested () const { return seek_time_pos_requested_; }
    const std::atomic_bool&             thread_is_paused        () const { return thread_is_paused_; }
    const std::atomic_bool&             thread_is_running       () const { return thread_is_running_; }
    void                                debug_print_info        () const;

private:
    void                    thread_function         ();
    void                    check_seek_position     ();
    void                    check_seek_completed    ();
    void                    flush_queues            ();
    void                    signal_flush_start      ();
    void                    signal_flush_done       ();

    player&                                 player_;
    const std::atomic_bool&                 threads_running_;
    const std::atomic_bool&                 threads_paused_;
    std::atomic<cpaf::video::seek_state_t>& seek_state_;
    std::atomic_bool                        thread_is_paused_ = true;
    std::atomic_bool                        thread_is_running_ = false;

    std::chrono::microseconds               seek_from_position_;
    std::chrono::microseconds               seek_time_pos_requested_;
    std::chrono::steady_clock::time_point   flush_done_time_point_;
    cpaf::video::seek_dir                   seek_direction_                 = cpaf::video::seek_dir::forward;
    pipeline_threads*                       pipeline_threads_ptr_           = nullptr;

    std::chrono::microseconds               read_packets_yield_time_        = std::chrono::milliseconds(1);
    uint32_t                                primary_queue_fill_level_       = 30;
    std::unique_ptr<std::thread>            read_packets_thread_;

};

} // namespace cpaf::gui::video

