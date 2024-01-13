#pragma once

#include <chrono>
#include <atomic>
#include <concurrent/srsw_fifo_s.hpp>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/av_packet.h>
#include <cpaf_libs/video/av_samples_buffer.h>
#include <video/av_frame.h>

namespace cpaf::video {
class av_format_context;
class av_codec_context;
class media_stream_time;
class av_samples_queue;
class subtitle_frame;
};



namespace cpaf::gui::video {
class render;
class player;

};

namespace cpaf::gui::video {

class video_render_thread
{
public:
    video_render_thread(player& owning_player,
                        cpaf::video::av_samples_queue& audio_samples_queue,
                        const std::atomic_bool& threads_running,
                        const std::atomic_bool& threads_paused,
                        std::atomic<cpaf::video::seek_state_t>& seek_state);
    void                        start                       ();
    void                        terminate                   ();
    void                        video_frame_update          (cpaf::video::av_frame& current_frame, cpaf::gui::video::render& video_render);
    void                        video_queue_flush_start     ()                          { video_queue_flush_in_progress_ = true; }
    void                        video_queue_flush_done      ()                          { video_queue_flush_in_progress_ = false; video_queue_flushed_ = true; }
    std::chrono::microseconds   time_to_current_frame       (cpaf::video::av_frame& current_frame) const;
    std::chrono::microseconds   time_to_current_frame_abs   (cpaf::video::av_frame& current_frame) const;
    bool                        is_seek_currently_possible  () const;

private:
    bool                                    video_frame_do_render   (cpaf::video::av_frame& current_frame, cpaf::gui::video::render& video_render);
    void                                    update_is_seek_possible (cpaf::video::av_frame& current_frame);
    cpaf::video::subtitle_frame             current_subtitle        () const;

    void                                    debug_video_frame_update(cpaf::video::av_frame& current_frame, cpaf::gui::video::render& video_render);

    cpaf::video::av_packet                  video_packet_queue_front();
    void                                    video_packet_queue_pop  ();
    const cpaf::video::packet_queue_t&      video_packet_queue      () const;
    const cpaf::video::packet_queue_t&      video_packet_queue_const() const;
    const cpaf::video::av_samples_queue&    audio_samples_queue     () const { return audio_samples_queue_; }

    const std::atomic_bool&                 threads_running         () const { return threads_running_; }
    const std::atomic_bool&                 threads_paused          () const { return threads_paused_; }

    player&                                 player_;
    cpaf::video::av_samples_queue&          audio_samples_queue_;
    const std::atomic_bool&                 threads_running_;
    const std::atomic_bool&                 threads_paused_;
    std::atomic<cpaf::video::seek_state_t>& seek_state_;

    int                                     video_frame_update_dbg_counter_ = 0;
    std::atomic_bool                        video_queue_flush_in_progress_  = false;
    std::atomic_bool                        video_queue_flushed_            = false;
    bool                                    is_seek_currently_possible_     = false;
};

} // namespace cpaf::gui::video

