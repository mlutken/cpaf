#include "player.h"

#include <cpaf_libs/torrent/torrents.h>
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/video/render/render.h>

using namespace cpaf::video;

namespace cpaf::gui::video {

player::player()
    : primary_source_stream_([this]() {return torrents_get();}),
      audio_samples_queue_(1000)
{
    current_media_time_set(cur_media_time_);
}

player::~player()
{
}

void player::init_video(const system_window& main_window)
{
    video_render_ = render::create_video_render(main_window, video_dst_dimensions());
    video_render_->video_codec_ctx_set(video_codec_context());
    video_render_->render_geometry_set(render_geometry_t(main_window.get_size()));
////    video_render_->render_geometry_set(render_geometry_t({100,100}, main_window.get_size())); // TEST ONLY!
}

void player::start(const std::chrono::microseconds& start_time_pos)
{
    // source_stream(stream_type_t::video)
    auto&  video_fmt_ctx = primary_stream().format_context(); // TODO: If we use multiple streams we need to get the right format ctx per stream here!
    auto&  audio_fmt_ctx = primary_stream().format_context(); // TODO: If we use multiple streams we need to get the right format ctx per stream here!
    auto&  subtitle_fmt_ctx = primary_stream().format_context(); // TODO: If we use multiple streams we need to get the right format ctx per stream here!
    video_fmt_ctx.seek_time_pos(start_time_pos);
    audio_fmt_ctx.seek_time_pos(start_time_pos);
    subtitle_fmt_ctx.seek_time_pos(start_time_pos);
    audio_resampler_.in_formats_set(audio_codec_context());

    audio_resampler_.init();
    video_codec_context().get_packet_function_set(video_fmt_ctx.get_packet_function(media_type::video));
    audio_codec_context().get_packet_function_set(video_fmt_ctx.get_packet_function(media_type::audio));

    media_pipeline_threads().audio_codec_ctx_set(audio_codec_context());
    media_pipeline_threads().audio_resampler_set(audio_resampler_);
    media_pipeline_threads().audio_samples_queue_set(audio_samples_queue_);

    video_fmt_ctx.read_packets_to_queues(video_fmt_ctx.primary_media_type(), 10);
    media_pipeline_threads().format_context_set(video_fmt_ctx);
    media_pipeline_threads().video_codec_ctx_set(video_codec_context());

    media_pipeline_threads().start();
}

void player::terminate()
{
    threads_running_ = false;
    // Reset codec contexts
    video_codec_ctx_ = av_codec_context{};
    audio_codec_ctx_ = av_codec_context{};
    media_pipeline_threads().terminate();
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
    return source_stream(sti) != nullptr;
}

// TODO: A little convoluted this implementation
const play_stream* player::source_stream(stream_type_t sti) const
{
    const auto index = to_size_t(sti);
    if (source_streams_[index]) {
        return source_streams_[index].get();
    }
    if (sti == stream_type_t::video && primary_source_stream_.has_media_type(media_type::video)) {
        return &primary_source_stream_;
    }
    else if (sti == stream_type_t::audio && primary_source_stream_.has_media_type(media_type::audio)) {
        return &primary_source_stream_;
    }
    else if (sti == stream_type_t::subtitle && primary_source_stream_.has_media_type(media_type::subtitle)) {
        return &primary_source_stream_;
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
    if (sti == stream_type_t::video && primary_source_stream_.has_media_type(media_type::video)) {
        return &primary_source_stream_;
    }
    else if (sti == stream_type_t::audio && primary_source_stream_.has_media_type(media_type::audio)) {
        return &primary_source_stream_;
    }
    else if (sti == stream_type_t::subtitle && primary_source_stream_.has_media_type(media_type::subtitle)) {
        return &primary_source_stream_;
    }
    return nullptr;
}

// ---------------------------
// --- Video setup/control ---
// ---------------------------
void player::video_dimensions_set(int32_t width, int32_t height)
{
    video_dimensions_set({width, height});
}

void player::video_dimensions_set(const surface_dimensions_t& dimensions)
{
    if (video_render_) {
        video_render_->render_geometry_set(render_geometry_t(dimensions));
    }
//    video_dst_dimensions_requested_ = dimensions; // TODO: This is currently not working as intended!
//    update_scaling_context(); // TODO: This is currently not working as intended!
}

void player::video_scaler_flags_set(int32_t flags)
{
    video_scaler_flags_ = flags;
    update_scaling_context();
}

void player::video_scaler_align_set(int32_t align)
{
    video_scaler_align_ = align;
    update_scaling_context();
}

void player::ff_dst_pixel_format_set(AVPixelFormat pixel_format)
{
    ff_dst_pixel_format_ = pixel_format;
}

av_codec_context& player::video_codec_context() const
{
    if (!video_codec_ctx_.is_valid()) {
        auto* video_stream = source_stream(stream_type_t::video);
        if (video_stream){
            video_codec_ctx_ = video_stream->codec_context(video_stream_index());
            update_scaling_context();
        }
    }
    return video_codec_ctx_;
}


// ----------------------------
// --- Video info functions ---
// ----------------------------

surface_dimensions_t player::video_src_dimensions() const
{
    return video_codec_context().dimensions();
}

surface_dimensions_t player::video_dst_dimensions() const
{
    if (video_codec_ctx_.is_valid()) {
        return video_codec_ctx_.dst_dimensions();
    }
    else if (video_dst_dimensions_requested_.x() <= 0 && video_dst_dimensions_requested_.y() <= 0 ) {
        return video_src_dimensions();
    }
    return video_dst_dimensions_requested_;
}

size_t player::video_stream_index() const
{
    return video_stream_index_ != no_stream_index ? video_stream_index_ : source_stream(stream_type_t::video)->first_video_index();
}

// ---------------------------
// --- Audio setup/control ---
// ---------------------------

bool player::audio_out_formats_set(const audio::device& audio_device)
{
    return audio_resampler_.out_formats_set(audio_device);
}

bool player::audio_out_formats_set(const ff_audio_format_t& ff_audio_format)
{
    return audio_resampler_.out_formats_set(ff_audio_format);
}

av_codec_context& player::audio_codec_context() const
{
    if (!audio_codec_ctx_.is_valid()) {
        auto* audio_stream = source_stream(stream_type_t::audio);
        if (audio_stream){
            audio_codec_ctx_ = audio_stream->codec_context(audio_stream_index());
            //TODO: Should we call update_resampler_context() or similar named function?;
        }
    }
    return audio_codec_ctx_;
}

// ----------------------------
// --- Audio info functions ---
// ----------------------------

size_t player::audio_stream_index() const
{
    return audio_stream_index_ != no_stream_index ? audio_stream_index_ : source_stream(stream_type_t::audio)->first_audio_index();
}

// ---------------------------------------------
// --- Interfacing to surrounding app/system ---
// ---------------------------------------------

player::audio_play_callback_t player::audio_callback_get()
{
    return media_pipeline_threads().audio_callback_get();
}

bool player::video_frame_update(av_frame& current_frame, cpaf::gui::video::render& video_render)
{
    return media_pipeline_threads().video_frame_update(current_frame, video_render);
}

bool player::video_frame_update(av_frame& current_frame)
{
    return video_frame_update(current_frame, *video_render_);
}

std::shared_ptr<torrent::torrents> player::torrents_get() const
{
    if (!torrents_) {
        torrents_ = std::make_shared<torrent::torrents>();
        torrents_->start();
    }
    return torrents_;
}

// --------------------
// --- Play control ---
// --------------------

void player::seek_position(const std::chrono::microseconds& stream_pos, seek_dir dir)
{
    media_pipeline_threads().seek_position(stream_pos, dir);
}

void player::seek_position(const std::chrono::microseconds& stream_pos)
{
    media_pipeline_threads().seek_position(stream_pos);
}

void player::seek_relative(const std::chrono::microseconds& delta_time)
{
    media_pipeline_threads().seek_relative(delta_time);
}

void player::pause_playback()
{
    cur_media_time_.pause_time();
    threads_paused_ = true;
    media_pipeline_threads().pause_playback();
}

void player::resume_playback()
{
    cur_media_time_.resume_time();
    threads_paused_ = false;
    media_pipeline_threads().resume_playback();
}

void player::toggle_pause_playback()
{
    if (playback_paused()) {
        resume_playback();
    }
    else {
        pause_playback();
    }
}

// -----------------------
// --- Debug functions ---
// -----------------------
std::string player::queues_info() const
{
    return primary_stream().format_context().queues_info();
}


// ---------------------------------
// --- PRIVATE: Helper functions ---
// ---------------------------------

bool player::open_stream(const std::string& resource_path, stream_type_t sti)
{
    const auto index = to_size_t(sti);
    source_streams_[index] = std::make_unique<play_stream>([this]() {return torrents_get();});
    const auto open_ok = source_streams_[index]->open(resource_path);

    return open_ok;
}

bool player::open_primary_stream(const std::string& resource_path)
{
    primary_resource_path_ = resource_path;
    const auto open_ok = primary_source_stream_.open(resource_path);
    return open_ok;
}

void player::current_media_time_set(media_stream_time* mts)
{
    media_pipeline_threads().current_media_time_set(mts);
}

void player::current_media_time_set(media_stream_time& mts)
{
    media_pipeline_threads().current_media_time_set(mts);
}

void player::update_scaling_context() const
{
    if (video_codec_ctx_.is_valid()) {
        video_codec_ctx_.init_scaling_context(ff_dst_pixel_format_, video_dst_dimensions_requested_, video_scaler_flags_, video_scaler_align_);
    }
}


} //END namespace cpaf::gui::video

