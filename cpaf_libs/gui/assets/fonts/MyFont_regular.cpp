#include "imgui_fonts.h"
#include <array>

namespace cpaf::gui {


std::span<const unsigned char> imgui_fonts::myfont_regular()
{
    static constexpr size_t len = XX;

    static constexpr std::array<unsigned char, len> data =
    {
    };
    return std::span<const unsigned char>(data.begin(), data.end());

}

} // namespace capf::gui


