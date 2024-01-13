#pragma once


#include <string>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include <cpaf_libs/audio/cpaf_audio_device_base.h>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/av_samples_buffer.h>
#include <cpaf_libs/video/subtitle_frame.h>
#include <cpaf_libs/video/av_samples_queue.h>
#include <cpaf_libs/video/audio_resampler.h>
#include <cpaf_libs/gui/video/pipeline_threads/audio_render_thread.h>
#include <cpaf_libs/gui/video/pipeline_threads/audio_resampler_thread.h>
#include <cpaf_libs/gui/video/pipeline_threads/packet_reader_thread.h>
#include <cpaf_libs/gui/video/pipeline_threads/subtitle_reader_thread.h>
#include <cpaf_libs/gui/video/pipeline_threads/video_render_thread.h>

namespace cpaf::video {
class av_format_context;
class av_codec_context;
class audio_resampler;
class media_stream_time;
};

namespace cpaf::gui::video {
class player;

class pipeline_threads
{
    friend class packet_reader_thread;
public:
    using audio_play_callback_t  = cpaf::audio::device_base::play_callback_t;

    pipeline_threads(const pipeline_threads&) = delete;
    pipeline_threads& operator=(const pipeline_threads&)  = delete;

    explicit pipeline_threads(player& owning_player);
    ~pipeline_threads();

    void                        audio_resampler_set     (cpaf::video::audio_resampler* resampler);
    void                        audio_resampler_set     (cpaf::video::audio_resampler& resampler);
    audio_play_callback_t       audio_callback_get      ();

    void                        start                   ();
    void                        terminate               ();
    void                        seek_position           (const std::chrono::microseconds& stream_pos, cpaf::video::seek_dir dir);
    void                        seek_position           (const std::chrono::microseconds& stream_pos);
    void                        seek_relative           (const std::chrono::microseconds& delta_time);
    cpaf::video::seek_state_t   seek_state              () const { return seek_state_;      }
    std::chrono::microseconds   seek_from_position      () const { return packet_reader_thread_.seek_from_position(); }
    std::chrono::microseconds   seek_position_requested () const { return packet_reader_thread_.seek_position_requested(); }
    void                        pause_playback          ();
    void                        resume_playback         ();
    bool                        playback_paused         () const { return threads_paused_; }
    void                        video_frame_update      (cpaf::video::av_frame& current_frame, cpaf::gui::video::render& video_render);

    cpaf::video::av_samples_queue& audio_samples_queue() { return audio_samples_queue_; }
    const cpaf::video::av_samples_queue&       audio_samples_queue () const { return audio_samples_queue_; }
private:
    void                        flush_queues            ();
    void                        signal_flush_start      ();
    void                        signal_flush_done       ();

    cpaf::video::audio_resampler&        audio_sampler           () { return *audio_resampler_ptr_; }

    player&                                     player_;
    cpaf::video::av_samples_queue               audio_samples_queue_;
    cpaf::video::subtitles_queue                subtitles_queue_;
//TODO: Nove this to here    cpaf::video::audio_resampler                audio_resampler_;
    packet_reader_thread                        packet_reader_thread_;
    audio_resampler_thread                      audio_resampler_thread_;
    audio_render_thread                         audio_render_thread_;
    subtitle_reader_thread                      subtitle_reader_thread_;
    video_render_thread                         video_render_thread_;
    cpaf::video::audio_resampler*               audio_resampler_ptr_            = nullptr;
    std::atomic<cpaf::video::seek_state_t>      seek_state_                     = cpaf::video::seek_state_t::ready;
    std::atomic_bool                            threads_running_                = true;
    std::atomic_bool                            threads_paused_                 = false;
};

} //END namespace cpaf::gui::video




