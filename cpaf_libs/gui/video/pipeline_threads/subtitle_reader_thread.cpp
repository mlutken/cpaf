#include "subtitle_reader_thread.h"

#include <cpaf_libs/video/media_stream_time.h>
#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/video/av_codec_context.h>
#include <cpaf_libs/gui/video/player.h>

using namespace std::chrono;

namespace cpaf::gui::video {

subtitle_reader_thread::subtitle_reader_thread(
    player& owning_player,
    cpaf::video::subtitles_queue& subtitles_queue,
    const std::atomic_bool& threads_running,
    const std::atomic_bool& threads_paused)
    : player_(owning_player)
    , subtitles_queue_(subtitles_queue)
    , threads_running_(threads_running)
    , threads_paused_(threads_paused)
{

}

void subtitle_reader_thread::start()
{
    thread_object_ = std::make_unique<std::thread>(&subtitle_reader_thread::thread_function, this);
    thread_object_->detach();
}

void subtitle_reader_thread::thread_function()
{
    while(threads_running()) {
        if (!threads_paused()) {
//            std::cerr << "FIXMENM subtitle reader thread ....\n";
        }
        std::this_thread::sleep_for(thread_yield_time_);
    }
    std::cerr << "\n!!! EXIT subtitle_reader_thread::audio_samples_thread_fn() !!!\n";
}


} // namespace cpaf::gui::video
