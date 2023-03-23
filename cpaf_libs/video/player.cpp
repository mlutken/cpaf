#include "player.h"

namespace cpaf::video {

player::player()
{
    primary_source_stream_ = std::make_unique<play_stream>();
    current_media_time_set(cur_media_time_);
}

void player::start()
{
    pipeline_threads_temp_only().start();
}

void player::terminate()
{
    threads_running_ = false;
    pipeline_threads_temp_only().terminate();
}

/// @todo need to support reading "hybrid" resources that (potentially) specify different streams
///       for video, audio, key_frames etc.. For now we simply open a single primary stream with this.
bool player::open(const std::string& resource_path)
{
    return open_primary_stream(resource_path);
}

bool player::open(const std::string& resource_path, stream_type_t sti)
{
    return open_stream(resource_path, sti);
}

bool player::has_source_stream(stream_type_t sti) const
{
    return source_stream(sti) == nullptr;
}

// TODO: A little convoluted this implementation
const play_stream* player::source_stream(stream_type_t sti) const
{
    const auto index = to_size_t(sti);
    if (source_streams_[index]) {
        return source_streams_[index].get();
    }
    if (primary_source_stream_) {
        if (sti == stream_type_t::video && primary_source_stream_->has_media_type(media_type::video)) {
            return primary_source_stream_.get();
        }
        else if (sti == stream_type_t::audio && primary_source_stream_->has_media_type(media_type::audio)) {
            return primary_source_stream_.get();
        }
        else if (sti == stream_type_t::subtitle && primary_source_stream_->has_media_type(media_type::subtitle)) {
            return primary_source_stream_.get();
        }
    }
    return nullptr;
}

// TODO: A little convoluted this implementation
play_stream* player::source_stream(stream_type_t sti)
{
    const auto index = to_size_t(sti);
    if (source_streams_[index]) {
        return source_streams_[index].get();
    }
    if (primary_source_stream_) {
        if (sti == stream_type_t::video && primary_source_stream_->has_media_type(media_type::video)) {
            return primary_source_stream_.get();
        }
        else if (sti == stream_type_t::audio && primary_source_stream_->has_media_type(media_type::audio)) {
            return primary_source_stream_.get();
        }
        else if (sti == stream_type_t::subtitle && primary_source_stream_->has_media_type(media_type::subtitle)) {
            return primary_source_stream_.get();
        }
    }
    return nullptr;
}

// -----------------------------------------------------
// --------- TEMP/REFACTOR: pipeline_threads related ---
// -----------------------------------------------------
void player::format_context_set(av_format_context* ctx)
{
    pipeline_threads_temp_only().format_context_set(ctx);
}

void player::format_context_set(av_format_context& ctx)
{
    pipeline_threads_temp_only().format_context_set(ctx);
}

void player::video_codec_ctx_set(av_codec_context* ctx)
{
    pipeline_threads_temp_only().video_codec_ctx_set(ctx);
}

void player::video_codec_ctx_set(av_codec_context& ctx)
{
    pipeline_threads_temp_only().video_codec_ctx_set(ctx);
}

void player::audio_codec_ctx_set(av_codec_context* ctx)
{
    pipeline_threads_temp_only().audio_codec_ctx_set(ctx);
}

void player::audio_codec_ctx_set(av_codec_context& ctx)
{
    pipeline_threads_temp_only().audio_codec_ctx_set(ctx);
}

void player::audio_resampler_set(audio_resampler* resampler)
{
    pipeline_threads_temp_only().audio_resampler_set(resampler);
}

void player::audio_resampler_set(audio_resampler& resampler)
{
    pipeline_threads_temp_only().audio_resampler_set(resampler);
}

void player::audio_samples_queue_set(av_samples_queue* queue)
{
    pipeline_threads_temp_only().audio_samples_queue_set(queue);
}

void player::audio_samples_queue_set(av_samples_queue& queue)
{
    pipeline_threads_temp_only().audio_samples_queue_set(queue);
}

// ---------------------------------------------
// --- Interfacing to surrounding app/system ---
// ---------------------------------------------

player::audio_play_callback_t player::audio_callback_get()
{
    return pipeline_threads_temp_only().audio_callback_get();
}

bool player::video_frame_update(av_frame& current_frame, render& video_render)
{
    return pipeline_threads_temp_only().video_frame_update(current_frame, video_render);
}

// --------------------
// --- Play control ---
// --------------------

void player::seek_position(const std::chrono::microseconds& stream_pos, seek_dir dir)
{
    pipeline_threads_temp_only().seek_position(stream_pos, dir);
}

void player::seek_position(const std::chrono::microseconds& stream_pos)
{
    pipeline_threads_temp_only().seek_position(stream_pos);
}

void player::seek_relative(const std::chrono::microseconds& delta_time)
{
    pipeline_threads_temp_only().seek_relative(delta_time);
}

void player::pause_playback()
{
    cur_media_time_.pause_time();
    threads_paused_ = true;
    pipeline_threads_temp_only().pause_playback();
}

void player::resume_playback()
{
    cur_media_time_.resume_time();
    threads_paused_ = false;
    pipeline_threads_temp_only().resume_playback();
}

// ---------------------------------
// --- PRIVATE: Helper functions ---
// ---------------------------------

bool player::open_stream(const std::string& resource_path, stream_type_t sti)
{
    const auto index = to_size_t(sti);
    source_streams_[index] = std::make_unique<play_stream>();
    const auto open_ok = source_streams_[index]->open(resource_path);

    return open_ok;
}

bool player::open_primary_stream(const std::string& resource_path)
{
    primary_resource_path_ = resource_path;
    const auto open_ok = primary_source_stream_->open(resource_path);
    return open_ok;
}

void player::current_media_time_set(media_stream_time* mts)
{
    pipeline_threads_temp_only().current_media_time_set(mts);
}

void player::current_media_time_set(media_stream_time& mts)
{
    pipeline_threads_temp_only().current_media_time_set(mts);
}


} //END namespace cpaf::video

