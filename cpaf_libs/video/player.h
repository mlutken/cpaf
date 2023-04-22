#pragma once

#include <array>
#include <memory>
#include <cpaf_libs/audio/cpaf_audio_device_base.h>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/play_stream.h>
#include <cpaf_libs/video/media_stream_time.h>
#include <cpaf_libs/video/audio_resampler.h>

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

    bool                        has_video_source_stream () const { return has_source_stream(stream_type_t::video); }
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

    audio_resampler&            audio_resampler_get     () { return audio_resampler_; }
    av_samples_queue&           audio_samples_queue     () { return audio_samples_queue_; }


    // ---------------------------
    // --- Video setup/control ---
    // ---------------------------
    void                        video_dimensions_set    (int32_t width, int32_t height);
    void                        video_dimensions_set    (const surface_dimensions_t& dimensions);
    void                        video_scaler_flags_set  (int32_t flags);
    void                        video_scaler_align_set  (int32_t align);
    av_codec_context&           video_codec_context     () const;

    // ----------------------------
    // --- Video info functions ---
    // ----------------------------
    surface_dimensions_t        video_src_dimensions    () const;
    surface_dimensions_t        video_dst_dimensions    () const;
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
    std::array<std::unique_ptr<play_stream>, stream_type_index_size()>  source_streams_         = {nullptr, nullptr, nullptr, nullptr, nullptr};
    std::unique_ptr<play_stream>                                        primary_source_stream_  ;
    surface_dimensions_t                                                video_dimensions_       = {-1,-1};
    int32_t                                                             video_scaler_flags_     = SWS_BILINEAR;
    int32_t                                                             video_scaler_align_     = 32;
    media_stream_time                                                   cur_media_time_;
    std::atomic_bool                                                    threads_running_        = true;
    std::atomic_bool                                                    threads_paused_         = false;
    mutable av_codec_context                                            video_codec_ctx_;
    av_samples_queue                                                    audio_samples_queue_;
    audio_resampler                                                     audio_resampler_;
    av_codec_context*               video_codec_ctx_ptr_FIXMENM_            = nullptr;

//    video_render_thread                                                 video_render_thread_;
    std::string                                                         primary_resource_path_;

};

} //END namespace cpaf::video

