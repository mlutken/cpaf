#include "audio_render_thread.h"

#include <iostream>
#include <cpaf_libs/video/media_stream_time.h>
#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/video/av_samples_queue.h>


using namespace std;

namespace cpaf::video {

void audio_render_thread::start()
{

}

bool audio_render_thread::flush_to_index(const pipeline_index_t& pipeline_index)
{
    return flush_queue_.push(pipeline_index);
}

bool audio_render_thread::state_matches(pipeline_state_t desired_state, const pipeline_index_t& desired_index) const
{
    return desired_state == pipeline_state_ && desired_index == current_pipeline_index_;
}

audio_render_thread::audio_play_callback_t audio_render_thread::audio_callback_get()
{
    return [this](uint8_t* stream, int32_t length) {
        this->audio_callback_function(stream, length);
    };
}

void audio_render_thread::audio_callback_function(uint8_t* stream, int32_t length)
{
//    switch_state();
    switch (pipeline_state_) {
    case pipeline_state_t::normal_flow:
        state__normal_flow(stream, length);
        break;
    case pipeline_state_t::flush_in_progress:
        state__flush_in_progress(stream, length);
        break;
    default:
        // TODO: How many states do we really need here! For now we treat remaining states as the normal_flow case!!
        state__normal_flow(stream, length);
        break;
    }
    debug_audio_callback(stream, length);
}

pipeline_index_t audio_render_thread::get_flush_to_index()
{
    pipeline_index_t flush_index = 0;
    while (!flush_queue_.empty()) {
        flush_index = flush_queue_.front();
        flush_queue_.pop();
    }
    return flush_index;
}

void audio_render_thread::switch_state()
{
    switch (pipeline_state_) {
    case pipeline_state_t::normal_flow:
        flush_in_progress_index_ = get_flush_to_index();
        if (flush_in_progress_index_ != 0) {
            pipeline_state_ = pipeline_state_t::flush_in_progress;
        }
        break;
    case pipeline_state_t::flush_in_progress:
        if (flush_in_progress_index_ == 0) {
            pipeline_state_ = pipeline_state_t::normal_flow;
        }
        break;
    default:
        // Don't change state!!
        break;
    }
}

void audio_render_thread::state__normal_flow(uint8_t* stream, int32_t length)
{

    if (current_media_time().time_is_paused()) {
        render_audio_silence(stream, length);
        return;
    }

    if (audio_samples_queue().empty()) {
        render_audio_silence(stream, length);
        if (!empty_audio_buffer_dbg_) {

//            const auto& video_queue = format_context().packet_queue_const(media_type::video);

//            cerr << "*** [" << audio_callback_dbg_counter_ << "] ERROR audio samples queue EMPTY!!"
//                 << ", audio_packets_queue().size(): " << format_context().packet_queue_const(media_type::audio).size()
//                 << ", video_packets_queue().size(): " << video_queue.size()
//                 << ", XX audio_samples_queue().size(): " << audio_samples_queue().size()
//                 << ", video_packets_queue() ts: " << (video_queue.size() > 0 ? video_queue.front().presentation_time_ms().count() : 0)
//                 << "\n";
            }
        empty_audio_buffer_dbg_ = true;
        return;
    }


    empty_audio_buffer_dbg_ = false;
    prev_pipeline_index_ = audio_samples_queue().front().pipeline_index();
    current_pipeline_index_ = audio_samples_queue().front().pipeline_index();
    current_media_time().adjust_time(audio_samples_queue().front().presentation_time());

    // Check adjust time on audio samples queue pop()
    auto queue_pop_callback = [this](const av_samples_buffer& /*samples_buf*/) {
        current_pipeline_index_ = audio_samples_queue().front().pipeline_index();
        //            std::cerr << " Audio current time: " << audio_samples_queue().front().presentation_time_ms().count() << " ms\n";
//        if (audio_samples_queue().front().pipeline_control() == pipeline_control_t::seek_pos) {
//            current_media_time().adjust_time(audio_samples_queue().front().presentation_time());
//            std::cerr << "ADJUSTING current_media_time() TO audio sample time!! => "
//                      << " current media time: " << current_media_time().current_time_pos_ms().count() << " ms\n";
//        }
    };

    const auto bytes_copied = audio_samples_queue().copy_audio_samples(
                stream,
                length,
                current_media_time().current_time_pos(),
                sync_ok_interval,
                queue_pop_callback);
    //    const auto bytes_copied = copy_audio_samples(stream, audio_samples_queue(), length);
    if (bytes_copied != length) {
        std::cerr << "ERROR: Not enough samples in queue for audio device callback!!\n";
    }
}

void audio_render_thread::state__flush_in_progress(uint8_t* stream, int32_t length)
{
    std::cerr << "FIXMENM AUDIO state__flush_in_progress audio samples queue size: '" <<  audio_samples_queue().size() << "'\n";
    render_audio_silence(stream, length);
    //    std::cerr << "FIXMENM state__flush_in_progress: '" << flush_in_progress_index_ << "'\n";
    //    if (!audio_samples_queue().empty()) {
    //        return;
    //    }
    // TODO: Implement flushing of queue until desired index is reached!
    bool done = false;
    while (!done) {
        if (audio_samples_queue().empty()) {
            //    std::cerr << "FIXMENM state__flush_in_progress: '" << flush_in_progress_index_ << "'\n";
            break;
        }
        const auto front_pipeline_index = audio_samples_queue().front().pipeline_index();
        if (front_pipeline_index >= flush_in_progress_index_) {
            std::cerr << "FIXMENM AUDIO DONE FLUSHING!!! = 0; flushed to index:" << flush_in_progress_index_
                      << ", audio time: " << audio_samples_queue().front().presentation_time_ms().count() << " ms"
                      << ", audio samples queue size: " << audio_samples_queue().size()
                      << "\n";
            current_pipeline_index_ = front_pipeline_index;
            flush_in_progress_index_ = 0;
            done = true;
        }
        else {
            //            std::cerr << "FIXMENM audio_samples_queue().pop()\n";
            audio_samples_queue().pop();
        }
    }
}

void audio_render_thread::render_audio_silence(uint8_t* stream, int32_t length)
{
    memset(stream, 0, static_cast<size_t>(length));
}


void audio_render_thread::debug_audio_callback(uint8_t* /*stream*/, int32_t /*length*/)
{
    if ((audio_callback_dbg_counter_ % 50000 == 0) ||
//            (pipeline_state_ != pipeline_state_t::normal_flow) ||
//            (audio_samples_queue().front().pipeline_index() != prev_pipeline_index_) ||
//            (audio_samples_queue().front().pipeline_control() == pipeline_control_t::seek_pos) ||
            (  0 < audio_samples_queue().size() && audio_samples_queue().size() < 5  )) {
        std::cerr
                << "AUDIO(" << audio_callback_dbg_counter_ << ") [" << (audio_samples_queue().front().presentation_time_ms() - current_media_time().current_time_pos_ms()).count() << " ms]"
                << " state: '" << to_string(pipeline_state_) << "' "
                << " pi: '" << audio_samples_queue().front().pipeline_index() << "' "
                << " => '" << to_string(audio_samples_queue().front().pipeline_control()) << "'"
                << " current media time: " << current_media_time().current_time_pos_ms().count() << " ms"
                << ", audio time: " << audio_samples_queue().front().presentation_time_ms().count() << " ms"
                << ", flush_in_progress_index_: " << flush_in_progress_index_ << ""
                   //                << ", audio pkts buf: " << format_context().packet_queue(media_type::audio).size() << ""
                   //                << ", samples buf: " << audio_samples_queue().size() << ""
                   //                << ", sample remaining: " << audio_samples_queue().front().duration_ms().count() << " ms"
                   //                << ", sample size: " << audio_samples_queue().front().size() << " bytes"
                << "\n";
    }
    audio_callback_dbg_counter_++;
}

} // namespace cpaf::video
