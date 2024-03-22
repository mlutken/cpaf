#pragma once

#include <chrono>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>

#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/subtitle_frame.h>

namespace cpaf::video {
class subtitle_container;
};

using namespace cpaf::video;

namespace cpaf::gui::video {
class player;

/// @todo make non copyable etc...
class subtitle_downloader_thread
{
public:

    subtitle_downloader_thread() = delete;
    subtitle_downloader_thread(player& owning_player);

    ~subtitle_downloader_thread();

    void                                start                   ();
    void                                cancel_download         ();
    subtitle_file_state_t               subtitle_file_state     () const { return subtitle_file_state_; }
    float                               progress                () const;

private:
    void                                thread_function         ();

    player&                             player_;
    std::atomic_bool                    thread_is_running_ = true;

    std::chrono::microseconds           thread_yield_time_              = std::chrono::milliseconds(1);

    std::unique_ptr<std::thread>        thread_object_;
    std::atomic<subtitle_file_state_t>  subtitle_file_state_            = subtitle_file_state_t::inactive;

};

} // namespace cpaf::gui::video

