#include "controls_default.h"

#include <cpaf_libs/time/cpaf_time.h>
#include <cpaf_libs/gui/fonts/font_size.h>
#include <cpaf_libs/gui/assets/fonts/imgui_fonts.h>
#include <cpaf_libs/gui/video/player.h>

using namespace cpaf::time;
using namespace std::chrono;

namespace cpaf::gui::video {



void controls_default::calc_geometry()
{
    const auto render_geometry = player_.render_geometry();
    const int32_t fwd_back_font_size_pixels = font_size::to_pixels(play_buttons_font_size_points_, player_.main_window_ptr());
    const int32_t slider_font_size_pixels = font_size::to_pixels(slider_font_size_points_, player_.main_window_ptr());
    const int32_t time_font_size_pixels = font_size::to_pixels(time_font_size_points_, player_.main_window_ptr());

//    const ImFont* font = imgui_fonts::instance().get(font_name_, font_size_pixels, subtitles_create_dist_);

    const float y_pos = relative_ypos_* render_geometry.size.height();

    play_pause_btn_pos_.x = render_geometry.size.width() / 2;
    play_pause_btn_pos_.y = y_pos;

    video_back_btn_pos_.x = fwd_back_font_size_pixels;
    video_back_btn_pos_.y = y_pos;
    video_fwd_btn_pos_.x = render_geometry.size.width() - 1*fwd_back_font_size_pixels;
    video_fwd_btn_pos_.y = y_pos;

    video_slider_pos_.x = render_geometry.size.width() / 2;
    video_slider_pos_.y = y_pos + 1.5*fwd_back_font_size_pixels;

    video_slider_size_.x = render_geometry.size.width() - 0.8*fwd_back_font_size_pixels;
    video_slider_grab_width_ = fwd_back_font_size_pixels / 2;

    elapsed_time_pos_.x = (render_geometry.size.width() - video_slider_size_.x);
    elapsed_time_pos_.y = video_slider_pos_.y + slider_font_size_pixels + time_font_size_pixels;

    remaining_time_pos_.x = video_fwd_btn_pos_.x;
    remaining_time_pos_.y = elapsed_time_pos_.y;
}

void controls_default::do_render()
{
    calc_geometry();    // TODO : Only calc new geometry when window or fonts/sizes have changed!

    const int32_t play_buttons_font_size_pixels = font_size::to_pixels(play_buttons_font_size_points_, player_.main_window_ptr());
    const int32_t slider_font_size_pixels = font_size::to_pixels(slider_font_size_points_, player_.main_window_ptr());
    const int32_t time_font_size_pixels = font_size::to_pixels(time_font_size_points_, player_.main_window_ptr());
    ImFont* font_fwd_back_btns = imgui_fonts::instance().get(font_name_, play_buttons_font_size_pixels);
    ImFont* font_slider = imgui_fonts::instance().get(font_name_, slider_font_size_pixels);
    ImFont* font_time = imgui_fonts::instance().get(font_name_, time_font_size_pixels);

    ///if (!font_fwd_back_btns) { return; }
    bool show_controls = true;

    {
        ImGui::Rai imrai;
        imrai.Font(font_fwd_back_btns)
            .StyleVar(ImGuiStyleVar_WindowPadding, {0,0})
            .StyleVar(ImGuiStyleVar_WindowRounding, play_buttons_font_size_pixels/2)
            .StyleVar(ImGuiStyleVar_FrameRounding, play_buttons_font_size_pixels/2)
            ;

        ImGui::SetNextWindowPos(play_pause_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        ImGui::Begin("play_pause_btn", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        if (ImGui::ArrowButton("##play_pause", ImGuiDir_Right)) {
            player_.toggle_pause_playback();
        }
        ImGui::End();

        ImGui::SetNextWindowPos(video_back_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        ImGui::Begin("video_back_btn", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("<<")) {
            player_.seek_relative(--relative_skip_time_small_);
        }
        ImGui::PopButtonRepeat();
        ImGui::End();

        ImGui::SetNextWindowPos(video_fwd_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        ImGui::Begin("video_fwd_btn", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        ImGui::PushButtonRepeat(true);
        if (ImGui::Button(">>")) {
            player_.seek_relative(relative_skip_time_small_);
        }
        ImGui::PopButtonRepeat();
        ImGui::End();
    }

    {
        const float total_time_seconds = duration_cast<seconds>(player_.total_time()).count();
        const float save_current_postion_seconds = duration_cast<seconds>(player_.current_time()).count();
        float current_postion_seconds = save_current_postion_seconds;
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
        ImGui::SliderFloat("video_slider", &current_postion_seconds, 0, total_time_seconds, "");
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

