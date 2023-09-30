#include "built_in_fonts.h"
#include <imgui.h>

namespace cpaf::gui {

const std::span<const unsigned char> built_in_fonts::font_data(std::string_view font_name)
{
    if (font_name == "manrope") {
        return built_in_fonts::manrope();
    }

    return std::span<const unsigned char>{};
}

bool built_in_fonts::add_font(ImGuiIO& io, std::string_view font_name, size_t font_size)
{
    const auto font_data = built_in_fonts::font_data (font_name);
    if (font_data.empty()) {
        return false;
    }
    ImFontConfig fnt_cfg;
    fnt_cfg.FontDataOwnedByAtlas = false;
    unsigned char* data = const_cast<unsigned char*>(font_data.data());
    const int size = font_data.size();
    io.FontDefault = io.Fonts->AddFontFromMemoryTTF(data, size, font_size, &fnt_cfg);
    return true;
}



} // namespace capf::gui
