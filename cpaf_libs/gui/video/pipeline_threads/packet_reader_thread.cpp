#include "packet_reader_thread.h"
#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/gui/video/pipeline_threads/pipeline_threads.h>

using namespace std;
using namespace cpaf::video;
using namespace std::chrono;

namespace cpaf::gui::video {

packet_reader_thread::packet_reader_thread(
    const std::atomic_bool& threads_running,
    const std::atomic_bool& threads_paused,
    std::atomic<seek_state_t>& seek_state)
    : threads_running_(threads_running)
    , threads_paused_(threads_paused)
    , seek_state_(seek_state)
{
}

void packet_reader_thread::start()
{
    read_packets_thread_  = std::make_unique<std::thread>(&packet_reader_thread::read_packets_thread_fn, this);
    read_packets_thread_->detach();
}

cpaf::video::pipeline_index_t packet_reader_thread::seek_position(const std::chrono::microseconds& stream_pos, cpaf::video::seek_dir dir)
{
    if (seek_requested_) {
        return format_context().pipeline_index();
    }

    auto seek_state = seek_state_t::ready;
    seek_state_.compare_exchange_strong(seek_state, seek_state_t::requested);

    flush_to_index_requested_index_ = format_context().pipeline_index() + 1;
    seek_position_requested_ = stream_pos;
    seek_requested_ = true;
    seek_direction_ = dir;
    return flush_to_index_requested_index_;
}

cpaf::video::pipeline_index_t packet_reader_thread::seek_position(const std::chrono::microseconds& stream_pos)
{
//    last_seek_start_time_ = steady_clock::now();
    if (seek_requested_) {
        return format_context().pipeline_index();
    }

    auto seek_state = seek_state_t::ready;
    seek_state_.compare_exchange_strong(seek_state, seek_state_t::requested);

    flush_to_index_requested_index_ = format_context().pipeline_index() + 1;
    seek_position_requested_ = stream_pos;
    seek_requested_ = true;

    seek_direction_ = cpaf::video::seek_dir::forward;
    return flush_to_index_requested_index_;
}

void packet_reader_thread::read_packets_thread_fn()
{
    const auto mt = format_context().primary_media_type();
    while(threads_running()) {
        format_context().pipeline_control_set(cpaf::video::pipeline_control_t::normal_flow);
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
//    auto diff = steady_clock::now() - last_seek_start_time_;
//    if ( diff < seek_throttle_time_) {
//        std::cerr << "FIXMENM check_seek_position() THROTTLE " << duration_cast<milliseconds>(diff) << "\n";
//        return;
//    }

    auto seek_state = seek_state_t::requested;
    if (seek_state_.compare_exchange_strong(seek_state, seek_state_t::flushing)) {
        // TODO: Move the flushing to here

        seek_state_ = seek_state_t::flush_done;
    }


    if (seek_requested_) {
//        const auto seek_start = steady_clock::now();
//        std::cerr << "--------- packet_reader_thread::seek_requested_ ------\n";

        const auto mt = format_context().primary_media_type();
        signal_flush_start();
        format_context().read_packets_to_queues(mt, primary_queue_fill_level_ + 1);
        format_context().seek_time_pos(mt, seek_position_requested_, seek_direction_);
        flush_queues();
        format_context().read_packets_to_queues(mt, primary_queue_fill_level_);
        signal_flush_done();
//        const auto seek_end = steady_clock::now();
//        const auto seek_duration = seek_end - seek_start;
//        std::cerr << "FIXMENM packet_reader_thread seek complete: " << duration_cast<microseconds>(seek_duration) << "\n";
        seek_requested_ = false;
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
