#include "video_render_thread.h"

#include <time/cpaf_time.h>
#include <video/av_format_context.h>
#include <video/av_codec_context.h>
#include <video/media_stream_time.h>
#include <video/render/render.h>
#include <cpaf_libs/video/av_samples_queue.h>

using namespace std::chrono_literals;
using namespace std::chrono;


namespace cpaf::video {

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

bool video_render_thread::video_frame_update (av_frame& current_frame, render& video_render)
{
    bool ret_val = video_frame_do_render(current_frame, video_render);
    debug_video_frame_update(current_frame, video_render);
    return ret_val;
}

bool video_render_thread::video_frame_do_render(av_frame& current_frame, render& video_render)
{
    if (video_queue_flush_in_progress_) {
        return false;
    }

    if (!current_media_time().time_is_paused()) {
        const auto cur_video_time = current_media_time().video_time_pos();
        auto time_dist_to_cur_frame = current_frame.presentation_time() - cur_video_time;

        if (video_queue_flushed_) {
            std::cerr << "******* FIXMENM video_queue_flushed_ ************\n";
            video_queue_flushed_ = false;
            current_frame = video_codec_ctx().read_frame();
            time_dist_to_cur_frame = current_frame.presentation_time() - cur_video_time;
        }


        if (time_dist_to_cur_frame > 1s ) {
            std::cerr << "******* FIXMENM long time to current video frame " << duration_cast<seconds>(time_dist_to_cur_frame).count() << "s\n";
            current_frame = video_codec_ctx().read_frame();
            time_dist_to_cur_frame = current_frame.presentation_time() - cur_video_time;
        }

        if (time_dist_to_cur_frame <= 1ms ) {
            current_pipeline_index_ = current_frame.pipeline_index();
            video_render.render_video_frame(current_frame);
            current_frame = video_codec_ctx().read_frame();
            return true;
        }

    }
    return false;
}

void video_render_thread::debug_video_frame_update(av_frame& current_frame, render& /*video_render*/)
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

av_packet video_render_thread::video_packet_queue_front()
{
    return format_context().packet_queue_front(media_type::video);
}

void video_render_thread::video_packet_queue_pop()
{
    format_context().packet_queue_pop(media_type::video);
}

const packet_queue_t& video_render_thread::video_packet_queue() const
{
    return format_context().packet_queue(media_type::video);
}

const packet_queue_t& video_render_thread::video_packet_queue_const() const
{
    return format_context().packet_queue(media_type::video);
}

} // namespace cpaf::video
