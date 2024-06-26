#include "audio_render_thread.h"

#include <iostream>
#include <cpaf_libs/video/media_stream_time.h>
#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/video/av_samples_queue.h>
#include <cpaf_libs/gui/video/pipeline_threads/pipeline_threads.h>
#include <cpaf_libs/gui/video/player.h>


using namespace std;
using namespace std::chrono_literals;
using namespace std::chrono;
using namespace cpaf::video;
using namespace cpaf;

namespace cpaf::gui::video {

    audio_render_thread::audio_render_thread(player& owning_player,
                                             pipeline_threads& pline_threads,
                                             av_samples_queue& audio_samples_queue,
                                             const std::atomic_bool& threads_running,
                                             const std::atomic_bool& threads_started,
                                             const std::atomic_bool& threads_paused,
                                             std::atomic<cpaf::video::seek_state_t>& seek_state)
        : player_(owning_player),
          pipeline_threads_(pline_threads),
          audio_samples_queue_(audio_samples_queue),
          threads_running_(threads_running),
          threads_started_(threads_started),
          threads_paused_(threads_paused),
          seek_state_(seek_state)
{

}

void audio_render_thread::run()
{
}

void audio_render_thread::terminate()
{
}

audio_render_thread::audio_play_callback_t audio_render_thread::audio_callback_get()
{
    return [this](uint8_t* stream, int32_t length) {
        this->audio_callback_function(stream, length);
    };
}

std::chrono::microseconds audio_render_thread::front_presentation_time() const
{
    return audio_samples_queue_.front_presentation_time();
}

void audio_render_thread::audio_callback_function(uint8_t* stream, int32_t length)
{
    const bool audio_active = threads_running_ && threads_started_ && !threads_paused_;
    const bool is_flushing = seek_state_ == seek_state_t::flushing;
    const bool queue_is_empty = audio_samples_queue_.empty();
    const bool do_render_silence = !audio_active || is_flushing || queue_is_empty;

    if (do_render_silence) {
        render_audio_silence(stream, length);
        return;
    }

    player_.cur_media_time().adjust_time(audio_samples_queue_.front_presentation_time());
    pipeline_threads_.check_set_seek_in_sync();

    // Check adjust time on audio samples queue pop()
    auto queue_pop_callback = [](const cpaf::video::av_samples_buffer& /*samples_buf*/) {
        //        current_pipeline_index_ = audio_samples_queue_.front().pipeline_index();
    };

    const auto bytes_copied = audio_samples_queue_.copy_audio_samples(
        stream,
        length,
        player_.cur_media_time().current_time_pos(),
        sync_ok_interval_,
        queue_pop_callback);
    //    const auto bytes_copied = copy_audio_samples(stream, audio_samples_queue_, length);
    if (bytes_copied != length) {
        std::cerr << "ERROR: Not enough samples in queue for audio device callback!!\n";
    }

    //debug_audio_callback(stream, length);
}

void audio_render_thread::render_audio_silence(uint8_t* stream, int32_t length)
{
    memset(stream, 0, static_cast<size_t>(length));
}


void audio_render_thread::debug_audio_callback(uint8_t* /*stream*/, int32_t /*length*/)
{
    if ((audio_callback_dbg_counter_ % 50000 == 0) ||
//            (audio_samples_queue_.front().pipeline_index() != prev_pipeline_index_) ||
            (  0 < audio_samples_queue_.size() && audio_samples_queue_.size() < 5  )) {
        std::cerr
                << "AUDIO(" << audio_callback_dbg_counter_ << ") [" << (audio_samples_queue_.front().presentation_time_ms() - player_.cur_media_time().current_time_pos_ms()).count() << " ms]"
//                << " state: '" << to_string(pipeline_state_) << "' "
                << " current media time: " << player_.cur_media_time().current_time_pos_ms().count() << " ms"
                << ", audio time: " << audio_samples_queue_.front().presentation_time_ms().count() << " ms"
                   //                << ", audio pkts buf: " << player_.format_context().packet_queue(media_type::audio).size() << ""
                   //                << ", samples buf: " << audio_samples_queue_.size() << ""
                   //                << ", sample remaining: " << audio_samples_queue_.front().duration_ms().count() << " ms"
                   //                << ", sample size: " << audio_samples_queue_.front().size() << " bytes"
                << "\n";
    }
    audio_callback_dbg_counter_++;
}

} // namespace cpaf::gui::video
