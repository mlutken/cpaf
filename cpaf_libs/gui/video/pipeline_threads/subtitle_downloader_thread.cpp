#include "subtitle_downloader_thread.h"
#include <fmt/format.h>
#include <cpaf_libs/video/io/subtitle_container.h>
#include <cpaf_libs/gui/video/player.h>

using namespace std::chrono;

namespace cpaf::gui::video {

subtitle_downloader_thread::subtitle_downloader_thread(
    player& owning_player)
    : player_(owning_player)
    , queue_in_(20)
    , queue_out_(20)
{
}

subtitle_downloader_thread::~subtitle_downloader_thread()
{
    if (thread_object_) {
        thread_is_running_ = false;
        thread_object_->join();
    }
}

void subtitle_downloader_thread::start()
{
    thread_object_ = std::make_unique<std::thread>(&subtitle_downloader_thread::thread_function, this);
    // thread_object_->detach();
}

void subtitle_downloader_thread::enqueue_subtitle(const std::string& subtitle_path, const std::string& language_code)
{
    fmt::println("FIXMENM subtitle_downloader_thread::enqueue_subtitle {} , {}", subtitle_path, language_code);
    queue_in_.push({subtitle_path, language_code});
}

std::unique_ptr<subtitle_container> subtitle_downloader_thread::dequeue_subtitle()
{
    if (!queue_out_.empty()) {
        auto container = std::move(queue_out_.front());
        queue_out_.pop();
        return container;
    }
    return std::unique_ptr<subtitle_container>();
}

bool subtitle_downloader_thread::has_subtitle() const
{
    return !queue_out_.empty();
}

/// @todo Implement progress
float subtitle_downloader_thread::download_progress() const
{
    return subtitle_text_file_data_.curl().progress();
}


void subtitle_downloader_thread::thread_function()
{
    while(thread_is_running_) {
        // std::cerr << "FIXMENM subtitle reader thread ....\n";
        if (!queue_in_.empty()) {
            const job_t job = queue_in_.front();
            queue_in_.pop();
            download_subtitle(job.subtitle_path, job.language_code);
        }
        std::this_thread::sleep_for(thread_yield_time_);
    }
    std::cerr << "\n!!! EXIT subtitle_reader_thread::audio_samples_thread_fn() !!!\n";
}

void subtitle_downloader_thread::download_subtitle(const std::string& subtitle_path, const std::string& language_code)
{
    std::cerr << "FIXMENM: subtitle_downloader_thread::download_subtitle: " << subtitle_path << "\n";
    auto container = std::make_unique<subtitle_container>();
    if (subtitle_text_file_data_.open(subtitle_path, download_time_out_)) {
        container->parse_srt_file_data(subtitle_text_file_data_.srt_file_data());
        container->language_code_set(language_code);
        queue_out_.push(std::move(container));
    }
    else {
        std::cerr << "LOG_ERR: could not download subtitle: " << subtitle_path << "\n";
    }
}

} // namespace cpaf::gui::video
