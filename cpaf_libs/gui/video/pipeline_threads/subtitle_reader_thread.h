#pragma once

#include <chrono>
#include <thread>
#include <memory>
#include <atomic>

#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/subtitle_frame.h>

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
                           cpaf::video::subtitles_queue& subtitles_queue,
                           const std::atomic_bool& threads_running,
                           const std::atomic_bool& threads_paused);

    void                                start                   ();
    const std::atomic_bool&             thread_is_paused        () const { return thread_is_paused_; }

private:
    void                                thread_function         ();
    void                                read_from_stream ();

    player&                             player_;
    cpaf::video::subtitles_queue&       subtitles_queue_;
    const std::atomic_bool&             threads_running_;
    const std::atomic_bool&             threads_paused_;
    std::atomic_bool                    thread_is_paused_ = false;
    cpaf::video::subtitle_frame         current_subtitle_frame_     {};


    std::chrono::microseconds           thread_yield_time_              = std::chrono::milliseconds(1);
//    std::chrono::microseconds       audio_samples_read_ahead_time_  = std::chrono::milliseconds(300);

    std::unique_ptr<std::thread>        thread_object_;

};

} // namespace cpaf::gui::video

