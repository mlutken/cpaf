#pragma once
#include <string_view>
#include <span>
#include <vector>
#include <cstdint>

struct ImGuiIO;
struct ImFont;

namespace cpaf::gui {


struct imgui_fonts {
    static  std::span<const unsigned char> manrope();

    static const std::span<const unsigned char>     font_data   (std::string_view font_name);

    static bool     add                 (std::string_view font_name, uint16_t font_size);
    static bool     add                 (std::string_view font_name, const std::vector<uint16_t>& font_sizes);
    static void     set_default         (std::string_view font_name, uint16_t font_size);
    static ImFont*  get                 (std::string_view font_name, uint16_t font_size);


};

} // namespace capf::gui
