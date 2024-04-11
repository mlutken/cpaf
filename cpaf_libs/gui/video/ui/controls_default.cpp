#include "controls_default.h"

#include <cpaf_libs/time/cpaf_time.h>
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

/// @see https://github.com/ocornut/imgui/issues/4216
/// @see https://html-color.codes/




controls_default::controls_default(player& parent_player, config& cfg)
    : controls(parent_player, cfg)
{
    control_icons_texture_ = std::make_unique<cpaf::gui::texture>(parent_player.main_window_ptr()->renderer_shared());
    control_icons_texture_ ->load_png_from_memory(cpaf::gui::video_player_data::video_player_control_icons());
}

void controls_default::do_calc_geometry()
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

    const float menu_buttons_y_pos = video_slider_pos_.y - menu_buttons_window_size_.y;

    menu_btn_pos_.x         = render_geometry.center().x() - menu_buttons_window_size_.x;
    subtitles_btn_pos_.x    = menu_btn_pos_.x + 2*menu_buttons_window_size_.x;

    menu_btn_pos_.y = menu_buttons_y_pos;
    subtitles_btn_pos_.y = menu_buttons_y_pos;

    // ------------------------
    // --- Calculate colors ---
    // ------------------------

    buttons_col_ = buttons_color();

    buttons_hover_col_ = buttons_col_*0.8f;
    buttons_active_col_ = buttons_col_*1.0f;
    time_text_col_ = time_text_color();

    // -----------------
    // --- Get fonts ---
    // -----------------
    font_slider_ = imgui_fonts::instance().get(slider_font_name(), slider_font_size_pixels);
    font_time_ = imgui_fonts::instance().get(time_font_name(), time_font_size_pixels);
}

void controls_default::do_render()
{
    // @todo Fix: Just a safeguard, should not be needed!
    if (!font_time_) {
        do_calc_geometry();
    }


    render_player_controls();
    render_menu_buttons();
    render_slider();
    render_player_time();
//    render_debug_window();
    render_subtitles_popup();
}

void controls_default::render_player_controls()
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
        player_.seek_relative(-config_.seconds("controls", "skip_time_small"));
    }
    ImGui::PopButtonRepeat();
    ImGui::End();

    ImGui::SetNextWindowPos(video_fwd_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ImGui::SetNextWindowSize(play_buttons_window_size_, ImGuiCond_::ImGuiCond_Always);
    ImGui::Begin("video_fwd_btn", &visible_, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    set_cursor_pos_image_buttons();

    ImGui::PushButtonRepeat(true);
    if (ImGui::ImageButton("forward", buttons_texture_ptr, play_buttons_size_, forward_uv0.to_struct<ImVec2>(), forward_uv1.to_struct<ImVec2>(), {0,0,0,0}, {1,1,1,1})) {
        player_.seek_relative(config_.seconds("controls", "skip_time_small"));
    }
    ImGui::PopButtonRepeat();
    ImGui::End();
}

void controls_default::render_menu_buttons()
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

    // --- Subtitles button ---
    ImGui::SetNextWindowPos(subtitles_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ImGui::SetNextWindowSize(menu_buttons_window_size_, ImGuiCond_::ImGuiCond_Always);

    ImGui::Begin("subtitles_btn", &visible_, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    set_cursor_pos_image_buttons();

    // ---- FIXMENM DEBUG ONLY BEGIN ----
    static int selected_fish = -1;
    const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
    static bool toggles[] = { true, false, false, false, false };

    // Simple selection popup (if you want to show the current selection inside the Button itself,
    // you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
    if (ImGui::ImageButton("subtitles_btn", buttons_texture_ptr, menu_buttons_size_, subtitles_on_uv0.to_struct<ImVec2>(), subtitles_on_uv1.to_struct<ImVec2>(), {0,0,0,0}, {1,1,1,1})) {
        ImGui::OpenPopup("subtitles_popup_menu");
    }
///    ImGui::SameLine();
///    ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
    if (ImGui::BeginPopup("subtitles_popup_menu"))
    {
        ImGui::Text("My Aquarium");
        ImGui::Separator();
        for (int i = 0; i < IM_ARRAYSIZE(names); i++)
            if (ImGui::Selectable(names[i])) {
                selected_fish = i;
            }
        ImGui::EndPopup();
    }

    // ---- FIXMENM DEBUG ONLY END ----


//    if (ImGui::ImageButton("subtitles", buttons_texture_ptr, menu_buttons_size_, subtitles_on_uv0.to_struct<ImVec2>(), subtitles_on_uv1.to_struct<ImVec2>(), {0,0,0,0}, {1,1,1,1})) {
//        std::cerr << "FIXMENM try to open popup\n";
//        ImGui::OpenPopup("subtitles_popup");;
//    }
    ImGui::End();

    // --- Menu button ---
    ImGui::SetNextWindowPos(menu_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ImGui::SetNextWindowSize(menu_buttons_window_size_, ImGuiCond_::ImGuiCond_Always);
    ImGui::Begin("menu_btn", &visible_, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    set_cursor_pos_image_buttons();

    if (ImGui::ImageButton("menu", buttons_texture_ptr, menu_buttons_size_, menu_uv0.to_struct<ImVec2>(), menu_uv1.to_struct<ImVec2>(), {0,0,0,0}, {1,1,1,1})) {
        render_menu_popup();
    }
    ImGui::End();


}

void controls_default::render_slider()
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
    //std::cerr << "FIXMENM diff slider: " << diff_in_seconds << "\n";
    if (std::abs(diff_in_seconds) > 3) {
        player_.seek_position(seconds(static_cast<uint32_t>(current_postion_seconds)));
    }
}

void controls_default::render_player_time()
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
    //        ImGui::Begin("elapsed_time", &show_controls, ImGuiWindowFlags_NoBackground |ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);

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

void controls_default::render_subtitles_popup()
{
//    std::cerr << "FIXMENM render_subtitles_popup()\n";
    if (ImGui::BeginPopupModal("subtitles_popup"))
    {
        ImGui::Text("Lorem ipsum");
        ImGui::EndPopup();
    }


}

void controls_default::render_menu_popup()
{
    std::cerr << "FIXMENM render_menu_popup()\n";
}

void controls_default::render_volume_popup()
{
    std::cerr << "FIXMENM render_volume_popup()\n";
}

void controls_default::render_debug_window()
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
    ImGui::Text("stream_state: %s", to_string(player_.stream_state()).c_str()  );

    ImGui::Text("screen_size_fac = %f x %f", screen_size_fac.width(), screen_size_fac.height());
//    ImGui::Text("render_size = %f x %f", render_size.width(), render_size.height());
//    ImGui::Text("video_slider_pos_ [ %f , %f ]", video_slider_pos_.x, video_slider_pos_.y);
//    ImGui::Text("video_slider_size_ ( %f x %f )", video_slider_size_.x, video_slider_size_.y);
    ImGui::End();

}

void controls_default::set_cursor_pos_image_buttons()
{
    auto cursor_pos_x = -ImGui::GetStyle().ItemSpacing.x/2 + image_buttons_window_size_extra;
    auto cursor_pos_y = -ImGui::GetStyle().ItemSpacing.y/2 + image_buttons_window_size_extra;
    ImGui::SetCursorPosX(cursor_pos_x);
    ImGui::SetCursorPosY(cursor_pos_y);
}


} // namespace cpaf::gui::video

