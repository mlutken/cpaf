#pragma once

#include <chrono>
#include <thread>
#include <memory>
#include <atomic>

#include <cpaf_libs/video/av_util.h>

namespace cpaf::video {
class av_format_context;
class av_codec_context;
class media_stream_time;
};

using namespace cpaf::video;

namespace cpaf::gui::video {
class player;

class subtitle_reader_thread
{
public:
    subtitle_reader_thread(player& owning_player,
                           const std::atomic_bool& threads_running,
                           const std::atomic_bool& threads_paused);
    void                    format_context_set      (cpaf::video::av_format_context* ctx)    { format_context_ptr_ = ctx; }
    void                    format_context_set      (cpaf::video::av_format_context& ctx)    { format_context_ptr_ = &ctx; }
    void                    subtitle_codec_ctx_set  (cpaf::video::av_codec_context* ctx)     { subtitle_codec_ctx_ptr_ = ctx; }
    void                    subtitle_codec_ctx_set  (cpaf::video::av_codec_context& ctx)     { subtitle_codec_ctx_ptr_ = &ctx; }
    void                    current_media_time_set  (cpaf::video::media_stream_time* mts)    { current_media_time_ptr_ = mts; }
    void                    current_media_time_set  (cpaf::video::media_stream_time& mts)    { current_media_time_ptr_ = &mts; }

    void                    start                   ();

private:
    void                                thread_function         ();
    void                                resample_frame          (bool& add_samples, const std::chrono::microseconds& cur_media_time_pos);

    cpaf::video::av_format_context&     format_context          () { return *format_context_ptr_; }
    cpaf::video::av_codec_context&      audio_codec_ctx         () { return *subtitle_codec_ctx_ptr_; }
    cpaf::video::media_stream_time&     current_media_time      () { return *current_media_time_ptr_; }
    const std::atomic_bool&             threads_running         () const { return threads_running_; }
    const std::atomic_bool&             threads_paused          () const { return threads_paused_; }

    player&                             player_;
    const std::atomic_bool&             threads_running_;
    const std::atomic_bool&             threads_paused_;

    cpaf::video::av_format_context*     format_context_ptr_             = nullptr;
    cpaf::video::av_codec_context*      subtitle_codec_ctx_ptr_            = nullptr;
    cpaf::video::media_stream_time*     current_media_time_ptr_         = nullptr;

//    uint32_t                        audio_samples_fill_level_       = 30;
    std::chrono::microseconds           thread_yield_time_              = std::chrono::milliseconds(1);
//    std::chrono::microseconds       audio_samples_read_ahead_time_  = std::chrono::milliseconds(300);

    std::unique_ptr<std::thread>        thread_object_;

};

} // namespace cpaf::gui::video

