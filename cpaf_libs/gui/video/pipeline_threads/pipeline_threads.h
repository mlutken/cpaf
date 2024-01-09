#pragma once


#include <string>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include <cpaf_libs/audio/cpaf_audio_device_base.h>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/av_samples_buffer.h>
#include <cpaf_libs/gui/video/pipeline_threads/audio_render_thread.h>
#include <cpaf_libs/gui/video/pipeline_threads/audio_resampler_thread.h>
#include <cpaf_libs/gui/video/pipeline_threads/packet_reader_thread.h>
#include <cpaf_libs/gui/video/pipeline_threads/video_render_thread.h>

namespace cpaf::video {
class av_format_context;
class av_codec_context;
class audio_resampler;
class media_stream_time;
};

namespace cpaf::gui::video {

class pipeline_threads
{
    friend class packet_reader_thread;
public:
    using audio_play_callback_t  = cpaf::audio::device_base::play_callback_t;

    pipeline_threads(const pipeline_threads&) = delete;
    pipeline_threads& operator=(const pipeline_threads&)  = delete;

    pipeline_threads();
    ~pipeline_threads();
//    pipeline_threads(std::atomic_bool& threads_running, std::atomic_bool& threads_paused);

    void                        format_context_set      (cpaf::video::av_format_context* ctx);
    void                        format_context_set      (cpaf::video::av_format_context& ctx);
    void                        video_codec_ctx_set     (cpaf::video::av_codec_context* ctx);
    void                        video_codec_ctx_set     (cpaf::video::av_codec_context& ctx);
    void                        audio_codec_ctx_set     (cpaf::video::av_codec_context* ctx);
    void                        audio_codec_ctx_set     (cpaf::video::av_codec_context& ctx);
    void                        audio_resampler_set     (cpaf::video::audio_resampler* resampler);
    void                        audio_resampler_set     (cpaf::video::audio_resampler& resampler);
    void                        audio_samples_queue_set (cpaf::video::av_samples_queue* queue);
    void                        audio_samples_queue_set (cpaf::video::av_samples_queue& queue);
    void                        current_media_time_set  (cpaf::video::media_stream_time* mts);
    void                        current_media_time_set  (cpaf::video::media_stream_time& mts);
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
private:
    void                        flush_queues            ();
    void                        signal_flush_start      ();
    void                        signal_flush_done       ();

    cpaf::video::av_format_context&      format_context          () { return *format_context_ptr_; }
    cpaf::video::av_codec_context&       audio_codec_ctx         () { return *audio_codec_ctx_ptr_; }
    cpaf::video::av_codec_context&       video_codec_ctx         () { return *video_codec_ctx_ptr_; }
    cpaf::video::audio_resampler&        audio_sampler           () { return *audio_resampler_ptr_; }
    cpaf::video::av_samples_queue&       audio_samples_queue     () { return *audio_samples_queue_ptr_; }
    cpaf::video::media_stream_time&      current_media_time      () { return *current_media_time_ptr_; }

    std::atomic<cpaf::video::seek_state_t>      seek_state_                     = cpaf::video::seek_state_t::ready;
    packet_reader_thread                        packet_reader_thread_;
    audio_resampler_thread                      audio_resampler_thread_;
    audio_render_thread                         audio_render_thread_;
    video_render_thread                         video_render_thread_;
    cpaf::video::av_format_context*             format_context_ptr_             = nullptr;
    cpaf::video::av_codec_context*              audio_codec_ctx_ptr_            = nullptr;
    cpaf::video::av_codec_context*              video_codec_ctx_ptr_            = nullptr;
    cpaf::video::audio_resampler*               audio_resampler_ptr_            = nullptr;
    cpaf::video::av_samples_queue*              audio_samples_queue_ptr_        = nullptr;
    cpaf::video::media_stream_time*             current_media_time_ptr_         = nullptr;
    std::atomic_bool                            threads_running_                = true;
    std::atomic_bool                            threads_paused_                 = false;
    std::chrono::steady_clock::time_point       seek_flush_done_time_point_;

};


} //END namespace cpaf::gui::video




