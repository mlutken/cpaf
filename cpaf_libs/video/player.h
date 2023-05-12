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

    player();
    ~player();
    void                        start                   (const std::chrono::microseconds& start_time_pos = std::chrono::microseconds(0));
    void                        terminate               ();
    bool                        open                    (const std::string& resource_path);
    bool                        open                    (const std::string& resource_path, stream_type_t sti);

    bool                        has_video_stream        () const { return has_source_stream(stream_type_t::video); }
    bool                        has_audio_stream        () const { return has_source_stream(stream_type_t::audio); }
    bool                        has_subtitle_stream     () const { return has_source_stream(stream_type_t::subtitle); }
    bool                        has_source_stream       (stream_type_t sti) const;
    const play_stream*          source_stream           (stream_type_t sti) const;
    play_stream*                source_stream           (stream_type_t sti);

    play_stream*                video_stream            ()       { return source_stream(stream_type_t::video); }
    const play_stream*          video_stream            () const { return source_stream(stream_type_t::video); }
    play_stream*                audio_stream            ()       { return source_stream(stream_type_t::audio); }
    const play_stream*          audio_stream            () const { return source_stream(stream_type_t::audio); }
    play_stream*                subtitle_stream         ()       { return source_stream(stream_type_t::subtitle); }
    const play_stream*          subtitle_stream         () const { return source_stream(stream_type_t::subtitle); }

    play_stream&                primary_stream          ()       { return primary_source_stream_; }
    const play_stream&          primary_stream          () const { return primary_source_stream_; }
    const std::string&          primary_resource_path	() const { return primary_resource_path_; }
    std::string                 video_resource_path     () const { return video_stream() ? video_stream()->resource_path() : ""; }
    std::string                 audio_resource_path     () const { return audio_stream() ? audio_stream()->resource_path() : ""; }
    std::string                 subtitle_resource_path  () const { return subtitle_stream() ? subtitle_stream()->resource_path() : ""; }

    media_stream_time&          cur_media_time          ()          { return cur_media_time_; }
    const media_stream_time&    cur_media_time          () const    { return cur_media_time_; }

    // ---------------------------
    // --- Video setup/control ---
    // ---------------------------
    void                        video_dimensions_set    (int32_t width, int32_t height = surface_dimension_auto);
    void                        video_dimensions_set    (const surface_dimensions_t& dimensions);
    void                        video_scaler_flags_set  (int32_t flags);
    void                        video_scaler_align_set  (int32_t align);
    void                        ff_dst_pixel_format_set (AVPixelFormat pixel_format);
    av_codec_context&           video_codec_context     () const;
    av_codec_context&           audio_codec_context     () const;

    // ----------------------------
    // --- Video info functions ---
    // ----------------------------
    surface_dimensions_t        video_src_dimensions    () const;
    surface_dimensions_t        video_dst_dimensions    () const;
    size_t                      video_stream_index		() const;

    // ---------------------------
    // --- Audio setup/control ---
    // ---------------------------
    bool                    	audio_out_formats_set   (const cpaf::audio::device& audio_device);
    bool                        audio_out_formats_set   (const ff_audio_format_t& ff_audio_format);

    // ----------------------------
    // --- Audio info functions ---
    // ----------------------------
    size_t                      audio_stream_index		() const;

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

    // -----------------------
    // --- Debug functions ---
    // -----------------------
    std::string                 queues_info             () const;

private:
    // ---------------------------------
    // --- PRIVATE: Helper functions ---
    // ---------------------------------
    bool                        open_stream             (const std::string& resource_path, stream_type_t sti);
    bool                        open_primary_stream     (const std::string& resource_path);
    void                        current_media_time_set  (media_stream_time* mts);
    void                        current_media_time_set  (media_stream_time& mts);
    void                        update_scaling_context  () const;
    pipeline_threads&           media_pipeline_threads  () { return media_pipeline_threads_; }

    // ----------------------------
    // --- PRIVATE: Member vars ---
    // ----------------------------
    media_stream_time                                                   cur_media_time_;
    play_stream                                                         primary_source_stream_;
    std::array<std::unique_ptr<play_stream>, stream_type_index_size()>  source_streams_                 = {nullptr, nullptr, nullptr, nullptr, nullptr};
    surface_dimensions_t                                                video_dst_dimensions_requested_ = {surface_dimension_auto,surface_dimension_auto};
    int32_t                                                             video_scaler_flags_             = SWS_BILINEAR;
    int32_t                                                             video_scaler_align_             = 32;
    AVPixelFormat                                                       ff_dst_pixel_format_            = AV_PIX_FMT_YUV420P;
    std::atomic_bool                                                    threads_running_                = true;
    std::atomic_bool                                                    threads_paused_                 = false;
    size_t                                                              video_stream_index_             = no_stream_index;
    size_t                                                              audio_stream_index_             = no_stream_index;

    mutable av_codec_context                                            video_codec_ctx_;
    mutable av_codec_context                                            audio_codec_ctx_;
    av_samples_queue                                                    audio_samples_queue_;
    audio_resampler                                                     audio_resampler_;
    pipeline_threads                                                    media_pipeline_threads_;

    std::string                                                         primary_resource_path_;
};

} //END namespace cpaf::video

