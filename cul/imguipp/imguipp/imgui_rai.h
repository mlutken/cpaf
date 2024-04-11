#pragma once

#include <string_view>
#include <vector>
#include <imgui.h>

namespace ImGui
{
class Rai
{
public:
    static Rai Create() { return Rai{}; }

    Rai(Rai&& other) {
        pushed_types_ = std::move(other.pushed_types_);
    }

    Rai& operator=(Rai&& other) {
        pushed_types_ = std::move(other.pushed_types_);
        return *this;
    }

    Rai() { pushed_types_.reserve(8);   }
    ~Rai()
    {
        for (auto stack_type: pushed_types_) {
            Pop(stack_type);
        }
    }

    Rai&    Font                (ImFont* font)                          { ImGui::PushFont(font); pushed_types_.push_back(Stacktype::Font); return *this; }
    Rai&    StyleColor          (ImGuiCol idx, ImU32 col)               { ImGui::PushStyleColor(idx, col); pushed_types_.push_back(Stacktype::StyleColor); return *this; }
    Rai&    StyleColor          (ImGuiCol idx, const ImVec4& col)       { ImGui::PushStyleColor(idx, col); pushed_types_.push_back(Stacktype::StyleColor); return *this; }
    Rai&    StyleVar            (ImGuiStyleVar idx, float val)          { ImGui::PushStyleVar(idx, val); pushed_types_.push_back(Stacktype::StyleVar); return *this; }
    Rai&    StyleVar            (ImGuiStyleVar idx, const ImVec2& val)  { ImGui::PushStyleVar(idx, val); pushed_types_.push_back(Stacktype::StyleVar); return *this; }
    Rai&    AllowKeyboardFocus  (bool allow_keyboard_focus)             { ImGui::PushAllowKeyboardFocus(allow_keyboard_focus); pushed_types_.push_back(Stacktype::AllowKeyboardFocus); return *this; }
    Rai&    ButtonRepeat        (bool repeat)                           { ImGui::PushButtonRepeat(repeat); pushed_types_.push_back(Stacktype::ButtonRepeat); return *this; }
    Rai&    ItemWidth           (float item_width)                      { ImGui::PushItemWidth(item_width); pushed_types_.push_back(Stacktype::ItemWidth); return *this; }
    Rai&    TextWrapPos         (float wrap_local_pos_x = 0.0f)         { ImGui::PushTextWrapPos(wrap_local_pos_x); pushed_types_.push_back(Stacktype::TextWrapPos); return *this; }

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

inline void TextUnformatted(std::string_view text) { ImGui::TextUnformatted(text.data(), text.data() + text.size()); }

inline bool Selectable(std::string_view label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0)) {
    return Selectable(label.data(), selected, flags, size);
}

inline bool Selectable(std::string_view label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0))
{
    return Selectable(label.data(), p_selected, flags, size);
}

}

