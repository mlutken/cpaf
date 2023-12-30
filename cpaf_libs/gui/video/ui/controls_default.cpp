#include "controls_default.h"

#include <imguipp/imgui_rai.h>
#include <cpaf_libs/gui/fonts/font_size.h>
#include <cpaf_libs/gui/assets/fonts/imgui_fonts.h>
#include <cpaf_libs/gui/video/player.h>
namespace cpaf::gui::video {

void controls_default::do_render()
{
    const int32_t font_size_pixels = font_size::to_pixels(fwd_back_font_size_points_, player_.main_window_ptr());
    ImFont* font = imgui_fonts::instance().get(font_name_, font_size_pixels);
    if (!font) { return; }
    bool show_controls = true;

    static int counter = 0;

    ImGui::PushStyleColor(ImGuiCol_Border, reinterpret_cast<const ImVec4&>(bg_color_));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, reinterpret_cast<const ImVec4&>(bg_color_));
    ImGui::PushFont(font);
    ImGui::PushStyleColor(ImGuiCol_Text, reinterpret_cast<const ImVec4&>(text_color_));
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(4, 4)); // Just something smaller than what we would realistictly use as font size!


    // float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    ImGui::Begin("controls", &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { counter--; }
    // ImGui::SameLine(0.0f, spacing);
    // if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { counter++; }
    ImGui::PopButtonRepeat();
    // ImGui::SameLine();
    // ImGui::Text("%d", counter);
    ImGui::End();

    // ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

}


} // namespace cpaf::gui::video

