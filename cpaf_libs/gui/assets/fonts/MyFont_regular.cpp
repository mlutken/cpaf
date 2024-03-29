#include "imgui_fonts.h"
#include <array>

namespace cpaf::gui {


std::span<const unsigned char> imgui_fonts::myfont_regular()
{
    static constexpr size_t font_len = XX;

    static const std::array<unsigned char, font_len> font_data =
    {
    };
    return std::span<const unsigned char>(font_data.begin(), font_data.end());

}

} // namespace capf::gui


