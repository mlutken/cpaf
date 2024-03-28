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

/// @todo make non copyable etc...
class subtitle_reader_thread
{
public:

    subtitle_reader_thread() = delete;
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
    void                                read_from_container     ();
    void                                push_from_container     (std::chrono::microseconds presentation_time);

    std::mutex                          subtitle_container_mutex_;
    player&                             player_;
    cpaf::video::subtitles_queue&       subtitles_queue_;
    const std::atomic_bool&             threads_running_;
    const std::atomic_bool&             threads_paused_;
    std::atomic_bool                    thread_is_paused_ = true;
    cpaf::video::subtitle_frame         current_subtitle_frame_     {};


    std::chrono::microseconds           thread_yield_time_              = std::chrono::milliseconds(1);

    std::unique_ptr<std::thread>        thread_object_;
    std::unique_ptr<subtitle_container> subtitle_container_;

};

} // namespace cpaf::gui::video

