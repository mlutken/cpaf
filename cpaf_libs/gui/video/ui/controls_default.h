#pragma once
#include <imguipp/imgui_rai.h>
#include <cpaf_libs/gui/video/ui/controls.h>
#include <cpaf_libs/gui/texture.h>

namespace cpaf::gui::video {

class controls_default : public controls
{
public:
    using controls::controls;	// "Import" constructors to scope
    controls_default(player& parent_player, config& cfg);

private:
    void    do_calc_geometry                () override;
    void    do_render                       () override;

    void    render_player_controls          ();
    void    render_menu_buttons             ();
    void    render_slider                   ();
    void    render_player_time              ();

    void    render_subtitles_popup          ();
    void    render_menu_window              ();
    void    render_subtitles_window         ();

    void    render_volume_popup             ();
    void    render_debug_window             ();



    void    set_cursor_pos_image_buttons    ();

    static constexpr float slider_illegal_pos   {-1};
    static constexpr float general_margin       {4};
    static constexpr float image_buttons_window_size_extra   {2};

    std::unique_ptr<cpaf::gui::texture> control_icons_texture_   {};
    ImVec2      file_dialog_min_size_           {300,200};
    ImVec2      play_buttons_size_              {};
    ImVec2      play_buttons_window_size_       {};

    ImVec2      menu_buttons_size_              {};
    ImVec2      menu_buttons_window_size_       {};

    ImVec2      video_slider_size_              {};
    ImVec2      video_time_size_                {};

    ImVec2      play_pause_btn_pos_             {};
    ImVec2      video_back_btn_pos_             {};
    ImVec2      video_fwd_btn_pos_              {};
    ImVec2      subtitles_btn_pos_              {};
    ImVec2      menu_btn_pos_                   {};
    ImVec2      volume_btn_pos_                 {};

    ImVec2      video_slider_pos_               {};
    ImVec2      elapsed_time_pos_               {};
    ImVec2      remaining_time_pos_             {};
    float       video_slider_grab_width_        {20};
    float       slider_last_user_pos_seconds_   {slider_illegal_pos};
    color       buttons_col_                    {};
    color       buttons_hover_col_              {};
    color       buttons_active_col_             {};

    color       time_text_col_                  {};

    ImFont*     font_slider_                    = nullptr;
    ImFont*     font_time_                      = nullptr;

    bool        do_render_subtitles_window_     = true; // FIXMENM FIXMENM Set to false when done creating the window
    bool        do_render_menu_window_          = false;

    std::string new_user_subtitle_file_         {};


};

} // namespace cpaf::gui::video
