#include "subtitle_downloader_thread.h"

#include <cpaf_libs/video/io/subtitle_container.h>
#include <cpaf_libs/gui/video/player.h>

using namespace std::chrono;

namespace cpaf::gui::video {

subtitle_downloader_thread::subtitle_downloader_thread(
    player& owning_player)
    : player_(owning_player)
{

}

subtitle_downloader_thread::~subtitle_downloader_thread()
{

}

void subtitle_downloader_thread::start()
{
    thread_object_ = std::make_unique<std::thread>(&subtitle_downloader_thread::thread_function, this);
    thread_object_->detach();
}

/// @todo Implement progress
float subtitle_downloader_thread::progress() const
{

    return 0;
}


void subtitle_downloader_thread::thread_function()
{
    while(thread_is_running_) {
        std::cerr << "FIXMENM subtitle reader thread ....\n";
        std::this_thread::sleep_for(thread_yield_time_);
    }
    std::cerr << "\n!!! EXIT subtitle_reader_thread::audio_samples_thread_fn() !!!\n";
}



} // namespace cpaf::gui::video
