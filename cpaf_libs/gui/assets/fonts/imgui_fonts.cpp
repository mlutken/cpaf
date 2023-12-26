#include "imgui_fonts.h"

#include <unordered_map>
#include <map>
#include <string>
#include <imgui.h>

using namespace std;

namespace cpaf::gui {


using font_lookup_map = map<string, map<uint16_t, ImFont*, std::less<>>, std::less<>>;
////using font_lookup_map = unordered_map<string, unordered_map<uint16_t, ImFont*>, std::hash<string>, std::equal_to<>>;

font_lookup_map& all_fonts()
{
    static font_lookup_map all_the_fonts;

    return all_the_fonts;
}

const std::span<const unsigned char> imgui_fonts::font_data(std::string_view font_name)
{
    if (font_name == "manrope") {
        return imgui_fonts::manrope();
    }

    return std::span<const unsigned char>{};
}

bool imgui_fonts::add(std::string_view font_name, uint16_t font_size)
{
    const auto font_data = imgui_fonts::font_data (font_name);
    if (font_data.empty()) {
        return false;
    }
    ImFontConfig fnt_cfg;
    fnt_cfg.FontDataOwnedByAtlas = false;
    unsigned char* data = const_cast<unsigned char*>(font_data.data());
    const int size = font_data.size();
    const auto font_ptr = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(data, size, font_size, &fnt_cfg);

    all_fonts()[string(font_name)][font_size] = font_ptr;

//    ImGui::GetIO().FontDefault = font_ptr;

    return true;
}

bool imgui_fonts::add(std::string_view font_name, const std::vector<uint16_t>& font_sizes)
{
    bool ok = false;
    for (const auto size: font_sizes) {
        ok = imgui_fonts::add(font_name, size);
    }
    return ok;
}

void imgui_fonts::set_default(std::string_view font_name, uint16_t font_size)
{
    ImGui::GetIO().FontDefault = imgui_fonts::get(font_name, font_size);
}

ImFont* imgui_fonts::get(std::string_view font_name, uint16_t font_size)
{
    const auto& fonts = all_fonts();
    const auto it_name = fonts.find(font_name);
    if (it_name == fonts.end()) {
        return nullptr;
    }

    const auto& size_map = it_name->second;
    const auto it_size = size_map.find(font_size);
    if (it_size == size_map.end()) {
        return nullptr;
    }

    return it_size->second;


//    try {
//        return all_fonts().at(font_name).at(font_size);
//    } catch (...) {
//        return nullptr;
//    }
}



} // namespace capf::gui
