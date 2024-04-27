#pragma once

#include <chrono>
#include <thread>
#include <concurrent/mutex_fifo.hpp>
#include <cpaf_libs/gui/video/data_definitions/playable.h>


namespace cpaf::gui::video {
class player;

/// @todo make non copyable etc...
class play_handler_thread
{
public:
    play_handler_thread() = delete;
    explicit play_handler_thread(player& owning_player);

    ~play_handler_thread();

    void                    run             ();
    void                    terminate       ();
    void                    open_async      (playable playab);
    void                    close_async     ();


private:
    playable&               current_playable();
    const playable&         current_playable() const;

    struct job_t {
        enum class type_t { open, close };
        playable    playab;
        type_t      typ;
        bool        done = false;
    };

    using job_queue_t = estl::mutex_fifo<job_t>;

    void                                thread_function     ();
    void                                perform_open        (job_t job);
    void                                perform_close       (job_t job);

    void                                set_current_job     (job_t job);
    void                                set_current_job_done();

    player&                             player_;
    job_queue_t                         job_queue_;
    mutable std::mutex                  current_playable_mutex_;
    job_t                               current_job_;

    std::atomic_bool                    thread_is_running_  = false;
    std::chrono::microseconds           thread_yield_time_  = std::chrono::milliseconds(1);
    std::unique_ptr<std::thread>        thread_object_;
};

} // namespace cpaf::gui::video

