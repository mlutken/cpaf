
#include "imgui_rai.h"
#include <ranges>


namespace ImGui
{


Rai::~Rai()
{
    for (auto stack_type: pushed_types_ | std::views::reverse) {
        Pop(stack_type);
    }
}

void Rai::Pop(Stacktype type)
{
    switch (type) {
    case Stacktype::Font:
        ImGui::PopFont(); break;
    case Stacktype::StyleColor:
        ImGui::PopStyleColor(1); break;
    case Stacktype::StyleVar:
        ImGui::PopStyleVar(1); break;
    case Stacktype::AllowKeyboardFocus:
        ImGui::PopAllowKeyboardFocus(); break;
    case Stacktype::ButtonRepeat:
        ImGui::PopButtonRepeat(); break;
    case Stacktype::ItemWidth:
        ImGui::PopItemWidth(); break;
    case Stacktype::TextWrapPos:
        ImGui::PopTextWrapPos(); break;
    default:
        break;
    }
}

// --- Convenience helpers ---
// ---------------------------
// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
void HelpMarker(std::string_view description)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(description.data());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}


}
