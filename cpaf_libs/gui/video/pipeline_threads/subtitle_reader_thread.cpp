#include "subtitle_reader_thread.h"

#include <cpaf_libs/video/media_stream_time.h>
#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/video/av_codec_context.h>
#include <cpaf_libs/video/io/subtitle_container.h>
#include <cpaf_libs/gui/video/player.h>

using namespace std::chrono;

namespace cpaf::gui::video {

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
                std::cerr << "TODO subtitle reader thread subtitle_source_t::text_file support!\n";
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
                std::cerr << "ERROR: Can't push subtitle to queue!\n";
            }
        }
    }
}


} // namespace cpaf::gui::video
