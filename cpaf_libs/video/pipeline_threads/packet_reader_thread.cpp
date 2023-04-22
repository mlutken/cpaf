#include "packet_reader_thread.h"
#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/video/pipeline_threads/pipeline_threads.h>

namespace cpaf::video {

packet_reader_thread::packet_reader_thread(const std::atomic_bool& threads_running, const std::atomic_bool& threads_paused)
    : threads_running_(threads_running)
    , threads_paused_(threads_paused)
{
}

void packet_reader_thread::start()
{
    read_packets_thread_  = std::make_unique<std::thread>(&packet_reader_thread::read_packets_thread_fn, this);
    read_packets_thread_->detach();
}

pipeline_index_t packet_reader_thread::seek_position(const std::chrono::microseconds& stream_pos, seek_dir dir)
{
    flush_to_index_requested_index_ = format_context().pipeline_index() + 1;
    seek_position_requested_ = stream_pos;
    seek_requested_ = true;
    seek_direction_ = dir;
    return flush_to_index_requested_index_;
}

pipeline_index_t packet_reader_thread::seek_position(const std::chrono::microseconds& stream_pos)
{
    flush_to_index_requested_index_ = format_context().pipeline_index() + 1;
    seek_position_requested_ = stream_pos;
    seek_requested_ = true;
    seek_direction_ = seek_dir::forward;
    return flush_to_index_requested_index_;
}

void packet_reader_thread::read_packets_thread_fn()
{
    const auto mt = format_context().primary_media_type();
    while(threads_running()) {
        format_context().pipeline_control_set(pipeline_control_t::normal_flow);
        if (!threads_paused()) {
            check_seek_position();
            format_context().read_packets_to_queues(mt, primary_queue_fill_level_);
        }
        std::this_thread::sleep_for(read_packets_yield_time_);
    }
    std::cerr << "\n!!! EXIT packet_reader_thread::read_packets_thread_fn() !!!\n";
}

void packet_reader_thread::check_seek_position()
{
    if (seek_requested_) {
        seek_requested_ = false;
        std::cerr << "--------- packet_reader_thread::seek_requested_ ------\n";

        const auto mt = format_context().primary_media_type();
        signal_flush_start();
        format_context().read_packets_to_queues(mt, primary_queue_fill_level_ + 1);
        format_context().seek_time_pos(mt, seek_position_requested_, seek_direction_);
        flush_queues();
        format_context().read_packets_to_queues(mt, primary_queue_fill_level_);
        signal_flush_done();
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


} // namespace cpaf::video
