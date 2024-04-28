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
class player;

class pipeline_threads;

/** Note this is not running it's own thread, but supplies a callback
    for a cpaf::audio_device */
class audio_render_thread
{
public:
    using audio_play_callback_t  = cpaf::audio::device_base::play_callback_t;
    explicit audio_render_thread(player& owning_player,
                                 pipeline_threads& pline_threads,
                                 cpaf::video::av_samples_queue& audio_samples_queue,
                                 const std::atomic_bool& threads_running,
                                 const std::atomic_bool& threads_started,
                                 const std::atomic_bool& threads_paused,
                                 std::atomic<cpaf::video::seek_state_t>& seek_state);

    void                        run                     ();
    void                        terminate               ();
    audio_play_callback_t       audio_callback_get      ();
    bool                        thread_is_running       () const { return threads_running_; }
    bool                        thread_is_stopped       () const { return !threads_started_; }
    bool                        thread_is_paused        () const { return threads_paused_; }
    std::chrono::microseconds   front_presentation_time    () const;


private:
    void                        audio_callback_function (uint8_t* stream, int32_t length);
    void                        render_audio_silence    (uint8_t* stream, int32_t length);
    void                        debug_audio_callback    (uint8_t* stream, int32_t length);

    player&                                     player_;
    pipeline_threads&                           pipeline_threads_;
    cpaf::video::av_samples_queue&              audio_samples_queue_;
    const std::atomic_bool&                     threads_running_;
    const std::atomic_bool&                     threads_started_;
    const std::atomic_bool&                     threads_paused_;
    std::atomic<cpaf::video::seek_state_t>&     seek_state_;
    std::chrono::microseconds                   sync_ok_interval_               = std::chrono::milliseconds(15);
    int                                         audio_callback_dbg_counter_     = 0;
};

} // namespace cpaf::gui::video

