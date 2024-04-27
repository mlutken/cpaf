#pragma once

#include <chrono>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>

#include <concurrent/srsw_fifo.hpp>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/subtitle_frame.h>
#include <cpaf_libs/video/io/subtitle_text_file_data.h>

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

    void                                run                     ();
    void                                enqueue_subtitle        (const std::string& subtitle_path,
                                                                 const std::string& language_code,
                                                                 std::chrono::microseconds subtitle_adjust_offset = {});
    std::unique_ptr<subtitle_container> dequeue_subtitle        ();
    bool                                has_subtitle            () const;

    void                                cancel_download         ();
    subtitle_file_state_t               subtitle_file_state     () const { return subtitle_file_state_; }
    float                               download_progress                () const;

private:
    struct job_t {
        std::string                 subtitle_path           {};
        std::string                 language_code           {};
        std::chrono::microseconds   subtitle_adjust_offset  {};
    };
    using queue_in_t  = estl::srsw_fifo<job_t>;
    using queue_out_t = estl::srsw_fifo<std::unique_ptr<subtitle_container>>;

    void                                thread_function         ();
    void                                download_subtitle       (const std::string& subtitle_path,
                                                                 const std::string& language_code,
                                                                 std::chrono::microseconds subtitle_adjust_offset = {});

    player&                             player_;
    queue_in_t                          queue_in_;
    queue_out_t                         queue_out_;
    std::atomic_bool                    thread_is_running_ = true;

    std::chrono::microseconds           thread_yield_time_              = std::chrono::milliseconds(100);
    std::chrono::milliseconds           download_time_out_              = std::chrono::seconds(60);

    std::unique_ptr<std::thread>        thread_object_;
    std::atomic<subtitle_file_state_t>  subtitle_file_state_            = subtitle_file_state_t::inactive;
    subtitle_text_file_data             subtitle_text_file_data_;

};

} // namespace cpaf::gui::video

