#pragma once

#include <vector>
#include <imgui.h>

namespace ImGui
{
class Rai
{
    Rai() { pushed_types_.reserve(8);   }
    ~Rai()
    {
        for (auto stack_type: pushed_types_) {
            Pop(stack_type);
        }
    }

    Rai&    Font                (ImFont* font)                          { ImGui::PushFont(font) ; return *this; }
    Rai&    StyleColor          (ImGuiCol idx, ImU32 col)               { ImGui::PushStyleColor(idx, col) ; return *this; }
    Rai&    StyleColor          (ImGuiCol idx, const ImVec4& col)       { ImGui::PushStyleColor(idx, col) ; return *this; }
    Rai&    StyleVar            (ImGuiStyleVar idx, float val)          { ImGui::PushStyleVar(idx, val) ; return *this; }
    Rai&    StyleVar            (ImGuiStyleVar idx, const ImVec2& val)  { ImGui::PushStyleVar(idx, val) ; return *this; }
    Rai&    AllowKeyboardFocus  (bool allow_keyboard_focus)             { ImGui::PushAllowKeyboardFocus(allow_keyboard_focus) ; return *this; }
    Rai&    ButtonRepeat        (bool repeat)                           { ImGui::PushButtonRepeat(repeat) ; return *this; }
    Rai&    ItemWidth           (float item_width)                      { ImGui::PushItemWidth(item_width) ; return *this; }
    Rai&    TextWrapPos         (float wrap_local_pos_x = 0.0f)         { ImGui::PushTextWrapPos(wrap_local_pos_x) ; return *this; }

private:
    enum class Stacktype {
        Font,
        StyleColor,
        StyleVar,
        AllowKeyboardFocus,
        ButtonRepeat,
        ItemWidth,
        TextWrapPos
    };

    std::vector<Stacktype>  pushed_types_;

    void            Pop     (Stacktype type);

};

}

