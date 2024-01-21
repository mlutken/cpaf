#pragma once

#include <array>
#include <memory>
#include <cpaf_libs/gui/gui_types.h>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/play_stream.h>
#include <cpaf_libs/video/media_stream_time.h>
#include <cpaf_libs/gui/video/pipeline_threads/pipeline_threads.h>

namespace cpaf::audio {
class device;
}

namespace cpaf::gui {
class system_window;
}


namespace cpaf::torrent {
class torrents;
}


namespace cpaf::gui::video {

class render;
class controls;

class player
{
public:
    using audio_play_callback_t  = cpaf::audio::device_base::play_callback_t;

    explicit player(cpaf::audio::device& audio_device);
    ~player();
    void                            set_main_window         (const system_window& main_window);
///    void                            init                    ();
    void                            start_playing           (const std::chrono::microseconds& start_time_pos = std::chrono::microseconds(0));
    void                            terminate               ();
    bool                            open                    (const std::string& resource_path);
    void                            open_async              (const std::string& resource_path, std::chrono::microseconds start_time_pos = {});
    void                            close                   ();
    void                            close_async             ();
    void                            cancel_async_open       ();

    bool                            open_secondary                    (const std::string& resource_path, cpaf::video::stream_type_t sti);

    bool                            has_video_stream        () const { return has_source_stream(cpaf::video::stream_type_t::video); }
    bool                            has_audio_stream        () const { return has_source_stream(cpaf::video::stream_type_t::audio); }
    bool                            has_subtitle_stream     () const { return has_source_stream(cpaf::video::stream_type_t::subtitle); }
    bool                            has_source_stream       (cpaf::video::stream_type_t sti) const;
    const cpaf::video::play_stream* source_stream           (cpaf::video::stream_type_t sti) const;
    cpaf::video::play_stream*       source_stream           (cpaf::video::stream_type_t sti);

    cpaf::video::play_stream*       video_stream            ()       { return source_stream(cpaf::video::stream_type_t::video); }
    const cpaf::video::play_stream* video_stream            () const { return source_stream(cpaf::video::stream_type_t::video); }
    cpaf::video::play_stream*       audio_stream            ()       { return source_stream(cpaf::video::stream_type_t::audio); }
    const cpaf::video::play_stream* audio_stream            () const { return source_stream(cpaf::video::stream_type_t::audio); }
    cpaf::video::play_stream*       subtitle_stream         ()       { return source_stream(cpaf::video::stream_type_t::subtitle); }
    const cpaf::video::play_stream* subtitle_stream         () const { return source_stream(cpaf::video::stream_type_t::subtitle); }

    cpaf::video::play_stream&       primary_stream          ()       { return *primary_source_stream_; }
    const cpaf::video::play_stream& primary_stream          () const { return *primary_source_stream_; }
    const std::string&              primary_resource_path	() const { return primary_resource_path_; }
    std::string                     video_resource_path     () const { return video_stream() ? video_stream()->resource_path() : ""; }
    std::string                     audio_resource_path     () const { return audio_stream() ? audio_stream()->resource_path() : ""; }
    std::string                     subtitle_resource_path  () const { return subtitle_stream() ? subtitle_stream()->resource_path() : ""; }

    // ----------------------------------
    // --- Current media playing info ---
    // ----------------------------------
    std::chrono::microseconds       current_time            () const { return cur_media_time().current_time_pos(); }
    std::chrono::microseconds       total_time              () const;
    std::chrono::microseconds       remaining_time          () const { return total_time() - current_time(); }
    bool                            is_playing              () const;
    std::atomic<stream_state_t>&        stream_state        ()       { return primary_stream().stream_state(); }
    const std::atomic<stream_state_t>&  stream_state        () const { return primary_stream().stream_state(); }

    // ----------------
    // --- Contexts ---
    // ----------------
    av_format_context&                  format_context          ()                          { return primary_stream().format_context(); }
    const av_format_context&            format_context          () const                    { return primary_stream().format_context(); }
    cpaf::video::av_codec_context&      video_codec_context     () const;
    cpaf::video::av_codec_context&      audio_codec_context     () const;
    cpaf::video::av_codec_context&      subtitle_codec_context  () const;

    // ---------------------------
    // --- Video setup/control ---
    // ---------------------------
    void                                video_dimensions_set    (int32_t width, int32_t height = cpaf::video::surface_dimension_auto);
    void                                video_dimensions_set    (const cpaf::video::surface_dimensions_t& dimensions);
    void                                video_scaler_flags_set  (int32_t flags);
    void                                video_scaler_align_set  (int32_t align);
    void                                ff_dst_pixel_format_set (AVPixelFormat pixel_format);

    // ----------------------------
    // --- Video info functions ---
    // ----------------------------
    cpaf::video::surface_dimensions_t   video_src_dimensions        () const;
    cpaf::math::v2f                     video_src_dimensions_float  () const;
    cpaf::video::surface_dimensions_t   video_dst_dimensions        () const;
    size_t                              video_stream_index          () const;
    rect                                render_geometry             () const;

    // ---------------------------
    // --- Audio setup/control ---
    // ---------------------------
    bool                                audio_out_formats_set   (const cpaf::audio::device& audio_device);
    bool                                audio_out_formats_set   (const cpaf::video::ff_audio_format_t& ff_audio_format);

    // ----------------------------
    // --- Audio info functions ---
    // ----------------------------
    size_t                              audio_stream_index		() const;
    const cpaf::video::av_samples_queue&audio_samples_queue     () const { return media_pipeline_threads_->audio_samples_queue(); }

    // -------------------------------
    // --- Subtitles setup/control ---
    // -------------------------------
    subtitle_source_t                   subtitle_source         () const { return subtitle_source_; }
    size_t                              subtitle_stream_index	() const;

    // ---------------------------------------------
    // --- Interfacing to surrounding app/system ---
    // ---------------------------------------------
    const system_window*        main_window_ptr         () const { return main_window_ptr_; }
    audio_play_callback_t       audio_callback_get      ();
    void                        frame_update                  ();

    // TODO: Make these two private
    void                        video_frame_update      (cpaf::video::av_frame& current_frame, cpaf::gui::video::render& video_render);
    void                        video_frame_update      (cpaf::video::av_frame& current_frame);



    // --------------------
    // --- Play control ---
    // --------------------
    void                        seek_position           (const std::chrono::microseconds& stream_pos,
                                                         cpaf::video::seek_dir dir);
    void                        seek_position           (const std::chrono::microseconds& stream_pos);
    void                        seek_relative           (const std::chrono::microseconds& delta_time);
    void                        pause_playback          ();
    void                        resume_playback         ();
    void                        toggle_pause_playback   ();
    bool                        playback_paused         () const;
    cpaf::video::seek_state_t   seek_state              () const;
    std::chrono::microseconds   seek_from_position      () const;
    std::chrono::microseconds   seek_position_requested () const;

    // ----------
    // --- UI ---
    // ----------
    void                        set_controls            (std::unique_ptr<video::controls> controls);
    bool                        show_controls           () const                        { return show_controls_;    }
    void                        show_controls_set       (bool show)                     { show_controls_ = show;    }


    cpaf::video::media_stream_time&         cur_media_time  ()       { return cur_media_time_; }
    const cpaf::video::media_stream_time&   cur_media_time  () const { return cur_media_time_; }
    std::shared_ptr<torrent::torrents>      torrents_get    () const;
    void                                    torrents_set    (std::shared_ptr<torrent::torrents> tors) { torrents_ = tors; }

    // -----------------
    // --- Callbacks ---
    // -----------------
    void                        cb_start_playing            (std::function<void ()>&& cb) { cb_start_playing_ = std::move(cb); }


    // -----------------------
    // --- Debug functions ---
    // -----------------------
    std::string                 queues_info                 () const;

private:
    // ---------------------------------
    // --- PRIVATE: Helper functions ---
    // ---------------------------------
    void                            init_video              (const system_window& main_window);
    bool                            open_stream             (const std::string& resource_path, cpaf::video::stream_type_t sti);
    bool                            open_primary_stream     (const std::string& resource_path);
    void                            update_scaling_context  () const;
    pipeline_threads&               media_pipeline_threads  () { return *media_pipeline_threads_; }
    const pipeline_threads&         media_pipeline_threads  () const { return *media_pipeline_threads_; }
    void                            handle_internal_events  ();
    void                            handle_stream_state     ();

    // ----------------------------
    // --- PRIVATE: Member vars ---
    // ----------------------------
    using source_streams_array_t = std::array<std::unique_ptr<cpaf::video::play_stream>, cpaf::video::stream_type_index_size()>;
    cpaf::audio::device&                            audio_device_;
    std::unique_ptr<cpaf::video::play_stream>       primary_source_stream_;
    std::chrono::microseconds                       start_time_pos_;
    std::unique_ptr<pipeline_threads>               media_pipeline_threads_;
    const system_window*                            main_window_ptr_                = nullptr;
    source_streams_array_t                          source_streams_                 = {nullptr, nullptr, nullptr, nullptr, nullptr};
    cpaf::video::surface_dimensions_t               video_dst_dimensions_requested_ = {cpaf::video::surface_dimension_auto,cpaf::video::surface_dimension_auto};
    int32_t                                         video_scaler_flags_             = SWS_BILINEAR;
    int32_t                                         video_scaler_align_             = 32;
    AVPixelFormat                                   ff_dst_pixel_format_            = AV_PIX_FMT_YUV420P;
    ///std::atomic_bool                                threads_running_                = true;
///    std::atomic_bool                                threads_paused_                 = false;
    size_t                                          video_stream_index_             = cpaf::video::no_stream_index;
    size_t                                          audio_stream_index_             = cpaf::video::no_stream_index;
    size_t                                          subtitle_stream_index_          = cpaf::video::no_stream_index;

    cpaf::video::av_frame                           next_video_frame_;
    mutable cpaf::video::av_codec_context           video_codec_ctx_;
    mutable cpaf::video::av_codec_context           audio_codec_ctx_;
    mutable cpaf::video::av_codec_context           subtitle_codec_ctx_;
    cpaf::video::audio_resampler                    audio_resampler_;

    std::string                                     primary_resource_path_;
    mutable std::shared_ptr<torrent::torrents>      torrents_;
    cpaf::video::media_stream_time                  cur_media_time_;
    std::unique_ptr<video::render>                  video_render_;
    std::unique_ptr<video::controls>                video_controls_;
    bool                                            show_controls_                  = true;
    bool                                            resume_from_pause_on_seek_      = true;
    subtitle_source_t                               subtitle_source_                = subtitle_source_t::stream;    /// @todo
    std::function<void ()>                          cb_start_playing_;
    std::unique_ptr<std::jthread>                   open_thread_                    = nullptr;

};

} //END namespace cpaf::gui::video

