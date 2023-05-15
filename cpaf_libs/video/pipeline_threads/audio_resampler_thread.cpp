#include "audio_resampler_thread.h"

#include <cpaf_libs/video/media_stream_time.h>
#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/video/av_codec_context.h>
#include <cpaf_libs/video/audio_resampler.h>
#include <cpaf_libs/video/av_samples_queue.h>

using namespace std::chrono;

namespace cpaf::video {

audio_resampler_thread::audio_resampler_thread(const std::atomic_bool& threads_running, const std::atomic_bool& threads_paused)
    : threads_running_(threads_running)
    , threads_paused_(threads_paused)
{

}

void audio_resampler_thread::start()
{
    thread_object_ = std::make_unique<std::thread>(&audio_resampler_thread::thread_function, this);
    thread_object_->detach();
}

void audio_resampler_thread::thread_function()
{
    while(threads_running()) {
        if (!threads_paused()) {
            const auto cur_media_time_pos = current_media_time().current_time_pos();
            bool add_samples = true;
            while (add_samples) {
                resample_frame(add_samples, cur_media_time_pos);
            }
        }
        std::this_thread::sleep_for(audio_samples_yield_time_);
    }
    std::cerr << "\n!!! EXIT audio_resampler_thread::audio_samples_thread_fn() !!!\n";
}

void audio_resampler_thread::resample_frame(bool& add_samples, const std::chrono::microseconds& cur_media_time_pos)
{
    add_samples = false;

    if (audio_samples_queue().full() || format_context().packet_queue_const(media_type::audio).empty()) {
        return;
    }


    const auto frame = audio_codec_ctx().read_frame();
    if(!frame.is_valid()) {
        return;
    }


    add_samples = true;
    const auto read_ahead_time = frame.presentation_time() - cur_media_time_pos;
    if (read_ahead_time >= audio_samples_read_ahead_time_ && audio_samples_queue().size() >= audio_samples_fill_level_) {
        add_samples = false;
    }

    av_samples_buffer samples_buf = audio_sampler().audio_resampling(frame);

    if (!audio_samples_queue().push(std::move(samples_buf))) {
        std::cerr << "ERROR: Exeeeded audio_samples_queue size\n";
        add_samples = false;
    }

}

} // namespace cpaf::video
