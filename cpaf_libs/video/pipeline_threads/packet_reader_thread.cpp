#include "packet_reader_thread.h"
#include <cpaf_libs/video/av_format_context.h>

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
            const auto FIXMENM_audio_queue_size_before = format_context().packet_queue_const(media_type::audio).size();
            const auto FIXMENM_video_queue_size_before = format_context().packet_queue_const(media_type::video).size();
            format_context().read_packets_to_queues(mt, primary_queue_fill_level_);
            const auto FIXMENM_audio_queue_size_after = format_context().packet_queue_const(media_type::audio).size();
            const auto FIXMENM_video_queue_size_after = format_context().packet_queue_const(media_type::video).size();
            const auto FIXMENM_audio_queue_size_diff = FIXMENM_audio_queue_size_after - FIXMENM_audio_queue_size_before;
            const auto FIXMENM_video_queue_size_diff = FIXMENM_video_queue_size_after - FIXMENM_video_queue_size_before;
            if (FIXMENM_audio_queue_size_diff != 0 || FIXMENM_video_queue_size_diff != 0) {
//                std::cerr << "FIXMENM audio_queue_size_diff: " << FIXMENM_audio_queue_size_diff
//                          << ", video_queue_size_diff: " << FIXMENM_video_queue_size_diff
//                          << "\n";
            }

        }
        std::this_thread::sleep_for(read_packets_yield_time_);
    }
    std::cerr << "\n!!! EXIT packet_reader_thread::read_packets_thread_fn() !!!\n";
}

void packet_reader_thread::check_seek_position()
{
    if (seek_requested_) {
        std::cerr << "--------- packet_reader_thread::seek_requested_ ------\n";
        const auto types_to_read = format_context().set_of_each_media_type();
        const pipeline_index_t flush_to_index = flush_to_index_requested_index_;
        format_context().pipeline_index_set(flush_to_index);
        std::cerr << "FIXMENM seek_requested;  flush_to_index: '" << flush_to_index << "'\n";
        format_context().seek_time_pos(media_type::video, seek_position_requested_, seek_direction_);
//        format_context().seek_time_pos(media_type::audio, seek_position_requested_, seek_direction_);
        bool all_packets_read = format_context().read_each_type_to_queues(types_to_read);
//        all_packets_read = all_packets_read && format_context().read_each_type_to_queues(types_to_read);
//        all_packets_read = all_packets_read && format_context().read_each_type_to_queues(types_to_read);
//        all_packets_read = all_packets_read && format_context().read_each_type_to_queues(types_to_read);
//        all_packets_read = all_packets_read && format_context().read_each_type_to_queues(types_to_read);
//        all_packets_read = all_packets_read && format_context().read_each_type_to_queues(types_to_read);
//        all_packets_read = all_packets_read && format_context().read_each_type_to_queues(types_to_read);
//        all_packets_read = all_packets_read && format_context().read_each_type_to_queues(types_to_read);
        format_context().pipeline_control_set(pipeline_control_t::normal_flow);
        seek_requested_ = false;


        if (!all_packets_read) {
            std::cerr << "WARNING Could mot read all desired packet types from stream SEEK_POS warning!";
        }
    }
}


} // namespace cpaf::video
