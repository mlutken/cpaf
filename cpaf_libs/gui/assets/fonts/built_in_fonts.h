#pragma once
#include <string_view>
#include <span>

struct ImGuiIO;

namespace cpaf::gui {


struct built_in_fonts {
    static  std::span<const unsigned char> manrope();

    static const std::span<const unsigned char>     font_data   (std::string_view font_name);

    static bool add_font        (ImGuiIO& io, std::string_view font_name, size_t font_size);


};

} // namespace capf::gui
