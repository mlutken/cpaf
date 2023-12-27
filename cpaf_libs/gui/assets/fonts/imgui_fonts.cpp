#include "imgui_fonts.h"

#include <algorithm>
#include <unordered_map>
#include <map>
#include <string>
#include <imgui.h>

using namespace std;

namespace cpaf::gui {

const std::span<const unsigned char> imgui_fonts::font_data(const string& font_name)
{
    if (font_name == "manrope") {
        return imgui_fonts::manrope();
    }

    return std::span<const unsigned char>{};
}

imgui_fonts& imgui_fonts::instance()
{
    static imgui_fonts inst;
    return inst;
}

ImFont* imgui_fonts::add(const string& font_name, uint32_t size_pixels)
{
    ImFont* font_ptr = find(font_name, size_pixels);
    if (font_ptr) {
        return font_ptr;
    }


    const auto font_data = imgui_fonts::font_data (font_name);
    if (font_data.empty()) {
        return nullptr;
    }

    if (ImGui::GetIO().Fonts->Locked) {
        requested_fonts_.push_back({font_name, size_pixels});
        return nullptr;
    }

    ImFontConfig fnt_cfg;
    fnt_cfg.FontDataOwnedByAtlas = false;
    unsigned char* data = const_cast<unsigned char*>(font_data.data());
    const int size = font_data.size();
    font_ptr = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(data, size, size_pixels, &fnt_cfg);

    all_fonts_[string(font_name)][size_pixels] = font_ptr;

//    ImGui::GetIO().FontDefault = font_ptr;

    return font_ptr;
}

bool imgui_fonts::add(const string& font_name, const std::vector<uint32_t>& size_pixels)
{
    bool ok = false;
    for (const auto size: size_pixels) {
        ok = imgui_fonts::add(font_name, size) != nullptr;
    }
    return ok;
}

void imgui_fonts::set_default(const string& font_name, uint32_t size_pixels)
{
    ImGui::GetIO().FontDefault = imgui_fonts::find(font_name, size_pixels);
}

ImFont* imgui_fonts::get(const std::string& font_name, int32_t size_pixels, int32_t create_dist_pixels)
{
    return find_create_closest(font_name, size_pixels, create_dist_pixels);
}

ImFont* imgui_fonts::find(const string& font_name, uint32_t size_pixels) const
{
    const auto it_name = all_fonts_.find(font_name);
    if (it_name == all_fonts_.end()) {
        return nullptr;
    }

    const auto& size_map = it_name->second;
    const auto it_size = size_map.find(size_pixels);
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

void imgui_fonts::add_pending_requested_fonts()
{
    if (requested_fonts_.empty()) {
        return;
    }

    if (ImGui::GetIO().Fonts->Locked) {
        return;
    }

    for (const auto& requested_font : requested_fonts_) {
        add(requested_font.font, requested_font.size_pixels);
    }
}


ImFont* imgui_fonts::find_create_closest(const string& font_name, int32_t size_pixels, int32_t create_dist_pixels)
{
    font_size_map& size_map = all_fonts_[font_name];

    // See if we have an exact match
    const auto it_size = size_map.find(size_pixels);
    if (it_size != size_map.end()) {
        return it_size->second;
    }

    // Look for a close enough match (create new only if more than create_dist_pixels away)
    for (const auto& [size, font_ptr] : size_map) {
        const int32_t dist_in_pixels = std::abs(static_cast<int32_t>(size) - static_cast<int32_t>(size_pixels));
        if ( dist_in_pixels <= create_dist_pixels) {
            return font_ptr;
        }
    }

    // Create new as we are too far from
    return add(font_name, static_cast<uint32_t>(size_pixels));
}



} // namespace capf::gui
