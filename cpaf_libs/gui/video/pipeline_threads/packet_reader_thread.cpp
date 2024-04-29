#include "packet_reader_thread.h"
#include <cpaf_libs/time/cpaf_time.h>
#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/video/media_stream_time.h>
#include <cpaf_libs/gui/video/pipeline_threads/pipeline_threads.h>
#include <cpaf_libs/gui/video/player.h>

using namespace std;
using namespace cpaf::video;
using namespace std::chrono;
using namespace std::chrono_literals;

namespace cpaf::gui::video {

packet_reader_thread::packet_reader_thread(
    player& owning_player,
    const std::atomic_bool& threads_running,
    const std::atomic_bool& threads_started,
    const std::atomic_bool& threads_paused,
    std::atomic<seek_state_t>& seek_state)
    : player_(owning_player)
    , threads_running_(threads_running)
    , threads_started_(threads_started)
    , threads_paused_(threads_paused)
    , seek_state_(seek_state)
{
}

void packet_reader_thread::run()
{
    read_packets_thread_  = std::make_unique<std::thread>(&packet_reader_thread::thread_function, this);
    read_packets_thread_->detach();
}

bool packet_reader_thread::seek_position(const std::chrono::microseconds& stream_pos, cpaf::video::seek_dir dir)
{
    auto seek_state_expected1 = seek_state_t::ready;
    auto seek_state_expected2 = seek_state_t::waiting_for_sync;
    if (seek_state_.compare_exchange_strong(seek_state_expected1, seek_state_t::requested) ||
        seek_state_.compare_exchange_strong(seek_state_expected2, seek_state_t::requested)
        )
    {
        seek_from_position_ = player_.cur_media_time().current_time_pos();
        seek_time_pos_requested_ = stream_pos;
        seek_direction_ = dir;
        return true;
    }
    return false;
}

bool packet_reader_thread::seek_position(const std::chrono::microseconds& stream_pos)
{
    return seek_position(stream_pos, cpaf::video::seek_dir::forward);
}

void packet_reader_thread::debug_print_info() const
{
    std::cerr << "\n!!! FIXMENM packet reader  seek req time: " << cpaf::time::format_h_m_s(seek_time_pos_requested_)
              << " player time: " << cpaf::time::format_h_m_s(player_.current_time())
              << " !!!\n\n";
}

/// @todo add a possible nullptr to threads_running_ to the read_packets_to_queues() so
/// we can terminate the loop it holds in case the thread needs to stop!
void packet_reader_thread::thread_function()
{
    thread_is_running_ = true;
    while(threads_running_) {
        thread_is_paused_  = threads_paused_ == true;
        thread_is_stopped_ = threads_started_ == false;
        work_function();
        std::this_thread::sleep_for(thread_yield_time_);
    }
    std::cerr << "\n!!! EXIT packet_reader_thread::read_packets_thread_fn() !!!\n";
    thread_is_running_ = false;
}

void packet_reader_thread::work_function()
{
    if (!threads_started_) {
        return;
    }
    check_seek_position();
    check_seek_completed();
    if (threads_paused_)  { return; }

    const auto mt = player_.format_context().primary_media_type();
    player_.format_context().read_packets_to_queues(mt, primary_queue_fill_level_);
}

void packet_reader_thread::check_seek_position()
{
    auto seek_state_expected = seek_state_t::requested;
    if (seek_state_.compare_exchange_strong(seek_state_expected, seek_state_t::flushing)) {
        const auto mt = player_.format_context().primary_media_type();
        signal_flush_start();
        player_.format_context().read_packets_to_queues(mt, primary_queue_fill_level_ + 1);
        player_.format_context().seek_time_pos(mt, seek_time_pos_requested_, seek_direction_);
        flush_queues();
        player_.format_context().read_packets_to_queues(mt, primary_queue_fill_level_);
        signal_flush_done();
        seek_state_ = seek_state_t::flush_done;
        flush_done_time_point_ = std::chrono::steady_clock::now();
//        std::this_thread::sleep_for(5ms);   // @todo Should we have a delay at all perhaps 0ms or 1ms! ?? FIXMENM
        seek_state_ = seek_state_t::waiting_for_sync;
    }
}

void cpaf::gui::video::packet_reader_thread::check_seek_completed()
{
    if (seek_state_ != seek_state_t::waiting_for_sync) {
        return;
    }
    // A timeout fallback in case the normal test fails due to thread getting exausted!
    if ( std::chrono::steady_clock::now() - flush_done_time_point_ > 2s) {
        seek_state_ = seek_state_t::ready;
        if (pipeline_threads_ptr_) {
            std::cerr << "LOG_WARNING FALL back in sync!\n";
            // [[maybe_unused]] pipeline_threads_ptr_->check_set_seek_in_sync(); // QtCreator still complains as it still does not understand C++20/C++23 too well!
            pipeline_threads_ptr_->check_set_seek_in_sync();
        }
    }
}

void packet_reader_thread::flush_queues()
{
    if (pipeline_threads_ptr_) {
        pipeline_threads_ptr_->flush_queues();
    }
}

void packet_reader_thread::signal_flush_start()
{
    if (pipeline_threads_ptr_) {
        pipeline_threads_ptr_->signal_flush_start();
    }
}

void packet_reader_thread::signal_flush_done()
{
    if (pipeline_threads_ptr_) {
        pipeline_threads_ptr_->signal_flush_done();
    }
}




} // namespace cpaf::gui::video
