
#include "pipeline_threads.h"
#include <iostream>

extern "C"
{
}

#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/video/av_frame.h>
#include <cpaf_libs/video/audio_resampler.h>
#include <cpaf_libs/video/media_stream_time.h>

using namespace std;
namespace cpaf::video {


pipeline_threads::pipeline_threads()
    : audio_render_thread_()
    , audio_resampler_thread_(threads_running_, threads_paused_)
    , packet_reader_thread_(threads_running_, threads_paused_)
    , video_render_thread_(threads_running_, threads_paused_)
{
    packet_reader_thread_.pipeline_threads_set(this);
}

void pipeline_threads::format_context_set(av_format_context* ctx)
{
    audio_render_thread_.format_context_set(ctx);
    audio_resampler_thread_.format_context_set(ctx);
    packet_reader_thread_.format_context_set(ctx);
    video_render_thread_.format_context_set(ctx);
    format_context_ptr_ = ctx;
}

void pipeline_threads::format_context_set(av_format_context& ctx)
{
    audio_render_thread_.format_context_set(ctx);
    audio_resampler_thread_.format_context_set(ctx);
    packet_reader_thread_.format_context_set(ctx);
    video_render_thread_.format_context_set(ctx);
    format_context_ptr_ = &ctx;
}

void pipeline_threads::video_codec_ctx_set(av_codec_context* ctx)
{
    video_render_thread_.video_codec_ctx_set(ctx);
    video_codec_ctx_ptr_ = ctx;
}

void pipeline_threads::video_codec_ctx_set(av_codec_context& ctx)
{
    video_render_thread_.video_codec_ctx_set(ctx);
    video_codec_ctx_ptr_ = &ctx;
}

void pipeline_threads::audio_codec_ctx_set(av_codec_context* ctx)
{
    audio_resampler_thread_.audio_codec_ctx_set(ctx);
    audio_codec_ctx_ptr_ = ctx;
}

void pipeline_threads::audio_codec_ctx_set(av_codec_context& ctx)
{
    audio_resampler_thread_.audio_codec_ctx_set(ctx);
    audio_codec_ctx_ptr_ = &ctx;
}

void pipeline_threads::audio_resampler_set(audio_resampler* resampler)
{
    audio_resampler_thread_.audio_resampler_set(resampler);
    audio_resampler_ptr_ = resampler;
}

void pipeline_threads::audio_resampler_set(audio_resampler& resampler)
{
    audio_resampler_thread_.audio_resampler_set(resampler);
    audio_resampler_ptr_ = &resampler;
}

void pipeline_threads::audio_samples_queue_set(av_samples_queue_t* queue)
{
    audio_resampler_thread_.audio_samples_queue_set(queue);
    audio_render_thread_.audio_samples_queue_set(queue);
    video_render_thread_.audio_samples_queue_set(queue);
    audio_samples_queue_ptr_ = queue;
}

void pipeline_threads::audio_samples_queue_set(av_samples_queue_t& queue)
{
    audio_resampler_thread_.audio_samples_queue_set(queue);
    audio_render_thread_.audio_samples_queue_set(queue);
    video_render_thread_.audio_samples_queue_set(queue);
    audio_samples_queue_ptr_ = &queue;
}

void pipeline_threads::current_media_time_set(media_stream_time* mts)
{
    audio_resampler_thread_.current_media_time_set(mts);
    audio_render_thread_.current_media_time_set(mts);
    video_render_thread_.current_media_time_set(mts);
    current_media_time_ptr_ = mts;
}

void pipeline_threads::current_media_time_set(media_stream_time& mts)
{
    audio_resampler_thread_.current_media_time_set(mts);
    audio_render_thread_.current_media_time_set(mts);
    video_render_thread_.current_media_time_set(mts);
    current_media_time_ptr_ = &mts;
}

pipeline_threads::play_callback_t pipeline_threads::audio_callback_get()
{
    return audio_render_thread_.audio_callback_get();
}


void pipeline_threads::start()
{
    audio_render_thread_.start();
    audio_resampler_thread_.start();
    packet_reader_thread_.start();
    video_render_thread_.start();
}

void pipeline_threads::terminate()
{
    threads_running_ = false;
}

void pipeline_threads::seek_position(const std::chrono::microseconds& stream_pos, seek_dir dir)
{
    std::cerr << "FIXMENM pipeline_threads::seek_position(dir): " << std::chrono::duration_cast<std::chrono::milliseconds>(stream_pos).count() << " ms\n";
    // TODO:
    // 1) Set flushing requested for all threads (except the packet reader)
    // 2) Instruct packet reader to seek to new position, while marking the first packets AFTER the seek with
    //    pipeline_control_t::first_after_flush
    // 3) [Happens in the other threads] They will keep flushing/popping from their queues until they see
    //    a frame/packet marked with pipeline_control_t::first_after_flush
    //    Also for threads producing packets or frames into queues they can drop incoming except that they
    //    have to resume putting in their producer queue when they see pipeline_control_t::first_after_flush
    //    As soon as the threads have seen the pipeline_control_t::first_after_flush marker they can resume
    //    normal state.

//    std::cerr << "FIXMENM BEFORE FLUSH:\n" << format_context().queues_info();
//    std::cerr << "FIXMENM BEFORE FLUSH samples queue size: " << audio_samples_queue().size() << "\n";
//    flush_queues();
//    std::cerr << "FIXMENM AFTER FLUSH:\n" << format_context().queues_info();
//    std::cerr << "FIXMENM AFTER FLUSH samples queue size: " << audio_samples_queue().size() << "\n";
    packet_reader_thread_.seek_position(stream_pos, dir);

//    const pipeline_index_t flush_to_idx = packet_reader_thread_.seek_position(stream_pos, dir);
//    flush_to_index(flush_to_idx);
}

void pipeline_threads::seek_position(const std::chrono::microseconds& stream_pos)
{
    std::cerr << "FIXMENM pipeline_threads::seek_position: " << std::chrono::duration_cast<std::chrono::milliseconds>(stream_pos).count() << " ms\n";
    // TODO:
    // 1) Set flushing requested for all threads (except the packet reader)
    // 2) Instruct packet reader to seek to new position, while marking the first packets AFTER the seek with
    //    pipeline_control_t::first_after_flush
    // 3) [Happens in the other threads] They will keep flushing/popping from their queues until they see
    //    a frame/packet marked with pipeline_control_t::first_after_flush
    //    Also for threads producing packets or frames into queues they can drop incoming except that they
    //    have to resume putting in their producer queue when they see pipeline_control_t::first_after_flush
    //    As soon as the threads have seen the pipeline_control_t::first_after_flush marker they can resume
    //    normal state.

//    flush_queues();
    packet_reader_thread_.seek_position(stream_pos);
//    const pipeline_index_t flush_to_idx = packet_reader_thread_.seek_position(stream_pos);
//    flush_to_index(flush_to_idx);

//    seek_position_requested_ = stream_pos;
    //    seek_requested_ = true;
}

void pipeline_threads::seek_relative(const std::chrono::microseconds& delta_time)
{
    auto new_pos = current_media_time().current_time_pos() + delta_time;
    if (new_pos <= 0ms) {
        new_pos = 0ms;
    }
    const seek_dir dir = delta_time.count() < 0 ? seek_dir::backward : seek_dir::forward;
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

void pipeline_threads::flush_to_index(const pipeline_index_t& pipeline_index)
{
    audio_render_thread_.flush_to_index(pipeline_index);
    video_render_thread_.flush_to_index(pipeline_index);
}

void pipeline_threads::flush_queues()
{
    format_context().flush_packet_queues();
    audio_samples_queue().flush();
}

void pipeline_threads::signal_flush_start()
{
    video_render_thread_.video_queue_flush_start();
    audio_render_thread_.audio_queue_flush_start();
    audio_resampler_thread_.samples_queue_flush_start();
}

void pipeline_threads::signal_flush_done()
{
    video_render_thread_.video_queue_flush_done();
    audio_render_thread_.audio_queue_flush_done();
    audio_resampler_thread_.samples_queue_flush_done();
}

bool pipeline_threads::video_frame_update(av_frame& current_frame, render& video_render)
{
    return video_render_thread_.video_frame_update(current_frame, video_render);
}

} //END namespace cpaf::video
