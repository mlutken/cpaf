#pragma once

#include <chrono>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>

#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/subtitle_frame.h>

namespace cpaf::video {
class av_format_context;
class av_codec_context;
class media_stream_time;
class subtitle_container;
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

    ~subtitle_reader_thread();

    void                                start                   ();
    const std::atomic_bool&             thread_is_paused        () const { return thread_is_paused_; }
    void                                subtitle_container_set  (std::unique_ptr<subtitle_container> container);

private:
    void                                thread_function         ();
    void                                read_from_stream        ();

    player&                             player_;
    cpaf::video::subtitles_queue&       subtitles_queue_;
    const std::atomic_bool&             threads_running_;
    const std::atomic_bool&             threads_paused_;
    std::atomic_bool                    thread_is_paused_ = true;
    cpaf::video::subtitle_frame         current_subtitle_frame_     {};


    std::chrono::microseconds           thread_yield_time_              = std::chrono::milliseconds(1);
//    std::chrono::microseconds       audio_samples_read_ahead_time_  = std::chrono::milliseconds(300);

    std::unique_ptr<std::thread>        thread_object_;
    std::unique_ptr<subtitle_container> subtitle_container_;
    std::mutex                          subtitle_container_mutex_;

};

} // namespace cpaf::gui::video

