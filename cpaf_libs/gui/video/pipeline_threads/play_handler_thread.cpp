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

void play_handler_thread::start()
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
        std::this_thread::sleep_for(thread_yield_time_);
    }
    std::cerr << "LOG_INFO: EXIT play_handler_thread::thread_function()!!!\n";
    // thread_is_running_ = false;
}

void play_handler_thread::perform_open(job_t&& job)
{
    set_current_job(std::move(job));
    player_.open(current_playable());
}

void play_handler_thread::peform_close(job_t&& job)
{
    set_current_job(std::move(job));
    player_.close();
}

void play_handler_thread::set_current_job(job_t&& job)
{
    // std::scoped_lock<std::mutex> lock(current_playable_mutex_);
    current_job_ = std::move(job);
}


} // namespace cpaf::gui::video
