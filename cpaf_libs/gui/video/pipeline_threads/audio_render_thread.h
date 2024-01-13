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
                                 std::atomic<cpaf::video::seek_state_t>& seek_state);

    void                    start                   ();
    audio_play_callback_t   audio_callback_get      ();

private:
    void                    audio_callback_function (uint8_t* stream, int32_t length);
    void                    render_audio_silence    (uint8_t* stream, int32_t length);
    void                    debug_audio_callback    (uint8_t* stream, int32_t length);

    cpaf::video::av_samples_queue&       audio_samples_queue     () { return audio_samples_queue_; }

    player&                                     player_;
    pipeline_threads&                           pipeline_threads_;
    cpaf::video::av_samples_queue&              audio_samples_queue_;
    std::atomic<cpaf::video::seek_state_t>&     seek_state_;
    std::chrono::microseconds                   sync_ok_interval                = std::chrono::milliseconds(15);
    int                                         audio_callback_dbg_counter_     = 0;
};

} // namespace cpaf::gui::video

