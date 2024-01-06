#include "video_render_thread.h"

#include <time/cpaf_time.h>
#include <video/av_format_context.h>
#include <video/av_codec_context.h>
#include <video/media_stream_time.h>
#include <cpaf_libs/video/subtitle_frame.h>
#include <cpaf_libs/video/av_samples_queue.h>
#include <cpaf_libs/gui/video/render/render.h>

using namespace std::chrono_literals;
using namespace std::chrono;
using namespace cpaf::video;


namespace cpaf::gui::video {

video_render_thread::video_render_thread(const std::atomic_bool& threads_running, const std::atomic_bool& threads_paused)
    : threads_running_(threads_running)
    , threads_paused_(threads_paused)
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

std::chrono::microseconds video_render_thread::time_to_current_frame(cpaf::video::av_frame& current_frame) const
{
    return current_frame.presentation_time() - current_media_time().video_time_pos();
}

bool video_render_thread::is_seek_currently_possible() const {
    if (video_queue_flush_in_progress_ || video_queue_flushed_) {
        return false;
    }
    return is_seek_currently_possible_;
}

/**
@todo Do we in case on non valid frame want to call video_render.clear_screen() ?
*/
bool video_render_thread::video_frame_do_render(cpaf::video::av_frame& current_frame, cpaf::gui::video::render& video_render)
{
    bool new_frame_was_read = false;
    // --- Special case when we are flushing (seeking) ---
    if (video_queue_flush_in_progress_) {
        if (current_frame) {
            video_render.render_video_frame(current_frame);
        }
        video_render.render_subtitle(current_subtitle());
        return new_frame_was_read;
    }

    // --- Test if we need to read a new fram due to flush/seek complete ---
    if (video_queue_flushed_) {
        video_queue_flushed_ = false;
        current_frame = video_codec_ctx().read_frame();
        new_frame_was_read = true;
    }

    // --- Safety measure in case current frame (for whatever reason) is not valid ---
    if (!current_frame) {
        current_frame = video_codec_ctx().read_frame();
        new_frame_was_read = true;
        if (!current_frame) {
            std::cerr << "!!! ERROR video render NO frame!\n";
            video_render.clear_screen();
            return new_frame_was_read;
        }
    }

    if (time_to_current_frame(current_frame) > 1s ) {
       // std::cerr << "******* ERROR long time to current video frame " << duration_cast<seconds>(time_to_current_frame(current_frame)) << "\n";
        new_frame_was_read = true;
        current_frame = video_codec_ctx().read_frame();
    }
    if (!current_frame) {
        video_render.clear_screen();
        return new_frame_was_read;
    }

    video_render.render_video_frame(current_frame);
    video_render.render_subtitle(current_subtitle());

    if (!current_media_time().time_is_paused()) {
        if (time_to_current_frame(current_frame) <= 1ms ) {
            current_pipeline_index_ = current_frame.pipeline_index();
            current_frame = video_codec_ctx().read_frame();
            new_frame_was_read = true;
        }
    }

    return new_frame_was_read;
}

void video_render_thread::update_is_seek_possible(av_frame& current_frame)
{
    const auto abs_dist_to_cur_frame = cpaf::time::abs(time_to_current_frame(current_frame));
    is_seek_currently_possible_ = abs_dist_to_cur_frame < 40ms;
}

cpaf::video::subtitle_frame video_render_thread::current_subtitle() const
{
    // FIXMENM
    subtitle_frame sf("My name is John Rambo. We served", "on the same team together in Nam.");
//    subtitle_frame sf("My name is John Rambo. We served");
    return sf;
}

void video_render_thread::debug_video_frame_update(cpaf::video::av_frame& current_frame, gui::video::render& /*video_render*/)
{
    if (
       (true && video_frame_update_dbg_counter_ % 2000 == 0)
    )
    {
        const auto video_to_media_ms = (current_frame.presentation_time_ms() - current_media_time().current_time_pos_ms()).count();
        const auto video_to_audio_ms = (current_frame.presentation_time_ms() - audio_samples_queue().front().presentation_time_ms()).count();

        const auto cur_video_time = current_media_time().video_time_pos();
        const auto cur_frame_time = current_frame.presentation_time();

        auto time_dist_to_cur_frame_ms = duration_cast<milliseconds>(cur_frame_time - cur_video_time);

        std::cerr
                << "XXX VIDEO [" << video_to_media_ms << "/" << video_to_audio_ms << " ms]"
                << " pi: '" << current_frame.pipeline_index() << "' "
                << " pc: '" << to_string(current_frame.pipeline_control()) << "' "
                << " current media time: " << current_media_time().current_time_pos_ms().count() << " ms"
                << ", audio frm t: " << audio_samples_queue().front().presentation_time_ms().count() << " ms"
                << ", video frm t: " << current_frame.presentation_time_ms().count() << " ms"
                << ", time_dist_to_cur_frame_ms: " << time_dist_to_cur_frame_ms.count() << " ms"
                << "\n";
    }
    ++video_frame_update_dbg_counter_;
}

cpaf::video::av_packet video_render_thread::video_packet_queue_front()
{
    return format_context().packet_queue_front(cpaf::video::media_type::video);
}

void video_render_thread::video_packet_queue_pop()
{
    format_context().packet_queue_pop(cpaf::video::media_type::video);
}

const cpaf::video::packet_queue_t& video_render_thread::video_packet_queue() const
{
    return format_context().packet_queue(cpaf::video::media_type::video);
}

const cpaf::video::packet_queue_t& video_render_thread::video_packet_queue_const() const
{
    return format_context().packet_queue(cpaf::video::media_type::video);
}

} // namespace cpaf::gui::video
