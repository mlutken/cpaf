#include "video_render_thread.h"

#include <time/cpaf_time.h>
#include <video/av_format_context.h>
#include <video/av_codec_context.h>
#include <video/media_stream_time.h>
#include <cpaf_libs/video/subtitle_frame.h>
#include <cpaf_libs/video/av_samples_queue.h>
#include <cpaf_libs/gui/video/render/render.h>
#include <cpaf_libs/gui/video/player.h>

using namespace std::chrono_literals;
using namespace std::chrono;
using namespace cpaf::video;
using namespace cpaf;


namespace cpaf::gui::video {

video_render_thread::video_render_thread(
    player& owning_player,
    cpaf::video::av_samples_queue& audio_samples_queue,
    cpaf::video::subtitles_queue& subtitles_queue,
    const std::atomic_bool& threads_running,
    const std::atomic_bool& threads_paused,
    std::atomic<seek_state_t>& seek_state)
    : player_(owning_player)
    , audio_samples_queue_(audio_samples_queue)
    , subtitles_queue_(subtitles_queue)
    , threads_running_(threads_running)
    , threads_paused_(threads_paused)
    , seek_state_(seek_state)
{

}

void video_render_thread::start()
{

}

void video_render_thread::terminate()
{

}

void video_render_thread::video_frame_update(cpaf::video::av_frame& current_frame, cpaf::gui::video::render& video_render)
{
    [[maybe_unused]] bool new_frame_was_read = video_frame_do_render(current_frame, video_render);

    debug_video_frame_update(current_frame, video_render);
}

void video_render_thread::video_queue_flush_start() {
    flush_requested_ = true;
    video_queue_flush_in_progress_ = true;
}

void video_render_thread::video_queue_flush_done() {
    video_queue_flush_in_progress_ = false;
    video_queue_flushed_ = true;
}

std::chrono::microseconds video_render_thread::time_to_current_frame(cpaf::video::av_frame& current_frame) const
{
    return current_frame.presentation_time() - player_.cur_media_time().video_time_pos();
}

std::chrono::microseconds video_render_thread::time_to_current_frame_abs(cpaf::video::av_frame& current_frame) const
{
    return cpaf::time::abs(time_to_current_frame(current_frame));
}

///bool video_render_thread::is_seek_currently_possible() const {
///    if (video_queue_flush_in_progress_ || video_queue_flushed_) {
///        return false;
///    }
///    return is_seek_currently_possible_;
///}

/**
@todo Do we in case on non valid frame want to call video_render.clear_screen() ?
*/
bool video_render_thread::video_frame_do_render(
    cpaf::video::av_frame& current_frame,
    cpaf::gui::video::render& video_render)
{

    // --- Special case when we are flushing (seeking) ---
    if (seek_state_ == seek_state_t::flushing) {
        video_render.render_video_frame(current_frame);
        return false;
    }
    else if ( seek_state_ == seek_state_t::flush_done) {
        current_frame = player_.video_codec_context().read_frame();
        video_render.render_video_frame(current_frame);
        return true;
    }

    if (flush_requested_) {
        flush_queues();
    }

    update_current_subtitle(video_render);

    bool new_frame_was_read = false;
    if (time_to_current_frame(current_frame) > 1s ) {
        std::cerr << "******* ERROR long time to current video frame " << duration_cast<seconds>(time_to_current_frame(current_frame)) << "\n";
        current_frame = player_.video_codec_context().read_frame();
        new_frame_was_read = true;
    }

    video_render.render_video_frame(current_frame);

    if (!player_.cur_media_time().time_is_paused()) {
        if (time_to_current_frame(current_frame) <= 1ms ) {
            current_frame = player_.video_codec_context().read_frame();
            new_frame_was_read = true;
        }
    }

    return new_frame_was_read;
}

void video_render_thread::update_current_subtitle(render& video_render)
{
    const auto& cur_subtitle = video_render.current_subtitle();
    if (!subtitle_within_display_time(cur_subtitle)) {
        video_render.clear_current_subtitle();
    }

//    video_render.set_current_subtitle(test_subtitle());
//    return; // FIXMENM

    while (!subtitles_queue_.empty() && subtitle_too_old(subtitles_queue_.front())) {
        subtitles_queue_.pop();
    }
    if (subtitles_queue_.empty()) {
        return;
    }

//    if (!subtitles_queue_.empty()) {
//        std::cerr << "FIXMENM show current subtitle: " << subtitles_queue_.front().dbg_str()
//                  << " cur time: " << cpaf::time::format_h_m_s(player_.cur_media_time().video_time_pos())
//                  << "  wintin time: '" << subtitle_within_display_time(subtitles_queue_.front()) << "'"
//                  << "\n";
//    }

    if (subtitle_within_display_time(subtitles_queue_.front())) {
//        std::cerr << "FIXMENM SET current subtitle 1: " << subtitles_queue_.front().dbg_str()
//                  << " cur time: " << cpaf::time::format_h_m_s(player_.cur_media_time().video_time_pos())
//                  << "  within time: '" << subtitle_within_display_time(subtitles_queue_.front()) << "'"
//                  << "\n";

        video_render.set_current_subtitle(std::move(subtitles_queue_.front()));
        subtitles_queue_.pop();
        std::cerr << "FIXMENM SET current subtitle 2: " << video_render.current_subtitle().dbg_str()
                  << " cur time: " << cpaf::time::format_h_m_s(player_.cur_media_time().video_time_pos())
                  << "  within time: '" << subtitle_within_display_time(video_render.current_subtitle()) << "'"
                  << "\n";

    }
}


cpaf::video::subtitle_frame video_render_thread::test_subtitle() const
{
    // FIXMENM
    subtitle_frame sf("ëËïÏÿŸæÆäÄüÜøØöÖåÅ -> ß", "on the same team together in Nam.");
//    subtitle_frame sf("My name is John Rambo. We served", "on the same team together in Nam.");
//    subtitle_frame sf("My name is John Rambo. We served");
    return sf;
}

bool video_render_thread::subtitle_within_display_time(const cpaf::video::subtitle_frame& subtitle) const
{
    if (!subtitle.is_valid()) {
        return false;
    }
    const auto cur_time = player_.cur_media_time().video_time_pos();
    const auto should_display = subtitle.presentation_time <= cur_time && cur_time <= subtitle.presentation_time_end;
    return should_display;
}

bool video_render_thread::subtitle_too_old(const cpaf::video::subtitle_frame& subtitle) const
{
    if (!subtitle.is_valid()) {
        return true;
    }
    const auto cur_time = player_.cur_media_time().video_time_pos();
    return subtitle.presentation_time_end < cur_time;
}

void video_render_thread::debug_video_frame_update(cpaf::video::av_frame& current_frame, gui::video::render& /*video_render*/)
{
    if (
       (true && video_frame_update_dbg_counter_ % 2000 == 0)
    )
    {
        const auto video_to_media_ms = (current_frame.presentation_time_ms() - player_.cur_media_time().current_time_pos_ms()).count();
        const auto video_to_audio_ms = (current_frame.presentation_time_ms() - audio_samples_queue().front().presentation_time_ms()).count();

        const auto cur_video_time = player_.cur_media_time().video_time_pos();
        const auto cur_frame_time = current_frame.presentation_time();

        auto time_dist_to_cur_frame_ms = duration_cast<milliseconds>(cur_frame_time - cur_video_time);

        std::cerr
                << "XXX VIDEO [" << video_to_media_ms << "/" << video_to_audio_ms << " ms]"
                << " current media time: " << player_.cur_media_time().current_time_pos_ms().count() << " ms"
                << ", audio frm t: " << audio_samples_queue().front().presentation_time_ms().count() << " ms"
                << ", video frm t: " << current_frame.presentation_time_ms().count() << " ms"
                << ", time_dist_to_cur_frame_ms: " << time_dist_to_cur_frame_ms.count() << " ms"
                << "\n";
    }
    ++video_frame_update_dbg_counter_;
}

cpaf::video::av_packet video_render_thread::video_packet_queue_front()
{
    return player_.format_context().packet_queue_front(cpaf::video::media_type::video);
}

void video_render_thread::video_packet_queue_pop()
{
    player_.format_context().packet_queue_pop(cpaf::video::media_type::video);
}

const cpaf::video::packet_queue_t& video_render_thread::video_packet_queue() const
{
    return player_.format_context().packet_queue(cpaf::video::media_type::video);
}

const cpaf::video::packet_queue_t& video_render_thread::video_packet_queue_const() const
{
    return player_.format_context().packet_queue(cpaf::video::media_type::video);
}

/**
 *  @note. Here only flush the subtitles queue. The vide frames are flushed from pipeline_threads
 */
void video_render_thread::flush_queues()
{
    subtitles_queue_.flush();
    flush_requested_ = false;
}

} // namespace cpaf::gui::video
