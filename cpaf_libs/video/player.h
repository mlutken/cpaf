#pragma once

#include <array>
#include <memory>
#include <cpaf_libs/audio/cpaf_audio_device_base.h>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/play_stream.h>
#include <cpaf_libs/video/media_stream_time.h>

namespace cpaf::video {

class player
{
public:
    using audio_play_callback_t  = cpaf::audio::device_base::play_callback_t;

    // START: TEMPORARY REFACTOR functions ONLY!

    pipeline_threads&           pipeline_threads_temp_only  () { return primary_source_stream_->pipeline_threads_temp_only(); }

    // END  : TEMPORARY REFACTOR functions ONLY!


    player();
    void                        start                   ();
    void                        terminate               ();
    bool                        open                    (const std::string& resource_path);
    bool                        open                    (const std::string& resource_path, stream_type_t sti);

    bool                        has_source_stream       (stream_type_t sti) const;
    const play_stream*          source_stream           (stream_type_t sti) const;
    play_stream*                source_stream           (stream_type_t sti);

    play_stream&                primary_stream          () { return *primary_source_stream_; }
    const std::string&          primary_resource_path	() const { return primary_resource_path_; }

    media_stream_time&          cur_media_time          ()          { return cur_media_time_; }
    const media_stream_time&    cur_media_time          () const    { return cur_media_time_; }

    // -----------------------------------------------------
    // --------- TEMP/REFACTOR: pipeline_threads related ---
    // -----------------------------------------------------
    void                        format_context_set      (av_format_context* ctx);
    void                        format_context_set      (av_format_context& ctx);
    void                        video_codec_ctx_set     (av_codec_context* ctx);
    void                        video_codec_ctx_set     (av_codec_context& ctx);
    void                        audio_codec_ctx_set     (av_codec_context* ctx);
    void                        audio_codec_ctx_set     (av_codec_context& ctx);
    void                        audio_resampler_set     (audio_resampler* resampler);
    void                        audio_resampler_set     (audio_resampler& resampler);
    void                        audio_samples_queue_set (av_samples_queue* queue);
    void                        audio_samples_queue_set (av_samples_queue& queue);

    // ---------------------------------------------
    // --- Interfacing to surrounding app/system ---
    // ---------------------------------------------
    audio_play_callback_t       audio_callback_get      ();
    bool                        video_frame_update      (av_frame& current_frame, render& video_render);


    // --------------------
    // --- Play control ---
    // --------------------
    void                        seek_position           (const std::chrono::microseconds& stream_pos, seek_dir dir);
    void                        seek_position           (const std::chrono::microseconds& stream_pos);
    void                        seek_relative           (const std::chrono::microseconds& delta_time);
    void                        pause_playback          ();
    void                        resume_playback         ();
    void                        toggle_pause_playback   ();
    bool                        playback_paused         () const { return threads_paused_; }


private:
    // ---------------------------------
    // --- PRIVATE: Helper functions ---
    // ---------------------------------
    bool                        open_stream             (const std::string& resource_path, stream_type_t sti);
    bool                        open_primary_stream     (const std::string& resource_path);
    void                        current_media_time_set  (media_stream_time* mts);
    void                        current_media_time_set  (media_stream_time& mts);


    // ----------------------------
    // --- PRIVATE: Member vars ---
    // ----------------------------
//    std::array<play_stream*, stream_type_index_size()>                  source_stream_ptrs_ = {nullptr, nullptr, nullptr, nullptr, nullptr};
    std::array<std::unique_ptr<play_stream>, stream_type_index_size()>  source_streams_ = {nullptr, nullptr, nullptr, nullptr, nullptr};
    std::unique_ptr<play_stream>                                        primary_source_stream_;
    media_stream_time                                                   cur_media_time_;
    std::atomic_bool                                                    threads_running_  = true;
    std::atomic_bool                                                    threads_paused_   = false;

//    video_render_thread                                                 video_render_thread_;
    std::string                                                         primary_resource_path_;

};

} //END namespace cpaf::video

