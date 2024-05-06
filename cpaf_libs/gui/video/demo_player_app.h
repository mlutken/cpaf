#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <cpaf_libs/audio/cpaf_audio_device.h>
#include <cpaf_libs/gui/app.h>
#include <cpaf_libs/gui/video/player.h>
#include <cpaf_libs/gui/video/render/render.h>
#include <cpaf_libs/gui/video/config.h>
#include <cpaf_libs/gui/texture.h>

namespace cpaf::gui::video {

// --------------------------
// --- demo_event_handler ---
// --------------------------
class demo_event_handler
{
public:
    explicit demo_event_handler(cpaf::gui::video::player&  player);
    cpaf::gui::events::is_handled   handle_event               (const cpaf::gui::events::event& evt);

private:
    cpaf::gui::video::player&  player_;
    cpaf::gui::events::is_handled   handle_window_event         (const cpaf::gui::events::window& evt);
    cpaf::gui::events::is_handled   handle_keyboard_event       (const cpaf::gui::events::keyboard& evt);
    cpaf::gui::events::is_handled   handle_key_pressed          (const cpaf::gui::events::keyboard& evt);

    std::chrono::seconds   skip_time_small_{15};
    std::chrono::seconds   skip_time_large_{60};
};

// -----------------------
// --- demo_player_app ---
// -----------------------
class demo_player_app : public cpaf::gui::app
{
public:
    using app::app;
    demo_player_app();

    void                            set_initial_playlist        (const nlohmann::json& playlist);
    void                            set_initial_start_time      (std::chrono::microseconds media_start_time_pos);
    bool                            has_video_stream            () const { return player_.has_video_stream(); }
    void                            open_path                   (const std::string& resource_path);
    void                            open_playable_or_list       (const nlohmann::json& playable_or_list);
    void                            open_playable               (const cpaf::gui::video::playable& playab);
    void                            open_playable               (const nlohmann::json& playable_jo);
    bool                            open_playlist               (const nlohmann::json& playlist);
    bool                            playlist_play_item          (uint32_t pl_index);
    bool                            playlist_play_next          ();
    bool                            playlist_play_prev          ();

    const cpaf::audio::device&      audio_device                () const { return audio_device_; }
    void                            dbg_print                   () const;

    cpaf::gui::video::config&       configuration               () { return player_.configuration; }

protected:
    void                            start_run                   () override;
    void                            frame_update                () override;
    cpaf::gui::events::is_handled   event_handler               (const cpaf::gui::events::event& evt) override;

private:
    void                            render_main_menu            ();
    void                            render_debug_panel         ();

    uint32_t                        playlist_start_play_index   () const    { return 0; }
    uint32_t                        playlist_get_next_play_index() const;
    uint32_t                        playlist_get_prev_play_index() const;
    uint32_t                        playlist_size               () const    { return playlist_["items"].size(); }
    bool                            is_valid_playlist_index     (uint32_t pl_index) const { return pl_index < playlist_size(); }
    void                            on_configuration_changed    ();
    bool                            ui_visible                  () const { return time_since_last_mouse_or_touch() < show_ui_timeout_;}


    cpaf::audio::device             audio_device_;
    cpaf::gui::video::player        player_;
    demo_event_handler              player_event_handler_;

    std::chrono::microseconds       media_start_time_pos_   {0};
    std::chrono::seconds            show_ui_timeout_        {2};
    nlohmann::json                  playlist_;
    uint32_t                        playlist_current_index_  {0};

    bool                            m_show_some_panel       {true};
    bool                            m_show_debug_panel      {false};
    bool                            m_show_imgui_dbg_window {false};
    std::unique_ptr<cpaf::gui::texture> dbg_text_texture_   {};
};

} // namespace cpaf::gui::video

