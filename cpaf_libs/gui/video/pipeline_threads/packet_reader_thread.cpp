#include "packet_reader_thread.h"
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
    const std::atomic_bool& threads_paused,
    std::atomic<seek_state_t>& seek_state)
    : player_(owning_player)
    , threads_running_(threads_running)
    , threads_paused_(threads_paused)
    , seek_state_(seek_state)
{
}

void packet_reader_thread::start()
{
    read_packets_thread_  = std::make_unique<std::thread>(&packet_reader_thread::read_packets_thread_fn, this);
    read_packets_thread_->detach();
}

bool packet_reader_thread::seek_position(const std::chrono::microseconds& stream_pos, cpaf::video::seek_dir dir)
{
    auto seek_state = seek_state_t::ready;
    if (seek_state_.compare_exchange_strong(seek_state, seek_state_t::requested)) {
        seek_from_position_ = current_media_time().current_time_pos();
        seek_position_requested_ = stream_pos;
        seek_direction_ = dir;
        return true;
    }
    return false;
}

bool packet_reader_thread::seek_position(const std::chrono::microseconds& stream_pos)
{
    auto seek_state = seek_state_t::ready;
    if (seek_state_.compare_exchange_strong(seek_state, seek_state_t::requested)) {

        seek_from_position_ = current_media_time().current_time_pos();
        seek_position_requested_ = stream_pos;
        seek_direction_ = cpaf::video::seek_dir::forward;
        return true;
    }
    return false;
}

void packet_reader_thread::read_packets_thread_fn()
{
    const auto mt = format_context().primary_media_type();
    while(threads_running()) {
        check_seek_position();
        if (!threads_paused_) {
            format_context().read_packets_to_queues(mt, primary_queue_fill_level_);
        }
        std::this_thread::sleep_for(read_packets_yield_time_);
    }
    std::cerr << "\n!!! EXIT packet_reader_thread::read_packets_thread_fn() !!!\n";
}

void packet_reader_thread::check_seek_position()
{
    auto seek_state = seek_state_t::requested;
    if (seek_state_.compare_exchange_strong(seek_state, seek_state_t::flushing)) {
        const auto mt = format_context().primary_media_type();
        signal_flush_start();
        format_context().read_packets_to_queues(mt, primary_queue_fill_level_ + 1);
        format_context().seek_time_pos(mt, seek_position_requested_, seek_direction_);
        flush_queues();
        format_context().read_packets_to_queues(mt, primary_queue_fill_level_);
        signal_flush_done();
        seek_state_ = seek_state_t::flush_done;
        std::this_thread::sleep_for(5ms);
        seek_state_ = seek_state_t::ready;
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
