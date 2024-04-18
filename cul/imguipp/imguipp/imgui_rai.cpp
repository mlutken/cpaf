
#include "imgui_rai.h"
#include <ranges>


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




}
