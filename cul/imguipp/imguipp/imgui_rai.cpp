
#include "imgui_rai.h"
#include <ranges>
#include <limits>


namespace ImGui
{


// -----------
// --- Rai ---
// -----------

Rai::~Rai()
{
    for (auto stack_type: pushed_types_ | std::views::reverse) {
        Pop(stack_type);
    }
}

Rai&& Rai::ButtonColorAuto(const ImVec4& col)
{
    constexpr auto hover_fac = 1.2f;
    constexpr auto active_fac = 1.1f;
    StyleColor(ImGuiCol_Button, col);
    StyleColor(ImGuiCol_ButtonHovered, ImVec4(col.x*hover_fac, col.y*hover_fac, col.z*hover_fac, col.w*hover_fac));
    StyleColor(ImGuiCol_ButtonActive, ImVec4(col.x*active_fac, col.y*active_fac, col.z*active_fac, col.w*active_fac));
    return std::move(*this);
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

// ------------------------
// --- Widgets: Buttons ---
// ------------------------

bool AlignedButton(std::string_view label, float alignment, float relative_width)
{
    const auto content_region = ImGui::GetContentRegionAvail();
    ImVec2 size {relative_width*content_region.x, 0};
    const float width_calculated = CalcTextWidth(label);
    const float width_wanted = std::max(size.x, width_calculated);
    size.x = std::min(width_wanted, content_region.x);

    const float avail = content_region.x;

    const float off = (avail - size.x) * alignment;
    if (off > 0.0f) {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
    }

    return ImGui::Button(label, size);
}


bool CenteredButton(std::string_view label, float relative_width)
{
    return AlignedButton(label, 0.5f, relative_width);
}

// ------------------------------
// --- Widgets: Radio Buttons ---
// ------------------------------
bool AlignedRadioButton(std::string_view label, int& value, int button_value, float alignment)
{
    const auto content_region = ImGui::GetContentRegionAvail();
    const float width_calculated = CalcTextWidth(label);

    const float avail = content_region.x;

    const float off = (avail - width_calculated) * alignment;
    if (off > 0.0f) {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
    }

    return RadioButton(label, value, button_value);
}


// ---------------------------
// --- Widgets: Input text ---
// ---------------------------
struct InputTextCallback_UserData
{
    std::string*            Str;
    ImGuiInputTextCallback  ChainCallback;
    void*                   ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
    InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
        std::string* str = user_data->Str;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(static_cast<size_t>(data->BufTextLen));
        data->Buf = (char*)str->c_str();
    }
    else if (user_data->ChainCallback)
    {
        // Forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}

bool InputText(std::string_view label, std::string& str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = &str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputText(label.data(), const_cast<char*>(str.c_str()), str.capacity() + 1, flags, InputTextCallback, &cb_user_data);
}

bool InputTextMultiline(std::string_view label, std::string& str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = &str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputTextMultiline(label.data(), const_cast<char*>(str.c_str()), str.capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
}

bool InputTextWithHint(std::string_view label, std::string_view hint, std::string& str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = &str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputTextWithHint(label.data(), hint.data(), const_cast<char*>(str.c_str()), str.capacity() + 1, flags, InputTextCallback, &cb_user_data);
}

// ------------------------
// --- Window positions ---
// ------------------------
void SetNextWindowPosRelative (const ImVec2& pos_relative, ImGuiCond cond, const ImVec2& pivot)
{
    const auto content_region = ImGui::GetContentRegionAvail();
    const ImVec2 pos {pos_relative.x*content_region.x, pos_relative.y*content_region.y};
    SetNextWindowPos(pos, cond, pivot);
}


void SetNextWindowSizeRelative(const ImVec2& size_relative, ImGuiCond cond)
{
    const auto content_region = ImGui::GetContentRegionAvail();
    const ImVec2 size {size_relative.x*content_region.x, size_relative.y*content_region.y};
    SetNextWindowSize(size, cond);
}


// ---------------------------
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

// ------------------------------
// --- Alignment/Layout Utils ---
// ------------------------------

float CalcTextWidth(std::string_view text, const ImGuiStyle& style)
{
    return ImGui::CalcTextSize(text.data()).x + style.FramePadding.x * 2.0f;

}







}
