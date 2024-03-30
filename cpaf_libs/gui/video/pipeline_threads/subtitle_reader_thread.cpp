#include "subtitle_reader_thread.h"
#include <fmt/format.h>
#include <cpaf_libs/video/media_stream_time.h>
#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/video/av_codec_context.h>
#include <cpaf_libs/video/io/subtitle_container.h>
#include <cpaf_libs/gui/video/player.h>

using namespace std::chrono;

namespace cpaf::gui::video {

static cpaf::video::subtitle_frame  create_from_container_frame(const subtitle_container::frame_t& frame_in)
{
    cpaf::video::subtitle_frame frame_out;
    frame_out.lines                   = frame_in.lines;
    frame_out.presentation_time       = frame_in.presentation_time;
    frame_out.presentation_time_end   = frame_in.presentation_time_end;
    frame_out.sequence_number         = frame_in.sequence_number;

    return frame_out;
}

subtitle_reader_thread::subtitle_reader_thread(player& owning_player,
                                               cpaf::video::subtitles_queue& subtitles_queue,
                                               const std::atomic_bool& threads_running,
                                               const std::atomic_bool& threads_paused,
                                               std::atomic<seek_state_t>& seek_state)
    : subtitle_container_mutex_{},
      player_(owning_player),
      subtitles_queue_(subtitles_queue),
      threads_running_(threads_running),
      threads_paused_(threads_paused),
      seek_state_(seek_state)
{

}

subtitle_reader_thread::~subtitle_reader_thread()
{

}

void subtitle_reader_thread::start()
{
    thread_object_ = std::make_unique<std::thread>(&subtitle_reader_thread::thread_function, this);
    thread_object_->detach();
}

void subtitle_reader_thread::subtitle_container_set(std::unique_ptr<subtitle_container> container)
{
    std::lock_guard<std::mutex> lg{ subtitle_container_mutex_ };
    subtitle_container_ = std::move(container);
    if (subtitle_container_) {
        current_subtitle_iter_ = subtitle_container_->end();
    }
}

void subtitle_reader_thread::flush()
{

}

void subtitle_reader_thread::flush_start()
{

}

void subtitle_reader_thread::flush_done()
{
    std::lock_guard<std::mutex> lg{ subtitle_container_mutex_ };
    if (!subtitle_container_) { return; }
    current_subtitle_iter_ = subtitle_container_->end();
}

void subtitle_reader_thread::thread_function()
{
    while(threads_running_) {
        if (threads_paused_) {
            thread_is_paused_ = true;
        }
        else
        {
            thread_is_paused_ = false;
            if (player_.subtitle_source() == subtitle_source_t::stream) {
                read_from_stream();
            }
            else if (player_.subtitle_source() == subtitle_source_t::text_file) {
                read_from_container();
            }
        }
        std::this_thread::sleep_for(thread_yield_time_);
    }
    std::cerr << "LOG_INFO: EXIT subtitle_reader_thread::thread_function()!!!\n";
}

void subtitle_reader_thread::read_from_stream()
{
    if (player_.has_subtitle_stream()) {
        auto subtitles = player_.subtitle_codec_context().read_subtitles();
        for (auto& sub: subtitles) {
            if (!subtitles_queue_.push(std::move(sub)) ) {
                std::cerr << "LOG_ERR: Can't push subtitle to queue!\n";
            }
        }
    }
}

void subtitle_reader_thread::read_from_container()
{
    if (seek_state_ != seek_state_t::ready) {
        return;
    }

    std::lock_guard<std::mutex> lg{ subtitle_container_mutex_ };
    if (!subtitle_container_) {
        return;
    }

    const auto container_end = subtitle_container_->end();
    set_cur_subtitle_iter();

    int32_t subtitles_to_read = subtitles_read_ahead_size - subtitles_queue_.size();
    if (subtitles_to_read <= 0) { return ; }
    // if (subtitles_to_read > 0) std::cerr << "FIXMENM subtitles_to_read: " << subtitles_to_read << "\n";

//    if (static_cast<int32_t>(subtitles_queue_.size()) < subtitles_to_read) {
//        std::cerr << "FIXMENM subtitles_queue_.size(): " << subtitles_queue_.size()
//                  << " seek_state: " << to_string(seek_state_)
//                  << "\n";
//    }

    while ((current_subtitle_iter_ < container_end) &&  subtitles_to_read > 0 ) {
        enqueue_current_subtitle();
        ++current_subtitle_iter_;
        --subtitles_to_read;
    }

}

// Assumes mutex is already locked and subtitle_container_ and current iterator valid
void subtitle_reader_thread::enqueue_current_subtitle()
{
//    fmt::println("--- FIXMENM enqueue_current_subtitle() -----\n{}\n", current_subtitle_iter_->to_string()); std::cout << std::endl;

    auto sub = create_from_container_frame(*current_subtitle_iter_);
    if (!subtitles_queue_.push(std::move(sub)) ) {
        std::cerr << "LOG_ERR: Can't push container subtitle to queue!\n";
    }
}

// Assumes mutex is already locked and subtitle_container_ valid
void subtitle_reader_thread::set_cur_subtitle_iter()
{
    current_subtitle_iter_ = subtitle_container_->find_last_before(player_.current_time());
    // fmt::println("****FIXMENM subtitle_reader_thread::set_cur_subtitle_iter -----\n{}\n", current_subtitle_iter_->to_string()); std::cout << std::endl;
}

} // namespace cpaf::gui::video
