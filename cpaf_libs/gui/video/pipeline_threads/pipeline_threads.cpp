
#include "pipeline_threads.h"
#include <iostream>

#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/video/av_frame.h>
#include <cpaf_libs/video/audio_resampler.h>
#include <cpaf_libs/video/media_stream_time.h>
#include <cpaf_libs/video/av_samples_queue.h>
#include <cpaf_libs/gui/video/player.h>

using namespace std;
using namespace cpaf::video;
using namespace std::chrono;
using namespace std::chrono_literals;

namespace cpaf::gui::video {


//pipeline_threads::pipeline_threads(atomic_bool& /*threads_running*/, atomic_bool& /*threads_paused*/)
pipeline_threads::pipeline_threads(player& owning_player, av_samples_queue& /* audio_samples_queue REMOVE*/)
    : player_(owning_player)
    , audio_samples_queue_(1000)
//    , subtitles_queue_(subtitles_queue)
    , packet_reader_thread_(owning_player, threads_running_, threads_paused_, seek_state_)
    , audio_resampler_thread_(owning_player, audio_samples_queue_, threads_running_, threads_paused_)
    , audio_render_thread_(owning_player, audio_samples_queue_, *this, seek_state_)
    , subtitle_reader_thread_(owning_player, threads_running_, threads_paused_)
    , video_render_thread_(owning_player, threads_running_, threads_paused_, seek_state_)
{
    packet_reader_thread_.pipeline_threads_set(this);
}

pipeline_threads::~pipeline_threads()
{
    terminate();
}

void pipeline_threads::audio_resampler_set(cpaf::video::audio_resampler* resampler)
{
    audio_resampler_thread_.audio_resampler_set(resampler);
    audio_resampler_ptr_ = resampler;
}

void pipeline_threads::audio_resampler_set(cpaf::video::audio_resampler& resampler)
{
    audio_resampler_thread_.audio_resampler_set(resampler);
    audio_resampler_ptr_ = &resampler;
}

void pipeline_threads::audio_samples_queue_set(cpaf::video::av_samples_queue& queue)
{
    audio_resampler_thread_.audio_samples_queue_set(queue);
    audio_render_thread_.audio_samples_queue_set(queue);
    video_render_thread_.audio_samples_queue_set(queue);
    audio_samples_queue_ptr_ = &queue;
}

pipeline_threads::audio_play_callback_t pipeline_threads::audio_callback_get()
{
    return audio_render_thread_.audio_callback_get();
}


void pipeline_threads::start()
{
    audio_render_thread_.start();
    audio_resampler_thread_.start();
    packet_reader_thread_.start();
    subtitle_reader_thread_.start();
    video_render_thread_.start();
}

void pipeline_threads::terminate()
{
    threads_running_ = false;
}

void pipeline_threads::seek_position(const std::chrono::microseconds& stream_pos, cpaf::video::seek_dir dir)
{
    packet_reader_thread_.seek_position(stream_pos, dir);
}

void pipeline_threads::seek_position(const std::chrono::microseconds& stream_pos)
{
    packet_reader_thread_.seek_position(stream_pos);
}

void pipeline_threads::seek_relative(const std::chrono::microseconds& delta_time)
{
    auto new_pos = player_.cur_media_time().current_time_pos() + delta_time;
    if (new_pos <= 0ms) {
        new_pos = 0ms;
    }
    const auto dir = delta_time.count() < 0 ? cpaf::video::seek_dir::backward : cpaf::video::seek_dir::forward;
    seek_position(new_pos, dir);
}

void pipeline_threads::pause_playback()
{
    threads_paused_ = true;
}

void pipeline_threads::resume_playback()
{
    threads_paused_ = false;
}

void pipeline_threads::video_frame_update(cpaf::video::av_frame& current_frame, gui::video::render& video_render)
{
//    if (seek_state_ != seek_state_t::ready) {
//        cerr << "FIXMENM seek_state: " << to_string(seek_state_) << "\n";
//    }

    video_render_thread_.video_frame_update(current_frame, video_render);
}

void pipeline_threads::flush_queues()
{
    player_.format_context().flush_packet_queues();
    audio_samples_queue().flush();
}

void pipeline_threads::signal_flush_start()
{
    video_render_thread_.video_queue_flush_start();
    audio_resampler_thread_.samples_queue_flush_start();
}

void pipeline_threads::signal_flush_done()
{
    video_render_thread_.video_queue_flush_done();
    audio_resampler_thread_.samples_queue_flush_done();
}


} //END namespace cpaf::gui::video
