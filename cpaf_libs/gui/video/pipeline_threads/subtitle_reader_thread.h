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

    void                                start                   ();

private:
    void                                thread_function         ();
    void                                resample_frame          (bool& add_samples, const std::chrono::microseconds& cur_media_time_pos);
    const std::atomic_bool&             threads_running         () const { return threads_running_; }
    const std::atomic_bool&             threads_paused          () const { return threads_paused_; }

    player&                             player_;
    const std::atomic_bool&             threads_running_;
    const std::atomic_bool&             threads_paused_;


    std::chrono::microseconds           thread_yield_time_              = std::chrono::milliseconds(1);
//    std::chrono::microseconds       audio_samples_read_ahead_time_  = std::chrono::milliseconds(300);

    std::unique_ptr<std::thread>        thread_object_;

};

} // namespace cpaf::gui::video

