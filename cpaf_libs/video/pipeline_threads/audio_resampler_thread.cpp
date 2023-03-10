#include "audio_resampler_thread.h"

#include <cpaf_libs/video/media_stream_time.h>
#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/video/av_codec_context.h>
#include <cpaf_libs/video/audio_resampler.h>
#include <cpaf_libs/video/av_samples_queue.h>

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
            bool add_samples = !format_context().packet_queue_const(media_type::audio).empty();
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
    if (format_context().packet_queue_const(media_type::audio).empty()) {
        add_samples = false;
        return;
    }


    //    DEBUG_seek_pos_added_index_ = -1;
    const auto frame = audio_codec_ctx().read_frame();

//    const std::string valid = frame.is_valid() ? "" : " INVALID! ";
//    std::cerr << "--- stream_threads::resample_frame()"
//              << valid
//              << ", video packet queue size: " << format_context().packet_queue(media_type::video).size()
//              << ", audio packet queue size: " << format_context().packet_queue(media_type::audio).size()
//              << ", audio_samples_queue().size(): " << audio_samples_queue().size()
//              << "\n";
    if(!frame.is_valid()) {
        //        std::cerr << "--- ERROR resample_frame() ---\n";
        add_samples = false;
        return;
    }


    av_samples_buffer samples_buf = audio_sampler().audio_resampling(frame);

    //    std::cerr << "--- FIXMENM resample_frame() ---\n";
    if (!audio_samples_queue().push(std::move(samples_buf))) {
        std::cerr << "ERROR: Exeeeded audio_samples_queue size\n";
        return;
    }

    const auto read_ahead_time = frame.presentation_time() - cur_media_time_pos;
    if (read_ahead_time >= audio_samples_read_ahead_time_ && audio_samples_queue().size() >= audio_samples_fill_level_) {
        //        std::cerr << "--- ADD_SAMPLES resample_frame() ---\n";
        add_samples = false;
    }
}

} // namespace cpaf::video
