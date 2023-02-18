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

class audio_resampler_thread
{
public:
    audio_resampler_thread(const std::atomic_bool& threads_running, const std::atomic_bool& threads_paused);
    void                    format_context_set      (av_format_context* ctx)    { format_context_ptr_ = ctx; }
    void                    format_context_set      (av_format_context& ctx)    { format_context_ptr_ = &ctx; }
    void                    audio_codec_ctx_set     (av_codec_context* ctx)     { audio_codec_ctx_ptr_ = ctx; }
    void                    audio_codec_ctx_set     (av_codec_context& ctx)     { audio_codec_ctx_ptr_ = &ctx; }
    void                    audio_resampler_set     (audio_resampler* resampler){ audio_resampler_ptr_ = resampler; }
    void                    audio_resampler_set     (audio_resampler& resampler){ audio_resampler_ptr_ = &resampler; }
    void                    audio_samples_queue_set (av_samples_queue_t* queue) { audio_samples_queue_ptr_ = queue; }
    void                    audio_samples_queue_set (av_samples_queue_t& queue) { audio_samples_queue_ptr_ = &queue; }
    void                    current_media_time_set  (media_stream_time* mts)    { current_media_time_ptr_ = mts; }
    void                    current_media_time_set  (media_stream_time& mts)    { current_media_time_ptr_ = &mts; }
    void                    samples_queue_flush_start()                         { samples_queue_flush_in_progress_ = true; }
    void                    samples_queue_flush_done()                          { samples_queue_flush_in_progress_ = false; samples_queue_flushed_ = true; }

    void                    start                   ();

private:
    void                    thread_function         ();
    void                    resample_frame          (bool& add_samples, const std::chrono::microseconds& cur_media_time_pos);

    av_format_context&      format_context          () { return *format_context_ptr_; }
    av_codec_context&       audio_codec_ctx         () { return *audio_codec_ctx_ptr_; }
    audio_resampler&        audio_sampler           () { return *audio_resampler_ptr_; }
    av_samples_queue_t&     audio_samples_queue     () { return *audio_samples_queue_ptr_; }
    media_stream_time&      current_media_time      () { return *current_media_time_ptr_; }
    const std::atomic_bool& threads_running         () const { return threads_running_; }
    const std::atomic_bool& threads_paused          () const { return threads_paused_; }

    const std::atomic_bool&         threads_running_;
    const std::atomic_bool&         threads_paused_;

    av_format_context*              format_context_ptr_             = nullptr;
    av_codec_context*               audio_codec_ctx_ptr_            = nullptr;
    audio_resampler*                audio_resampler_ptr_            = nullptr;
    av_samples_queue_t*             audio_samples_queue_ptr_        = nullptr;
    media_stream_time*              current_media_time_ptr_         = nullptr;

    uint32_t                        audio_samples_fill_level_       = 30;
    std::chrono::microseconds       audio_samples_yield_time_       = std::chrono::milliseconds(1);
    std::chrono::microseconds       audio_samples_read_ahead_time_  = std::chrono::milliseconds(300);
    std::atomic_bool                samples_queue_flush_in_progress_= false;
    std::atomic_bool                samples_queue_flushed_          = false;

    std::unique_ptr<std::thread>    thread_object_;

};

} // namespace cpaf::video

