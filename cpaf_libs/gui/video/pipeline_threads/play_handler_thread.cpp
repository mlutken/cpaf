#include "play_handler_thread.h"
#include <fmt/format.h>
#include <cpaf_libs/gui/video/player.h>

using namespace std::chrono;

namespace cpaf::gui::video {

play_handler_thread::play_handler_thread(player& owning_player)
    : player_(owning_player),
      job_queue_(8)
{
}

play_handler_thread::~play_handler_thread()
{
    terminate();
}

void play_handler_thread::run()
{
    thread_object_ = std::make_unique<std::thread>(&play_handler_thread::thread_function, this);
    thread_object_->detach();
}

void play_handler_thread::terminate()
{
    thread_is_running_ = false;
}

void play_handler_thread::open_async(playable playab)
{
    job_queue_.emplace(playab, job_t::type_t::open);
}

void play_handler_thread::close_async()
{
    job_queue_.emplace(playable(), job_t::type_t::close);
    // job_queue_.push({playable(), job_t::type_t::close, false});
}

playable& play_handler_thread::current_playable()
{
    // std::scoped_lock<std::mutex> lock(current_playable_mutex_);
    return current_job_.playab;
}

const playable& play_handler_thread::current_playable() const
{
    // std::scoped_lock<std::mutex> lock(current_playable_mutex_);
    return current_job_.playab;
}

void play_handler_thread::thread_function()
{
    thread_is_running_ = true;
    while(thread_is_running_) {
        // std::cerr << "FIXMENM: play_handler_thread::thread_function()...\n";

        job_t job;
        if (job_queue_.pop_front(job)) {
            switch (job.typ) {
            case job_t::type_t::open: perform_open(job);break;
            case job_t::type_t::close: perform_close(job);break;
            default:
                break;
            }
        }
        std::this_thread::sleep_for(thread_yield_time_);
    }
    std::cerr << "LOG_INFO: EXIT play_handler_thread::thread_function()!!!\n";
    // thread_is_running_ = false;
}

void play_handler_thread::perform_open(job_t job)
{
    std::cerr << "FIXMENM: 1 play_handler_thread::perform_open()!!!\n";
    set_current_job(std::move(job));
    player_.open_command(current_playable());

    // auto stream_state_expected = stream_state_t::open;
    // if (player_.primary_stream_state().compare_exchange_strong(stream_state_expected, stream_state_t::playing)) {
    //     player_.start_playing(current_playable().start_time());
    //     if (player_.cb_start_playing_) { player_.cb_start_playing_(); }
    // }

    set_current_job_done();
    std::cerr << "FIXMENM: 2 play_handler_thread::perform_open()!!!\n";
}

void play_handler_thread::perform_close(job_t job)
{
    std::cerr << "FIXMENM: 1 play_handler_thread::perform_close()!!!\n";
    set_current_job(std::move(job));
    player_.close_command();
    set_current_job_done();
    std::cerr << "FIXMENM: 2 play_handler_thread::perform_close()!!!\n";
}

void play_handler_thread::set_current_job(job_t job)
{
    // std::scoped_lock<std::mutex> lock(current_playable_mutex_);
    current_job_ = std::move(job);
}

void play_handler_thread::set_current_job_done()
{
  // std::scoped_lock<std::mutex> lock(current_playable_mutex_);
    current_job_.done = true;
}

} // namespace cpaf::gui::video
