#pragma once

#include <chrono>
#include <atomic>
#include <concurrent/srsw_fifo_s.hpp>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/av_packet.h>
#include <cpaf_libs/video/av_samples_buffer.h>
#include <cpaf_libs/video/av_frame.h>
#include <cpaf_libs/video/subtitle_frame.h>

namespace cpaf::video {
class av_format_context;
class av_codec_context;
class media_stream_time;
class av_samples_queue;
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
                        cpaf::video::subtitles_queue& subtitles_queue,
                        const std::atomic_bool& threads_running,
                        const std::atomic_bool& threads_paused,
                        std::atomic<cpaf::video::seek_state_t>& seek_state);
    void                                    start                       ();
    void                                    terminate                   ();
    void                                    video_frame_update          (cpaf::video::av_frame& current_frame,
                                                                         cpaf::gui::video::render& video_render);
    void                                    video_queue_flush_start     ();
    void                                    video_queue_flush_done      ();
    std::chrono::microseconds               time_to_current_frame       (cpaf::video::av_frame& current_frame) const;
    std::chrono::microseconds               time_to_current_frame_abs   (cpaf::video::av_frame& current_frame) const;

private:
    bool                                    video_frame_do_render       (cpaf::video::av_frame& current_frame,
                                                                         cpaf::gui::video::render& video_render);
    void                                    update_current_subtitle     (cpaf::gui::video::render& video_render);
    cpaf::video::subtitle_frame             test_subtitle            () const;
    bool                                    subtitle_within_display_time(const cpaf::video::subtitle_frame& subtitle) const;
    bool                                    subtitle_too_old            (const cpaf::video::subtitle_frame& subtitle) const;

    void                                    debug_video_frame_update    (cpaf::video::av_frame& current_frame,
                                                                         cpaf::gui::video::render& video_render);

    cpaf::video::av_packet                  video_packet_queue_front    ();
    void                                    video_packet_queue_pop      ();
    const cpaf::video::packet_queue_t&      video_packet_queue          () const;
    const cpaf::video::packet_queue_t&      video_packet_queue_const    () const;
    const cpaf::video::av_samples_queue&    audio_samples_queue         () const { return audio_samples_queue_; }
    void                                    flush_queues                ();
    void                                    debug_gui                   ();

    player&                                 player_;
    cpaf::video::av_samples_queue&          audio_samples_queue_;
    cpaf::video::subtitles_queue&           subtitles_queue_;
    const std::atomic_bool&                 threads_running_;
    const std::atomic_bool&                 threads_paused_;
    std::atomic<cpaf::video::seek_state_t>& seek_state_;

    int                                     video_frame_update_dbg_counter_ = 0;
    std::atomic_bool                        video_queue_flush_in_progress_  = false;
    std::atomic_bool                        video_queue_flushed_            = false;
    std::atomic_bool                        flush_requested_                = false;
};

} // namespace cpaf::gui::video

