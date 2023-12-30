
#include "imgui_rai.h"


namespace ImGui
{



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

}
