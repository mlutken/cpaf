#include "icons_data.h"

#include <algorithm>
#include <unordered_map>
#include <map>
#include <string>
#include <iostream>

#include <IconFontCppHeaders/IconsFontAwesome6.h>

using namespace std;

namespace cpaf::gui {

std::span<const unsigned char> icons_data::png_data(std::string_view icon_name)
{
    if      (icon_name == "circle_pause_256")       {   return icons_data::circle_pause_256();     }
    else if (icon_name == "circle_play_256")        {   return icons_data::circle_play_256();      }

    return std::span<const unsigned char>{};
}




} // namespace capf::gui
