#include "controls_default.h"

#include <cpaf_libs/gui/fonts/font_size.h>
#include <cpaf_libs/gui/assets/fonts/imgui_fonts.h>
#include <cpaf_libs/gui/video/player.h>
namespace cpaf::gui::video {



void controls_default::calc_geometry()
{
    const auto render_geometry = player_.render_geometry();
    const int32_t fwd_back_font_size_pixels = font_size::to_pixels(fwd_back_font_size_points_, player_.main_window_ptr());
    const int32_t slider_font_size_pixels = font_size::to_pixels(slider_font_size_points_, player_.main_window_ptr());
    const int32_t time_font_size_pixels = font_size::to_pixels(time_font_size_points_, player_.main_window_ptr());

//    const ImFont* font = imgui_fonts::instance().get(font_name_, font_size_pixels, subtitles_create_dist_);

    const float y_pos = relative_ypos_* render_geometry.size.height();

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

    const int32_t fwd_back_font_size_pixels = font_size::to_pixels(fwd_back_font_size_points_, player_.main_window_ptr());
    const int32_t slider_font_size_pixels = font_size::to_pixels(slider_font_size_points_, player_.main_window_ptr());
    const int32_t time_font_size_pixels = font_size::to_pixels(time_font_size_points_, player_.main_window_ptr());
    ImFont* font_fwd_back_btns = imgui_fonts::instance().get(font_name_, fwd_back_font_size_pixels);
    ImFont* font_slider = imgui_fonts::instance().get(font_name_, slider_font_size_pixels);
    ImFont* font_time = imgui_fonts::instance().get(font_name_, time_font_size_pixels);

    ///if (!font_fwd_back_btns) { return; }
    bool show_controls = true;

    static int counter = 0;

    {

        ImGui::Rai imrai;
        imrai.Font(font_fwd_back_btns)
            .StyleVar(ImGuiStyleVar_WindowPadding, {0,0})
            .StyleVar(ImGuiStyleVar_WindowRounding, fwd_back_font_size_pixels/2)
            .StyleVar(ImGuiStyleVar_FrameRounding, fwd_back_font_size_pixels/2)
            ;

        // float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
        ImGui::SetNextWindowPos(video_back_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        ImGui::Begin("video_back_btn", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        ImGui::PushButtonRepeat(true);
        if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { counter--; }
        ImGui::PopButtonRepeat();
        ImGui::End();

        ImGui::SetNextWindowPos(video_fwd_btn_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        ImGui::Begin("video_fwd_btn", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        ImGui::PushButtonRepeat(true);
        if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { counter--; }
        ImGui::PopButtonRepeat();
        ImGui::End();
    }

    {
        float cur_video_pos = 0.5;
        ImGui::Rai imrai{};
        imrai.Font(font_slider)
            .StyleVar(ImGuiStyleVar_WindowPadding, {0,0})
            .StyleVar(ImGuiStyleVar_GrabMinSize, video_slider_grab_width_)
            .StyleVar(ImGuiStyleVar_GrabRounding, video_slider_grab_width_/4)
            ;

        ImGui::SetNextWindowPos(video_slider_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        ImGui::SetNextWindowSize(video_slider_size_, ImGuiCond_::ImGuiCond_Always);
        ImGui::Begin("video_slider_win", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        ImGui::PushItemWidth(-1);   // Force control to fill width with no label
        ImGui::SliderFloat("video_slider", &cur_video_pos, 0, 1, "");
        ImGui::PopItemWidth();
        ImGui::End();
    }


    {
        ImGui::Rai imrai{};
        imrai.Font(font_time)
             .StyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(4, 4))
            ;

        ImGui::SetNextWindowPos(elapsed_time_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        //ImGui::SetNextWindowSize(video_slider_size_, ImGuiCond_::ImGuiCond_Always);
        ImGui::Begin("elapsed_time", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        ImGui::TextUnformatted("Elapsed");
        ImGui::End();

        ImGui::SetNextWindowPos(remaining_time_pos_, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        //ImGui::SetNextWindowSize(video_slider_size_, ImGuiCond_::ImGuiCond_Always);
        ImGui::Begin("remaining_time", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
        ImGui::TextUnformatted("Remaining");
        ImGui::End();
    }

}


} // namespace cpaf::gui::video

