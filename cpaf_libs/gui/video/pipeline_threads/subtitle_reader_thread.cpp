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


subtitle_reader_thread::subtitle_reader_thread(
    player& owning_player,
    cpaf::video::subtitles_queue& subtitles_queue,
    const std::atomic_bool& threads_running,
    const std::atomic_bool& threads_paused)
    : subtitle_container_mutex_{}
    , player_(owning_player)
    , subtitles_queue_(subtitles_queue)
    , threads_running_(threads_running)
    , threads_paused_(threads_paused)
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
    reset_subtitle_iterator();
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
                // std::cerr << "TODO subtitle reader thread subtitle_source_t::text_file support!\n";
                read_from_container();
            }

//            std::cerr << "FIXMENM subtitle reader thread ....\n";
        }
        std::this_thread::sleep_for(thread_yield_time_);
    }
    std::cerr << "\n!!! EXIT subtitle_reader_thread::audio_samples_thread_fn() !!!\n";
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
    std::lock_guard<std::mutex> lg{ subtitle_container_mutex_ };
    if (!subtitle_container_) {
        return;
    }

    if (subtitles_queue_.empty()) {
        push_from_container();
    }

}

// Assumes mutex is already locked and subtitle_container_ valid
void subtitle_reader_thread::push_from_container()
{

    const auto it = subtitle_container_->find_first_after(player_.current_time());
    if (it != subtitle_container_->end()) {
        fmt::println("--- FIXMENM subtitle_reader_thread::push_from_container -----\n{}\n", it->to_string()); std::cout << std::endl;

        auto sub = create_from_container_frame(*it);
        if (!subtitles_queue_.push(std::move(sub)) ) {
            std::cerr << "LOG_ERR: Can't push conainer subtitle to queue!\n";
        }

    }

}

void subtitle_reader_thread::reset_subtitle_iterator()
{
    std::lock_guard<std::mutex> lg{ subtitle_container_mutex_ };
    std::cerr << "FIXMENM reset_subtitle_iterator()\n";
    if (subtitle_container_) {
        current_subtitle_iter_ = subtitle_container_->end();
    }
}

// Assumes mutex is already locked and subtitle_container_ valid
void subtitle_reader_thread::inc_cur_subtitle_iter()
{
    if (!subtitle_container_) {
        return;
    }

    if (current_subtitle_iter_ >= subtitle_container_->end()) {
        current_subtitle_iter_ = subtitle_container_->find_first_after(player_.current_time());
        return;
    }
    ++current_subtitle_iter_;

}

} // namespace cpaf::gui::video
