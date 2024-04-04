#include "controls_default.h"

#include <cpaf_libs/time/cpaf_time.h>
#include <cpaf_libs/gui/fonts/font_size.h>
#include <cpaf_libs/gui/assets/fonts/imgui_fonts.h>
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/video/player.h>
#include <cpaf_libs/gui/video/ui/assets/video_player_data.h>

// FIXMENM REMOVE this again!!
#include <IconFontCppHeaders/IconsFontAwesome6.h>


using namespace cpaf::time;
using namespace std::chrono;

namespace cpaf::gui::video {

using pos_2df = cpaf::gui::pos_2df;

constexpr float icon_width = 0.25f;
constexpr pos_2df wh_add = {icon_width, icon_width};

constexpr pos_2df forward_uv0       = {3*icon_width, 0*icon_width};  constexpr pos_2df forward_uv1  = forward_uv0   + wh_add;
constexpr pos_2df pause_uv0         = {1*icon_width, 2*icon_width};  constexpr pos_2df pause_uv1    = pause_uv0     + wh_add;
constexpr pos_2df play_uv0          = {1*icon_width, 3*icon_width};  constexpr pos_2df play_uv1     = play_uv0      + wh_add;
constexpr pos_2df backward_uv0      = {0*icon_width, 3*icon_width};  constexpr pos_2df backward_uv1 = backward_uv0  + wh_add;

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
    const int32_t fwd_back_font_size_pixels = font_size::to_pixels(buttons_size(), player_.main_window_ptr());
    const int32_t slider_font_size_pixels = font_size::to_pixels(slider_height(), player_.main_window_ptr());
    const int32_t time_font_size_pixels = font_size::to_pixels(time_font_size(), player_.main_window_ptr());

//    const ImFont* font = imgui_fonts::instance().get(font_name_, font_size_pixels, subtitles_create_dist_);

    const float y_pos = relative_ypos()* render_geometry.size().height();

    play_pause_btn_pos_.x = render_geometry.size().width() / 2;
    play_pause_btn_pos_.y = y_pos;

    video_back_btn_pos_.x = fwd_back_font_size_pixels;
    video_back_btn_pos_.y = y_pos;
    video_fwd_btn_pos_.x = render_geometry.size().width() - 1*fwd_back_font_size_pixels;
    video_fwd_btn_pos_.y = y_pos;

    video_slider_pos_.x = render_geometry.size().width() / 2;
    video_slider_pos_.y = y_pos + 1.5*fwd_back_font_size_pixels;

    video_slider_size_.x = render_geometry.size().width() - 0.8*fwd_back_font_size_pixels;
    video_slider_grab_width_ = fwd_back_font_size_pixels / 2;

    elapsed_time_pos_.x = (render_geometry.size().width() - video_slider_size_.x);
    elapsed_time_pos_.y = video_slider_pos_.y - slider_font_size_pixels*1 - time_font_size_pixels*1;

    remaining_time_pos_.x = video_fwd_btn_pos_.x;
    remaining_time_pos_.y = elapsed_time_pos_.y;


    const auto play_buttons_size = buttons_size();
    play_buttons_size_ = ImVec2{play_buttons_size, play_buttons_size};

    buttons_text_col_ = buttons_text_color();
    buttons_col_ = buttons_color();

    buttons_hover_col_ = buttons_col_*0.8f;
    buttons_active_col_ = buttons_col_*1.0f;

}

void controls_default::do_render()
{
//    do_calc_geometry();    // TODO : Only calc new geometry when window or fonts/sizes have changed!

    const color time_text_col = time_text_color();
    const int32_t play_buttons_font_size_pixels = font_size::to_pixels(buttons_size(), player_.main_window_ptr());
    const int32_t slider_font_size_pixels = font_size::to_pixels(slider_height(), player_.main_window_ptr());
    const int32_t time_font_size_pixels = font_size::to_pixels(time_font_size(), player_.main_window_ptr());
    ImFont* font_fwd_back_btns = imgui_fonts::instance().get(buttons_font_name(), play_buttons_font_size_pixels);
    ImFont* font_slider = imgui_fonts::instance().get(slider_font_name(), slider_font_size_pixels);
    ImFont* font_time = imgui_fonts::instance().get(time_font_name(), time_font_size_pixels);

    auto buttons_texture_ptr = control_icons_texture_->native_texture<void>();

    bool show_controls = true;

    {
        ImGui::Rai imrai;
        imrai.Font(font_fwd_back_btns)
            .StyleColor(ImGuiCol_Text, reinterpret_cast<const ImVec4&>(buttons_text_col_))
            .StyleColor(ImGuiCol_WindowBg, {0,0,0,0})
            .StyleColor(ImGuiCol_Border, {0,0,0,0})
            .StyleColor(ImGuiCol_Button, {0,0,0,0})
            .StyleColor(ImGuiCol_ButtonActive, reinterpret_cast<const ImVec4&>(buttons_active_col_))
            .StyleColor(ImGuiCol_ButtonHovered, reinterpret_cast<const ImVec4&>(buttons_hover_col_))
            ;

        ImGui::SetNextWindowPos(play_pause_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        ImGui::Begin("play_pause_btn", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);

        const char* const btn = player_.playback_is_paused() ? ICON_FA_CIRCLE_PAUSE : ICON_FA_CIRCLE_PLAY;
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
//        if (ImGui::Button(btn)) {
//            player_.toggle_pause_playback();
//        }

//        const char* const btn = player_.playback_is_paused() ? ICON_FA_CIRCLE_PAUSE : ICON_FA_CIRCLE_PLAY;
//        if (ImGui::Button(btn)) {
//            player_.toggle_pause_playback();
//        }
        ImGui::End();

        ImGui::SetNextWindowPos(video_back_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        ImGui::Begin("video_back_btn", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        ImGui::PushButtonRepeat(true);
        if (ImGui::ImageButton("backward", buttons_texture_ptr, play_buttons_size_, backward_uv0.to_struct<ImVec2>(), backward_uv1.to_struct<ImVec2>(), {0,0,0,0}, {1,1,1,1})) {
            player_.seek_relative(-config_.controls_seconds("skip_time_small"));
        }
        ImGui::PopButtonRepeat();
        ImGui::End();

        ImGui::SetNextWindowPos(video_fwd_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        ImGui::Begin("video_fwd_btn", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        ImGui::PushButtonRepeat(true);
        if (ImGui::ImageButton("forward", buttons_texture_ptr, play_buttons_size_, forward_uv0.to_struct<ImVec2>(), forward_uv1.to_struct<ImVec2>(), {0,0,0,0}, {1,1,1,1})) {
            player_.seek_relative(config_.controls_seconds("skip_time_small"));
        }
//        if (ImGui::Button(ICON_FA_ARROW_ROTATE_RIGHT)) {
//            player_.seek_relative(config_.controls_seconds("skip_time_small"));
//        }
        ImGui::PopButtonRepeat();
        ImGui::End();
    }

    {
        const float total_time_seconds = player_.total_time().count() / 1'000'000;
        const float save_current_postion_seconds = player_.current_time().count() / 1'000'000;
        float current_postion_seconds = save_current_postion_seconds;
//        if (player_.playback_paused()) {
//            current_postion_seconds = slider_last_user_pos_seconds_;
//        }

        ImGui::Rai imrai{};
        imrai.Font(font_slider)
//  Test/Debug only
//            .StyleColor(ImGuiCol_Border, {1,1,1,1})
//            .StyleColor(ImGuiCol_WindowBg, {0,1,0,0.5})
            .StyleVar(ImGuiStyleVar_WindowPadding, {0,0})
            .StyleVar(ImGuiStyleVar_GrabMinSize, video_slider_grab_width_)
            .StyleVar(ImGuiStyleVar_GrabRounding, video_slider_grab_width_/4)
            .StyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(4, 4))
            ;

        ImGui::SetNextWindowPos(video_slider_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        ImGui::SetNextWindowSize(video_slider_size_, ImGuiCond_::ImGuiCond_Always);
        ImGui::Begin("video_slider_win", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        ImGui::PushItemWidth(-1);   // Force control to fill width with no label

        if (ImGui::SliderFloat("video_slider", &current_postion_seconds, 0, total_time_seconds, ""))
        {
//            std::cerr << "Moving slider....\n";
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
        if (std::abs(diff_in_seconds) > 5) {
            player_.seek_position(seconds(static_cast<uint32_t>(current_postion_seconds)));
        }

    }


    {

        const auto elapsed_time = format_h_m_s(player_.current_time());
        const auto remaining_time = format_h_m_s(player_.remaining_time());

        ImGui::Rai imrai{};
        imrai.Font(font_time)
             .StyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(4, 4))
            ;

        ImGui::SetNextWindowPos(elapsed_time_pos_, ImGuiCond_::ImGuiCond_Always, {0.35, 0.5} );
//        ImGui::Begin("elapsed_time", &show_controls, ImGuiWindowFlags_NoBackground |ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        ImGui::Begin("elapsed_time", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        ImGui::TextUnformatted(elapsed_time);
        ImGui::End();

        ImGui::SetNextWindowPos(remaining_time_pos_, ImGuiCond_::ImGuiCond_Always, {0.45, 0.5} );
        ImGui::Begin("remaining_time", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        ImGui::TextUnformatted(remaining_time);
        ImGui::End();
    }

}


} // namespace cpaf::gui::video

