#pragma once

#include <chrono>
#include <thread>
#include <memory>
#include <atomic>

#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/av_samples_buffer.h>

namespace cpaf::video {
class av_format_context;
class av_codec_context;
class audio_resampler;
class media_stream_time;
class av_samples_queue;
};

namespace cpaf::gui::video {
class player;

class audio_resampler_thread
{
public:
    audio_resampler_thread( player& owning_player,
                            cpaf::video::av_samples_queue& audio_samples_queue,
                            const std::atomic_bool& threads_running,
                            const std::atomic_bool& threads_paused);
    void                    audio_resampler_set     (cpaf::video::audio_resampler* resampler){ audio_resampler_ptr_ = resampler; }
    void                    audio_resampler_set     (cpaf::video::audio_resampler& resampler){ audio_resampler_ptr_ = &resampler; }
    void                    audio_samples_queue_set (cpaf::video::av_samples_queue* queue)   { audio_samples_queue_ptr_ = queue; }
    void                    audio_samples_queue_set (cpaf::video::av_samples_queue& queue)   { audio_samples_queue_ptr_ = &queue; }
    void                    samples_queue_flush_start()                         { samples_queue_flush_in_progress_ = true; }
    void                    samples_queue_flush_done()                          { samples_queue_flush_in_progress_ = false; samples_queue_flushed_ = true; }

    void                    start                   ();

private:
    void                    thread_function         ();
    void                    resample_frame          (bool& add_samples, const std::chrono::microseconds& cur_media_time_pos);

    const std::atomic_bool&             threads_running         () const { return threads_running_; }
    const std::atomic_bool&             threads_paused          () const { return threads_paused_; }
    cpaf::video::audio_resampler&       audio_sampler           () { return *audio_resampler_ptr_; }
    cpaf::video::av_samples_queue&      audio_samples_queue     () { return *audio_samples_queue_ptr_; }

    player&                             player_;
    const std::atomic_bool&             threads_running_;
    const std::atomic_bool&             threads_paused_;

    cpaf::video::audio_resampler*       audio_resampler_ptr_            = nullptr;
    cpaf::video::av_samples_queue*      audio_samples_queue_ptr_        = nullptr;

    uint32_t                            audio_samples_fill_level_       = 30;
    std::chrono::microseconds           audio_samples_yield_time_       = std::chrono::milliseconds(1);
    std::chrono::microseconds           audio_samples_read_ahead_time_  = std::chrono::milliseconds(300);
    std::atomic_bool                    samples_queue_flush_in_progress_= false;
    std::atomic_bool                    samples_queue_flushed_          = false;

    std::unique_ptr<std::thread>        thread_object_;

};


} // namespace cpaf::gui::video

