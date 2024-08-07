#include "controls_default__sdl2.h"

#include <ImGuiFileDialog.h>
#include <cpaf_libs/time/cpaf_time.h>
#include <cpaf_libs/filesystem/cpaf_special_dirs.h>
#include <cpaf_libs/gui/fonts/font_size.h>
#include <cpaf_libs/gui/assets/fonts/imgui_fonts.h>
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/video/player.h>
#include <cpaf_libs/gui/video/ui/assets/video_player_data.h>


using namespace cpaf::time;
using namespace std::chrono;

namespace cpaf::gui::video {

using pos_2df = cpaf::gui::pos_2df;

constexpr float icon_width = 0.25f;
constexpr pos_2df wh_add = {icon_width, icon_width};

//constexpr pos_2df forward_uv0       = {3*icon_width, 0*icon_width};  constexpr pos_2df forward_uv1    = forward_uv0   + wh_add;
constexpr pos_2df forward_uv0       = {3*icon_width, 3*icon_width};  constexpr pos_2df forward_uv1      = forward_uv0   + wh_add;
constexpr pos_2df pause_uv0         = {1*icon_width, 2*icon_width};  constexpr pos_2df pause_uv1        = pause_uv0     + wh_add;
constexpr pos_2df play_uv0          = {1*icon_width, 3*icon_width};  constexpr pos_2df play_uv1         = play_uv0      + wh_add;
//constexpr pos_2df backward_uv0      = {0*icon_width, 3*icon_width};  constexpr pos_2df backward_uv1 = backward_uv0  + wh_add;
constexpr pos_2df backward_uv0      = {3*icon_width, 2*icon_width};  constexpr pos_2df backward_uv1     = backward_uv0  + wh_add;
constexpr pos_2df menu_uv0          = {1*icon_width, 0*icon_width};  constexpr pos_2df menu_uv1         = menu_uv0  + wh_add;
constexpr pos_2df subtitles_off_uv0 = {2*icon_width, 1*icon_width};  constexpr pos_2df subtitles_off_uv1= subtitles_off_uv0  + wh_add;
constexpr pos_2df subtitles_on_uv0  = {2*icon_width, 2*icon_width};  constexpr pos_2df subtitles_on_uv1 = subtitles_on_uv0  + wh_add;
constexpr pos_2df fullscreen_uv0    = {0*icon_width, 2*icon_width};  constexpr pos_2df fullscreen_uv1   = fullscreen_uv0  + wh_add;

/// @see https://github.com/ocornut/imgui/issues/4216
/// @see https://html-color.codes/




controls_default_platform::controls_default_platform(player& parent_player, config& cfg)
    : controls(parent_player, cfg)
{
    control_icons_texture_ = std::make_unique<cpaf::gui::texture>(parent_player.main_window_ptr()->renderer_shared());
    control_icons_texture_ ->load_png_from_memory(cpaf::gui::video_player_data::video_player_control_icons());
}

void controls_default_platform::do_calc_geometry()
{
    const auto render_geometry = player_.render_geometry();
    const auto screen_fac = player_.screen_size_factor_use().height();
    const auto btns_relative_x_dist = buttons_relative_x_dist();
    // -----------------------
    // --- Calculate sizes ---
    // -----------------------
    const auto play_btns_size = play_buttons_size()*screen_fac;
    play_buttons_size_ = ImVec2{play_btns_size, play_btns_size};
    play_buttons_window_size_.x = play_buttons_size_.x + image_buttons_window_size_extra*2;
    play_buttons_window_size_.y = play_buttons_size_.y + image_buttons_window_size_extra*2;

    const auto menu_btns_size = menu_buttons_size()*screen_fac;
    menu_buttons_size_ = ImVec2{menu_btns_size, menu_btns_size};
    menu_buttons_window_size_.x = menu_buttons_size_.x + image_buttons_window_size_extra*2;
    menu_buttons_window_size_.y = menu_buttons_size_.y + image_buttons_window_size_extra*2;

    const int32_t slider_font_size_pixels = font_size::to_pixels(slider_height()*screen_fac, player_.main_window_ptr());
    video_slider_size_.x = render_geometry.size().width() - 2*general_margin;
    video_slider_size_.y = slider_font_size_pixels + 2*general_margin;
    video_slider_grab_width_ = play_btns_size / 2;

    const int32_t time_font_size_pixels = font_size::to_pixels(time_font_size()*screen_fac, player_.main_window_ptr());
    video_time_size_.x = time_font_size_pixels*6;
    video_time_size_.y = time_font_size_pixels;

    // ---------------------------
    // --- Calculate positions ---
    // ---------------------------
    const float control_buttons_y_pos = buttons_relative_ypos()* render_geometry.size().height() + render_geometry.y();

    play_pause_btn_pos_.x = render_geometry.x() + render_geometry.size().width() / 2;
    play_pause_btn_pos_.y = control_buttons_y_pos;

    video_back_btn_pos_.x = render_geometry.x() + play_btns_size;
    video_back_btn_pos_.y = control_buttons_y_pos;
    video_fwd_btn_pos_.x = render_geometry.x() + render_geometry.size().width() - 1*play_btns_size;
    video_fwd_btn_pos_.y = control_buttons_y_pos;
    if (0.0f < btns_relative_x_dist && btns_relative_x_dist < 1.0f) {
        const auto x_dist_fac = 1.0f - btns_relative_x_dist;
        auto back_pos = video_back_btn_pos_.x + (play_pause_btn_pos_.x - video_back_btn_pos_.x)*x_dist_fac;
        auto fwd_pos = video_fwd_btn_pos_.x - (video_fwd_btn_pos_.x - play_pause_btn_pos_.x)*x_dist_fac;
        back_pos = std::clamp(back_pos, video_back_btn_pos_.x, play_pause_btn_pos_.x - play_buttons_window_size_.x );
        fwd_pos = std::clamp(fwd_pos, play_pause_btn_pos_.x + play_buttons_window_size_.x, video_fwd_btn_pos_.x );

        if (back_pos > video_back_btn_pos_.x) {
            video_back_btn_pos_.x = back_pos;
        }

        if (fwd_pos < video_fwd_btn_pos_.x) {
            video_fwd_btn_pos_.x = fwd_pos;
        }
    }

    video_slider_pos_.x = render_geometry.x() + render_geometry.size().width() / 2;
    video_slider_pos_.y = slider_relative_ypos() * (render_geometry.size().height() - video_slider_size_.y - 2*general_margin)  + render_geometry.y();


    elapsed_time_pos_.x = render_geometry.x() + video_time_size_.x / 2 + general_margin;
    elapsed_time_pos_.y = video_slider_pos_.y - video_slider_size_.y - general_margin;

    remaining_time_pos_.x = render_geometry.x() + render_geometry.width() - video_time_size_.x / 2 - general_margin;
    remaining_time_pos_.y = elapsed_time_pos_.y;

    menu_btn_pos_.x         = render_geometry.x() + menu_buttons_window_size_.x;
    menu_btn_pos_.y         = render_geometry.y() + 2*menu_buttons_window_size_.y;

    subtitles_btn_pos_.x    = render_geometry.x() + render_geometry.width() - 4*menu_buttons_window_size_.x;
    subtitles_btn_pos_.y    = render_geometry.y() + render_geometry.height() - 2*menu_buttons_window_size_.y;

    fullscreen_btn_pos_.x   = subtitles_btn_pos_.x - 2*menu_buttons_window_size_.x;
    fullscreen_btn_pos_.y   = subtitles_btn_pos_.y;

    // ------------------------
    // --- Calculate colors ---
    // ------------------------

    buttons_col_ = buttons_color();
    menu_close_button_color_ = menu_close_button_color();

    buttons_hover_col_ = buttons_col_*0.8f;
    buttons_active_col_ = buttons_col_*1.0f;
    time_text_col_ = time_text_color();

    // -----------------
    // --- Get fonts ---
    // -----------------
    font_slider_ = imgui_fonts::instance().get(slider_font_name(), slider_font_size_pixels);
    font_time_ = imgui_fonts::instance().get(time_font_name(), time_font_size_pixels);


    // ------------------------------------------
    // --- Stream state display calc geometry ---
    // ------------------------------------------
    stream_state_font_size_pixels_ = font_size::to_pixels(screen_fac*stream_state_font_size(), player_.main_window_ptr());
    font_stream_state_ = imgui_fonts::instance().get(stream_state_font_name(), stream_state_font_size_pixels_);


    const auto render_geometry_center = render_geometry.center();
    stream_state_pos_.x = render_geometry_center.x();
    stream_state_pos_.y = render_geometry_center.y();

}

void controls_default_platform::do_render()
{
    // @todo Fix: Just a safeguard, should not be needed!
    if (!font_time_) {
        do_calc_geometry();
    }

    update_prevent_hiding();
    render_menu_buttons();
    render_menu_window();
    if (!player_.is_playing()) {
        return;
    }
    render_player_controls();
    render_media_buttons();
    render_slider();
    render_player_time();
    render_subtitles_window();

    //    render_debug_window();
}

void controls_default_platform::do_render_stream_state()
{

    ///    auto window_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoInputs;
    auto window_flags = ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoInputs;

    bool visible = true;

    std::string stream_state_str;
    if (player_.is_waiting_for_io()) {
        const auto io_time_secs = duration_cast<seconds>(player_.current_io_operation_duration());
        stream_state_str = tr().tr("Waiting for data") + " " + std::to_string(io_time_secs.count()) + " s";
    }
    else {
        stream_state_str = to_string(player_.primary_stream_state());
    }

    stream_state_size_.x = (stream_state_font_size_pixels_ * static_cast<int32_t>(stream_state_str.size()))*0.5f;
    stream_state_size_.y = stream_state_font_size_pixels_ + 2*general_margin;

    ImGui::Rai imrai{};
    imrai.Font(font_stream_state_)
        .StyleColor(ImGuiCol_Border, {0,0,0,0})
        .StyleColor(ImGuiCol_WindowBg, {0,0,0,0.5})
        .StyleColor(ImGuiCol_Text, reinterpret_cast<const ImVec4&>(stream_state_color_))
        .StyleVar(ImGuiStyleVar_WindowMinSize, stream_state_size_)
        ;

    ImGui::SetNextWindowPos(stream_state_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ///    ImGui::SetNextWindowSize(stream_state_size_, ImGuiCond_::ImGuiCond_Always);

    ImGui::Begin("stream_state", &visible, window_flags);
    ImGui::TextCentered(stream_state_str.c_str());
    ImGui::End();
}

void controls_default_platform::render_player_controls()
{
    auto buttons_texture_ptr = control_icons_texture_->native_texture<void>();

    ImGui::Rai imrai;
    imrai.StyleVar(ImGuiStyleVar_WindowPadding, {0,0})
        // .StyleColor(ImGuiCol_WindowBg, {1,1,1,1}) // FIXMENM
        .StyleColor(ImGuiCol_WindowBg, {0,0,0,0})
        .StyleColor(ImGuiCol_Border, {0,0,0,0})
        .StyleColor(ImGuiCol_Button, {0,0,0,0})
        .StyleColor(ImGuiCol_ButtonActive, reinterpret_cast<const ImVec4&>(buttons_active_col_))
        .StyleColor(ImGuiCol_ButtonHovered, reinterpret_cast<const ImVec4&>(buttons_hover_col_))
        ;

    ImGui::SetNextWindowPos(play_pause_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ImGui::SetNextWindowSize(play_buttons_window_size_, ImGuiCond_::ImGuiCond_Always);


    ImGui::Begin("play_pause_btn", &visible_, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    set_cursor_pos_image_buttons();

    if (player_.playback_is_paused()) {
        if (ImGui::ImageButton("pause", buttons_texture_ptr, play_buttons_size_, pause_uv0.to_struct<ImVec2>(), pause_uv1.to_struct<ImVec2>(), {0,0,0,0}, {1,1,1,1})) {
            player_.resume_playback();
        }
    }
    else {
        if (ImGui::ImageButton("play", buttons_texture_ptr, play_buttons_size_, play_uv0.to_struct<ImVec2>(), play_uv1.to_struct<ImVec2>(), {0,0,0,0}, {1,1,1,1})) {
            player_.pause_playback();
        }
    }
    ImGui::End();

    ImGui::SetNextWindowPos(video_back_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ImGui::SetNextWindowSize(play_buttons_window_size_, ImGuiCond_::ImGuiCond_Always);
    ImGui::Begin("video_back_btn", &visible_, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    set_cursor_pos_image_buttons();

    ImGui::PushButtonRepeat(true);
    if (ImGui::ImageButton("backward", buttons_texture_ptr, play_buttons_size_, backward_uv0.to_struct<ImVec2>(), backward_uv1.to_struct<ImVec2>(), {0,0,0,0}, {1,1,1,1})) {
        player_.seek_relative(-config_.time_s("controls", "skip_time_small"));
    }
    ImGui::PopButtonRepeat();
    ImGui::End();

    ImGui::SetNextWindowPos(video_fwd_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ImGui::SetNextWindowSize(play_buttons_window_size_, ImGuiCond_::ImGuiCond_Always);
    ImGui::Begin("video_fwd_btn", &visible_, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    set_cursor_pos_image_buttons();

    ImGui::PushButtonRepeat(true);
    if (ImGui::ImageButton("forward", buttons_texture_ptr, play_buttons_size_, forward_uv0.to_struct<ImVec2>(), forward_uv1.to_struct<ImVec2>(), {0,0,0,0}, {1,1,1,1})) {
        player_.seek_relative(config_.time_s("controls", "skip_time_small"));
    }
    ImGui::PopButtonRepeat();
    ImGui::End();
}


void controls_default_platform::render_menu_buttons()
{
    auto buttons_texture_ptr = control_icons_texture_->native_texture<void>();

    ImGui::Rai imrai;
    imrai.StyleVar(ImGuiStyleVar_WindowPadding, {0,0})
        // .StyleColor(ImGuiCol_WindowBg, {1,1,1,1}) // FIXMENM
        .StyleColor(ImGuiCol_WindowBg, {0,0,0,0})
        .StyleColor(ImGuiCol_Border, {0,0,0,0})
        .StyleColor(ImGuiCol_Button, {0,0,0,0})
        .StyleColor(ImGuiCol_ButtonActive, reinterpret_cast<const ImVec4&>(buttons_active_col_))
        .StyleColor(ImGuiCol_ButtonHovered, reinterpret_cast<const ImVec4&>(buttons_hover_col_))
        ;

    // -------------------
    // --- Menu button ---
    // -------------------
    ImGui::SetNextWindowPos(menu_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ImGui::SetNextWindowSize(menu_buttons_window_size_, ImGuiCond_::ImGuiCond_Always);
    ImGui::Begin("###menu_btn_win", &visible_, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    set_cursor_pos_image_buttons();

    if (ImGui::ImageButton("###menu_btn", buttons_texture_ptr, menu_buttons_size_, menu_uv0.to_struct<ImVec2>(), menu_uv1.to_struct<ImVec2>(), {0,0,0,0}, {1,1,1,1})) {
        do_render_menu_window_ = true;
    }
    ImGui::End();

    // -------------------------
    // --- Fullscreen button ---
    // -------------------------
    ImGui::SetNextWindowPos(fullscreen_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ImGui::SetNextWindowSize(menu_buttons_window_size_, ImGuiCond_::ImGuiCond_Always);
    ImGui::Begin("###fullscreen_btn_win", &visible_, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    set_cursor_pos_image_buttons();

    if (ImGui::ImageButton("###fullscreen_btn", buttons_texture_ptr, menu_buttons_size_, fullscreen_uv0.to_struct<ImVec2>(), fullscreen_uv1.to_struct<ImVec2>(), {0,0,0,0}, {1,1,1,1})) {
        std::cerr << "Toggle fullscreen\n";
        player_.toggle_full_screen();
    }
    ImGui::End();

}

void controls_default_platform::render_media_buttons()
{
    auto buttons_texture_ptr = control_icons_texture_->native_texture<void>();

    ImGui::Rai imrai;
    imrai.StyleVar(ImGuiStyleVar_WindowPadding, {0,0})
                                                        // .StyleColor(ImGuiCol_WindowBg, {1,1,1,1}) // FIXMENM
        .StyleColor(ImGuiCol_WindowBg, {0,0,0,0})
        .StyleColor(ImGuiCol_Border, {0,0,0,0})
        .StyleColor(ImGuiCol_Button, {0,0,0,0})
        .StyleColor(ImGuiCol_ButtonActive, reinterpret_cast<const ImVec4&>(buttons_active_col_))
        .StyleColor(ImGuiCol_ButtonHovered, reinterpret_cast<const ImVec4&>(buttons_hover_col_))
        ;

    // ------------------------
    // --- Subtitles button ---
    // ------------------------
    ImGui::SetNextWindowPos(subtitles_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ImGui::SetNextWindowSize(menu_buttons_window_size_, ImGuiCond_::ImGuiCond_Always);

    ImGui::Begin("###subtitles_btn_win", &visible_, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    set_cursor_pos_image_buttons();

    bool cur_show_subtitles = config_.bool_val("subtitles", "show");
    const auto subtitles_uv0 = cur_show_subtitles ? subtitles_on_uv0.to_struct<ImVec2>() : subtitles_off_uv0.to_struct<ImVec2>();
    const auto subtitles_uv1 = cur_show_subtitles ? subtitles_on_uv1.to_struct<ImVec2>() : subtitles_off_uv1.to_struct<ImVec2>();

    // Simple selection popup (if you want to show the current selection inside the Button itself,
    // you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
    if (ImGui::ImageButton("###subtitles_btn", buttons_texture_ptr, menu_buttons_size_, subtitles_uv0, subtitles_uv1, {0,0,0,0}, {1,1,1,1})) {
        do_render_subtitles_window_ = true;
    }
    ImGui::End();
}

void controls_default_platform::render_slider()
{
    const float total_time_seconds = player_.total_time().count() / 1'000'000;
    const float save_current_postion_seconds = player_.current_time().count() / 1'000'000;
    float current_postion_seconds = save_current_postion_seconds;

    ImGui::Rai imrai{};
    imrai.Font(font_slider_)
        .StyleVar(ImGuiStyleVar_WindowPadding, {0,0})
        .StyleVar(ImGuiStyleVar_GrabMinSize, video_slider_grab_width_)
        .StyleVar(ImGuiStyleVar_GrabRounding, video_slider_grab_width_/4)
        .StyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(4, 4))
        ;

    ImGui::SetNextWindowPos(video_slider_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ImGui::SetNextWindowSize(video_slider_size_, ImGuiCond_::ImGuiCond_Always);
    ImGui::Begin("video_slider_win", &visible_, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    ImGui::PushItemWidth(-1);   // Force control to fill width with no label

    if (ImGui::SliderFloat("video_slider", &current_postion_seconds, 0, total_time_seconds, ""))
    {
        slider_last_user_pos_seconds_ = current_postion_seconds;
    }
    else {
        //            if (std::abs(slider_last_user_pos_seconds_ - current_postion_seconds) > 5) {
        //                slider_last_user_pos_seconds_ = current_postion_seconds;
        //            }

    }
    ImGui::PopItemWidth();
    ImGui::End();

    const auto diff_in_seconds = current_postion_seconds - save_current_postion_seconds;
    if (std::abs(diff_in_seconds) > 3) {
        player_.seek_position(seconds(static_cast<uint32_t>(current_postion_seconds)));
    }
}

void controls_default_platform::render_player_time()
{
    const auto elapsed_time = format_h_m_s(player_.current_time());
    const auto remaining_time = format_h_m_s(player_.remaining_time());

    ImGui::Rai imrai{};
    imrai.Font(font_time_)
        .StyleColor(ImGuiCol_Border, {0,0,0,0})
        .StyleColor(ImGuiCol_WindowBg, {0,0,0,0})
        .StyleColor(ImGuiCol_Text, reinterpret_cast<const ImVec4&>(time_text_col_))
        .StyleVar(ImGuiStyleVar_WindowMinSize, video_time_size_)
        ;

    ImGui::SetNextWindowPos(elapsed_time_pos_, ImGuiCond_::ImGuiCond_Always, {0.35, 0.5} );
    ImGui::SetNextWindowSize(video_time_size_, ImGuiCond_::ImGuiCond_Always);

    ///ImGui::Begin("elapsed_time", &show_controls, ImGuiWindowFlags_NoBackground |ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    ImGui::Begin("elapsed_time", &visible_, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetCursorPosX(general_margin);
    ImGui::SetCursorPosY(0);
    ImGui::TextUnformatted(elapsed_time);
    ImGui::End();

    ImGui::SetNextWindowPos(remaining_time_pos_, ImGuiCond_::ImGuiCond_Always, {0.45, 0.5} );
    ImGui::SetNextWindowSize(video_time_size_, ImGuiCond_::ImGuiCond_Always);
    ImGui::Begin("remaining_time", &visible_, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetCursorPosX(general_margin);
    ImGui::SetCursorPosY(0);

    ImGui::TextUnformatted(remaining_time);
    ImGui::End();
}

void controls_default_platform::render_menu_window()
{
    if (!do_render_menu_window_) {
        new_user_media_file_.clear();
        return;
    }
    const float buttons_relative_width = 0.7f;
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);     // viewport->WorkPos/Pos
    ImGui::SetNextWindowSize(viewport->Size);   // viewport->WorkSize/Size


    if (ImGui::Begin("Main Player Menu", &do_render_menu_window_, flags))
    {
        if (ImGui::CenteredButton(tr().tr("Open Local Media"), buttons_relative_width)) {
            IGFD::FileDialogConfig config;
            config.path = cpaf::filesystem::special_dirs::home();

            auto filter = tr().tr("Video files") + "{.avi,.mkv,.mp4,.mov}" + ",All Files (*.*){.*}";
            filter += "," + tr().tr("Audio files") + "{.flac,.mp3,.wav}";
            filter += "," + tr().tr("All Files") +  " (*.*){.*}";
            ImGuiFileDialog::Instance()->OpenDialog("ChooseMediaFileDlgKey", tr().tr("Choose File"), filter.c_str(), config);
        }
        if (ImGui::CenteredButton(tr().tr("Open network Stream"), buttons_relative_width)) {
            ImGui::OpenPopup("###ChooseMediaNetworkStreamDlg");
        }

        ImGui::Separator();

        {
            ImGui::Rai imrai = ImGui::Rai::Create().ButtonColorAuto(reinterpret_cast<const ImVec4&>(menu_close_button_color_));
//            imrai.ButtonColorAuto(reinterpret_cast<const ImVec4&>(menu_close_button_color_))
//            ;

            if (ImGui::CenteredButton(tr().tr("Close this window"), buttons_relative_width)) {
                do_render_menu_window_ = false;
            }
        }
    }
    // -----------------------------------------------
    // --- Open local media file dialog definition ---
    // -----------------------------------------------
    ImGui::SetNextWindowPosRelative({0.5f, 0.1f}, ImGuiCond_Appearing, {0.5,0});
    ImGui::SetNextWindowSizeRelative({0.9, 0.9}, ImGuiCond_Appearing);
    if (ImGuiFileDialog::Instance()->Display("ChooseMediaFileDlgKey", ImGuiWindowFlags_NoCollapse, file_dialog_min_size_ )) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            do_render_menu_window_ = false;
            player_.open_media(ImGuiFileDialog::Instance()->GetFilePathName());
        }
        ImGuiFileDialog::Instance()->Close();
    }

    // ---------------------------------------------------
    // --- Open media network stream dialog definition ---
    // ---------------------------------------------------
    ImGui::SetNextWindowPosRelative({0.5f, 0.1f}, ImGuiCond_Appearing, {0.5,0.5});
    ImGui::SetNextWindowSizeRelative({0.8f, 0}, ImGuiCond_Appearing);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));  // Resize Horizontal only

    if (ImGui::BeginPopupModal((tr().tr("Open network stream") + "###ChooseMediaNetworkStreamDlg").c_str(), nullptr))
    {
        ImGui::PushItemWidth(-1);   // Force control to fill width with no label
        ImGui::InputTextWithHint("###open_stream_input_text", tr().tr("Open network URL https://, magnet:, ftp://"), new_user_media_file_);
        ImGui::PopItemWidth();

        if (ImGui::AlignedButton(tr().tr("Ok"), 0.2, 0.3)) {
            ImGui::CloseCurrentPopup();
            do_render_menu_window_ = false;
            player_.open_media(new_user_media_file_);
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::AlignedButton(tr().tr("Cancel"), 0.8, 0.3)) {
            new_user_media_file_.clear();
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

void controls_default_platform::render_subtitles_window()
{
    if (!do_render_subtitles_window_) {
        new_user_subtitle_file_ = player_.subtitle_user();
        new_subtitle_select_index_ = player_.subtitle_selected_index();
        return;
    }
    static constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    if (ImGui::Begin("subtitles_window", &do_render_menu_window_, flags))
    {
        player_.ui_window_active_set(true);
        if (ImGui::InputTextWithHint("###user_subtitle_input", tr().tr("User subtitle: URL/local .srt/.zip file"), new_user_subtitle_file_)) {
            new_subtitle_select_index_ = 0;
        }

        ImGui::SameLine();
        if (ImGui::Button(tr().tr("Open subtitle file"))) {
            IGFD::FileDialogConfig config;
            config.path = cpaf::filesystem::special_dirs::home();
            const auto filter = tr().tr("Subtitle files") + "{.srt,.zip}" "," + tr().tr("All Files") +  " (*.*){.*}";
            ImGuiFileDialog::Instance()->OpenDialog("ChooseSubtitleFileDlgKey", tr().tr("Choose File"), filter.c_str(), config);
        }

        ImGui::SameLine();
        ImGui::HelpMarker(tr().tr("Select a subtitle file either from your local disk [Open subtitle file] button or by entering an URL in the field."));

        const auto& subtitles = player_.selectable_subtitles();

        ImGui::RadioButton(tr().tr("Subtitles disabled"), new_subtitle_select_index_, -1);
        for (auto i = 0u; i < subtitles.size(); ++i) {
            const std::string selelectable_text = subtitles[i].language_name + "###LangSel" + std::to_string(i);
            ImGui::RadioButton(selelectable_text, new_subtitle_select_index_, static_cast<int32_t>(i));
        }

        if (ImGui::Button(tr().tr("Apply")) ) {
            player_.set_subtitle_user(new_user_subtitle_file_);
            player_.subtitle_select(new_subtitle_select_index_);
            player_.ui_window_active_set(false);
            do_render_subtitles_window_ = false;
        }

    }
    // --------------------------------------------
    // --- Open subtitle file dialog definition ---
    // --------------------------------------------
    ImGui::SetNextWindowPosRelative({0.5f, 0.1f}, ImGuiCond_Appearing, {0.5,0});
    ImGui::SetNextWindowSizeRelative({0.9, 0.9}, ImGuiCond_Appearing);
    if (ImGuiFileDialog::Instance()->Display("ChooseSubtitleFileDlgKey", ImGuiWindowFlags_NoCollapse, file_dialog_min_size_ )) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            new_user_subtitle_file_ = ImGuiFileDialog::Instance()->GetFilePathName();
            new_subtitle_select_index_ = 0;
        }
        ImGuiFileDialog::Instance()->Close();
    }

    ImGui::End();

}

void controls_default_platform::render_volume_popup()
{
    std::cerr << "FIXMENM render_volume_popup()\n";
}

void controls_default_platform::render_debug_window()
{
    ImGui::Rai imrai;
    imrai.StyleColor(ImGuiCol_Text, {0, 1, 0, 1})
        .StyleColor(ImGuiCol_Button, {0, 0, 0, 0})
        .StyleColor(ImGuiCol_ButtonActive, {0, 0, 0, 0})
        .StyleColor(ImGuiCol_ButtonHovered, {0, 0, 0, 0})
        ;



//    auto display_size = player_.main_window_ptr()->display_size();
//    auto render_size = player_.render_geometry().size();
    auto screen_size_fac = player_.screen_size_factor_use();
    ImGui::Begin("Video player default_controls debug");
    ImGui::Text("stream_state: %s", to_string(player_.primary_stream_state()).c_str()  );

    ImGui::Text("screen_size_fac = %f x %f", screen_size_fac.width(), screen_size_fac.height());
//    ImGui::Text("render_size = %f x %f", render_size.width(), render_size.height());
//    ImGui::Text("video_slider_pos_ [ %f , %f ]", video_slider_pos_.x, video_slider_pos_.y);
//    ImGui::Text("video_slider_size_ ( %f x %f )", video_slider_size_.x, video_slider_size_.y);
    ImGui::End();

}

void controls_default_platform::set_cursor_pos_image_buttons()
{
    auto cursor_pos_x = -ImGui::GetStyle().ItemSpacing.x/2 + image_buttons_window_size_extra;
    auto cursor_pos_y = -ImGui::GetStyle().ItemSpacing.y/2 + image_buttons_window_size_extra;
    ImGui::SetCursorPosX(cursor_pos_x);
    ImGui::SetCursorPosY(cursor_pos_y);
}

void controls_default_platform::update_prevent_hiding()
{
    prevent_hiding(do_render_subtitles_window_ || do_render_menu_window_);
}

} // namespace cpaf::gui::video

