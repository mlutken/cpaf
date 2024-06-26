#pragma once

#include <string_view>
#include <string>
#include <vector>
#include <imgui.h>

// Centering widgets: https://github.com/ocornut/imgui/discussions/3862

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
    ~Rai();

    Rai&    Font                (ImFont* font)                          { ImGui::PushFont(font); pushed_types_.push_back(Stacktype::Font); return *this; }
    Rai&    StyleColor          (ImGuiCol idx, ImU32 col)               { ImGui::PushStyleColor(idx, col); pushed_types_.push_back(Stacktype::StyleColor); return *this; }
    Rai&    StyleColor          (ImGuiCol idx, const ImVec4& col)       { ImGui::PushStyleColor(idx, col); pushed_types_.push_back(Stacktype::StyleColor); return *this; }
    Rai&&   ButtonColorAuto     (const ImVec4& col);
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


// ------------------------
// --- Widgets: Buttons ---
// ------------------------

inline bool Button(std::string_view label, const ImVec2& size = ImVec2(0, 0)) {
    return Button(label.data(), size);
}

bool AlignedButton  (std::string_view label, float alignment, float relative_width);
bool CenteredButton (std::string_view label, float relative_width);

// ------------------------------
// --- Widgets: Radio Buttons ---
// ------------------------------
inline bool RadioButton(std::string_view label, int& value, int button_value) {
    return RadioButton(label.data(), &value, button_value);
}
bool AlignedRadioButton(std::string_view label, int& value, int button_value, float alignment);


// ---------------------------
// --- Widgets: Input text ---
// ---------------------------
bool InputText          (std::string_view label, std::string& str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
bool InputTextMultiline (std::string_view label, std::string& str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);
bool InputTextWithHint  (std::string_view label, std::string_view hint, std::string& str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr);


// ----------------------------
// --- Widgets: Selectables ---
// ----------------------------

inline bool Selectable(std::string_view label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0)) {
    return Selectable(label.data(), selected, flags, size);
}

inline bool Selectable(std::string_view label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0))
{
    return Selectable(label.data(), p_selected, flags, size);
}

// -------------------
// --- Text output ---
// -------------------
inline void TextUnformatted (std::string_view text) { ImGui::TextUnformatted(text.data(), text.data() + text.size()); }
void        TextCentered    (std::string_view text);


// -------------
// --- Menus ---
// -------------
inline bool MenuItem(std::string_view label, std::string_view shortcut = std::string_view(), bool selected = false, bool enabled = true) {
    return MenuItem(label.data(), shortcut.data(), selected, enabled);
}

inline bool MenuItem(std::string_view label, std::string_view shortcut, bool* p_selected, bool enabled = true) {
    return MenuItem(label.data(), shortcut.data(), p_selected, enabled);
}

// ------------------------
// --- Window positions ---
// ------------------------
void SetNextWindowPosRelative   (const ImVec2& pos_relative, ImGuiCond cond = 0, const ImVec2& pivot = ImVec2(0, 0));
void SetNextWindowSizeRelative  (const ImVec2& size_relative, ImGuiCond cond = 0);

// ---------------------------
// --- Convenience helpers ---
// ---------------------------
// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
void HelpMarker(std::string_view description);

// ------------------------------
// --- Alignment/Layout Utils ---
// ------------------------------
float CalcTextWidth(std::string_view text, const ImGuiStyle& style = ImGui::GetStyle());


}

