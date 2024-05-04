#pragma once

#include <array>
#include <memory>
#include <thread>
#include <mutex>
#include <cpaf_libs/locale/translator.h>
#include <cpaf_libs/gui/gui_types.h>
#include <cpaf_libs/video/play_stream.h>
#include <cpaf_libs/video/media_stream_time.h>
#include <cpaf_libs/gui/video/pipeline_threads/pipeline_threads.h>
#include <cpaf_libs/gui/video/pipeline_threads/subtitle_downloader_thread.h>
#include <cpaf_libs/gui/video/config.h>
#include <cpaf_libs/gui/video/data_definitions/playable.h>
#include <cpaf_libs/gui/video/pipeline_threads/play_handler_thread.h>

namespace cpaf::audio {
class device;
}

namespace cpaf::gui {
class system_window;
}


namespace cpaf::torrent {
class torrent;
class torrents;
}


namespace cpaf::gui::video {

class render;
class controls;


class player
{
    friend class play_handler_thread;
public:
    using audio_play_callback_t  = cpaf::audio::device_base::play_callback_t;

    explicit player(cpaf::audio::device& audio_device, cpaf::locale::translator& tr);
    ~player();
    void                            set_main_window         (system_window& main_window);
    void                            terminate               ();
    void                            open_media              (playable playab);
    void                            open_media              (const std::string& resource_path, std::chrono::microseconds start_time_pos = {});
    void                            close_current_media     ();

    bool                            open_secondary          (const std::string& resource_path, cpaf::video::stream_type_t sti);

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
    std::string                     primary_resource_path	() const { return cur_playable().path(); }
    std::string                     video_resource_path     () const { return video_stream() ? video_stream()->resource_path() : ""; }
    std::string                     audio_resource_path     () const { return audio_stream() ? audio_stream()->resource_path() : ""; }
    std::string                     subtitle_resource_path  () const { return subtitle_stream() ? subtitle_stream()->resource_path() : ""; }

    // ----------------------------------
    // --- Current media playing info ---
    // ----------------------------------
    std::chrono::microseconds           current_time            () const { return cur_media_time().current_time_pos(); }
    std::chrono::microseconds           total_time              () const;
    std::chrono::microseconds           remaining_time          () const { return total_time() - current_time(); }
    bool                                is_playing              () const;
    const std::atomic<stream_state_t>&  primary_stream_state    () const { return primary_stream_state_; }
    std::atomic<stream_state_t>&        primary_stream_state    ()       { return primary_stream_state_; }
    const playable&                     cur_playable            () const;
    std::chrono::microseconds
                                        current_io_operation_duration() const;

    bool                                is_waiting_for_io       () const { return current_io_operation_duration() > std::chrono::seconds(3); }
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
    void                                player_geometry_set     (int32_t width, int32_t height);
    void                                player_geometry_set     (const rect& render_geom);
    void                                video_scaler_flags_set  (int32_t flags);
    void                                video_scaler_align_set  (int32_t align);
    void                                ff_dst_pixel_format_set (AVPixelFormat pixel_format);
    cpaf::math::v2f                     screen_size_factor_use  () const { return screen_size_factor_use_; }
    cpaf::math::v2f                     screen_size_factor_raw  () const { return screen_size_factor_raw_; }

    // ----------------------------
    // --- Video info functions ---
    // ----------------------------
    cpaf::video::surface_dimensions_t   video_src_dimensions        () const;
    cpaf::math::v2f                     video_src_dimensions_float  () const;
    cpaf::video::surface_dimensions_t   video_dst_dimensions        () const;
    size_t                              video_stream_index          () const;
    rect                                render_geometry             () const;
    rect                                video_render_geometry       () const;

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
    void                                enqueue_subtitle_file   (const std::string& subtitle_path, const std::string& language_code,
                                                                 std::chrono::microseconds subtitle_adjust_offset);
    subtitle_source_t                   subtitle_source         () const { return subtitle_source_; }
    void                                subtitle_select         (const std::string& language_code, std::chrono::microseconds subtitle_adjust_offset = {});
    void                                subtitle_select         (int32_t selectable_subtitle_index, std::chrono::microseconds subtitle_adjust_offset = {});
    void                                set_subtitle_user       (std::string subtitle_path)         { current_playable_.set_subtitle_user(subtitle_path);   }
    std::string                         subtitle_user           () const                            { return current_playable_.subtitle_user();             }

    const cpaf::video::subtitle_source_entries_t&
    selectable_subtitles                                        () const { return current_playable_.selectable_subtitles(); }
    int32_t                             subtitle_selected_index () const;

    size_t                              subtitle_stream_index	() const;
    void                                subtitle_stream_index_set(size_t stream_index, std::chrono::microseconds subtitle_adjust_offset = {});
    void                                subtitle_container_set  (std::unique_ptr<subtitle_container> container);

    // ---------------------------------------------
    // --- Interfacing to surrounding app/system ---
    // ---------------------------------------------
    const system_window*        main_window_ptr         () const { return main_window_ptr_; }
    audio_play_callback_t       audio_callback_get      ();
    void                        frame_update            ();
    void                        toggle_full_screen      ();

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
    void                        playback_paused_set     (bool is_paused);
    bool                        playback_is_paused      () const;
    cpaf::video::seek_state_t   seek_state              () const;
    std::chrono::microseconds   seek_from_position      () const;
    std::chrono::microseconds   seek_position_requested () const;

    // ----------
    // --- UI ---
    // ----------
    void                        set_controls            (std::unique_ptr<video::controls> controls);
    bool                        show_controls           () const                        { return show_controls_;    }
    void                        show_controls_set       (bool show)                     { show_controls_ = show;    }
    void                        ui_window_active_set    (bool ui_window_active);
    void                        ui_events_enabled_set   (bool enabled)                  { ui_events_enabled_ = enabled; }
    bool                        ui_events_enabled       () const                        { return ui_events_enabled_; }
    bool                        ui_window_active        () const                        { return ui_window_active_; }

    cpaf::locale::translator&               tr              ()          { return tr_; }
    const cpaf::locale::translator&         tr              () const    { return tr_; }

    cpaf::video::media_stream_time&         cur_media_time  ()          { return cur_media_time_; }
    const cpaf::video::media_stream_time&   cur_media_time  () const    { return cur_media_time_; }
    std::shared_ptr<torrent::torrents>      torrents_get    () ;
    void                                    torrents_set    (std::shared_ptr<torrent::torrents> tors);

    // -----------------
    // --- Callbacks ---
    // -----------------
    void                            cb_start_playing        (std::function<void ()>&& cb) { cb_start_playing_ = std::move(cb); }


    // -----------------------
    // --- Debug functions ---
    // -----------------------
    std::string                     queues_info             () const;
    const cpaf::video::av_frame&    next_video_frame        () const { return next_video_frame_; }
    std::chrono::microseconds       dbg_audio_front_time    () const;

    // ----------------------
    // --- Public members ---
    // ----------------------
    cpaf::gui::video::config    configuration;

private:
    // ---------------------------------
    // --- PRIVATE: Helper functions ---
    // ---------------------------------
    bool                            open_command            (playable playab);
    void                            start_playing           ();
    void                            close_command           ();
    void                            close_media             (std::chrono::milliseconds wait_for_threads_to_stop_time = std::chrono::milliseconds(0));


    void                            reset_primary_stream    (std::unique_ptr<cpaf::video::play_stream> new_primary_stream = nullptr);
    bool                            all_initialized         () const;
    void                            init_video              (const system_window& main_window);
    bool                            open_stream             (const std::string& resource_path, cpaf::video::stream_type_t sti);
    bool                            open_primary_stream     (const std::string& resource_path);
    void                            check_activate_subtitle ();
    void                            update_scaling_context  (bool mutex_already_locked = false) const;
    pipeline_threads&               media_pipeline_threads  () { return *media_pipeline_threads_; }
    const pipeline_threads&         media_pipeline_threads  () const { return *media_pipeline_threads_; }
    void                            handle_internal_events  ();
    void                            handle_stream_state     ();
    void                            handle_close_media      ();
    void                            torrent_finished_event  (std::shared_ptr<cpaf::torrent::torrent> tor_file);
    void                            on_configuration_changed();
    void                            cur_playable_upd_calc   (bool force);

    void                            update_screen_size_factor();
    bool                            should_show_stream_state       () const;
    bool                            set_subtitle_helper     (int32_t selectable_subtitle_index);
    void                            internal_paused_set     (bool is_paused);
    void                            push_paused             ();
    void                            pop_paused              ();
    void                            cur_playable_set        (playable playab);


    // ----------------------------
    // --- PRIVATE: Member vars ---
    // ----------------------------
    using source_streams_array_t = std::array<std::unique_ptr<cpaf::video::play_stream>, cpaf::video::stream_type_index_size()>;
    std::atomic<stream_state_t>                     primary_stream_state_           = stream_state_t::inactive;
    cpaf::audio::device&                            audio_device_;
    cpaf::locale::translator&                       tr_;
    subtitle_downloader_thread                      subtitle_downloader_thread_;
    play_handler_thread                             play_handler_thread_;
    playable                                        current_playable_;
    std::unique_ptr<cpaf::video::play_stream>       primary_source_stream_;
    std::chrono::microseconds                       start_time_pos_;
    std::unique_ptr<pipeline_threads>               media_pipeline_threads_;
    system_window*                                  main_window_ptr_                = nullptr;
    source_streams_array_t                          source_streams_                 = {nullptr, nullptr, nullptr, nullptr, nullptr};
    cpaf::video::surface_dimensions_t               video_dst_dimensions_requested_ = {cpaf::video::surface_dimension_auto,cpaf::video::surface_dimension_auto};
    int32_t                                         video_scaler_flags_             = SWS_BILINEAR;
    int32_t                                         video_scaler_align_             = 32;
    AVPixelFormat                                   ff_dst_pixel_format_            = AV_PIX_FMT_YUV420P;
    size_t                                          video_stream_index_             = cpaf::video::no_stream_index;
    size_t                                          audio_stream_index_             = cpaf::video::no_stream_index;
    size_t                                          subtitle_stream_index_          = cpaf::video::no_stream_index;

    cpaf::video::av_frame                           next_video_frame_;
    mutable cpaf::video::av_codec_context           video_codec_ctx_;
    mutable cpaf::video::av_codec_context           audio_codec_ctx_;
    mutable cpaf::video::av_codec_context           subtitle_codec_ctx_;
    mutable std::mutex                              video_codec_mutex_;
    mutable std::mutex                              audio_codec_mutex_;
    mutable std::mutex                              subtitle_codec_mutex_;
    mutable std::mutex                              current_playable_mutex_;
    cpaf::video::audio_resampler                    audio_resampler_;

    mutable std::shared_ptr<torrent::torrents>      torrents_;
    cpaf::video::media_stream_time                  cur_media_time_;
    std::unique_ptr<video::render>                  video_render_;
    std::unique_ptr<video::controls>                video_controls_;
    /// bool                                            keep_aspect_ratio_              = true;
    bool                                            show_controls_                  = false;
    bool                                            resume_from_pause_on_seek_      = true;

    bool                                            save_playback_is_paused_state_  = false;    // For saving current paused state. For example when showing i UI window on top and we want to ensure player is paused while window is active. See push_paused()/pop_paused()
    std::atomic<bool>                               stream_completely_downloaded_   = false;    // When active for example pause is forced when for example showing UI windows on top of player
    std::atomic<bool>                               ui_window_active_               = false;    // When active for example pause is forced when for example showing UI windows on top of player
    std::atomic<bool>                               ui_events_enabled_              = true;     // Use to disable player ui events when for example showing UI windows on top of player
    std::atomic<subtitle_source_t>                  subtitle_source_                = subtitle_source_t::none;
    int32_t                                         subtitle_selected_index_        = -1;
    std::function<void ()>                          cb_start_playing_;
    cpaf::gui::rect                                 player_render_geometry_         {0,0,0,0};
    cpaf::math::v2f                                 screen_size_factor_use_         {1,1};
    cpaf::math::v2f                                 screen_size_factor_raw_         {1,1};

    std::atomic<bool>                               close_media_requested_          = false;

};

} //END namespace cpaf::gui::video

