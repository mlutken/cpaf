#pragma once
#include <string>
#include <string_view>
#include <span>
#include <cstdint>

namespace cpaf::gui {

/**

## Convert png files to c array use xxd to get started
 $ xxd -i myicon.png > myicon.cpp

@see Color codes: https://html-color.codes/
*/
struct icons_data {
    static std::span<const unsigned char> circle_pause_256     ();
    static std::span<const unsigned char> circle_play_256      ();

    static std::span<const unsigned char> png_data              (std::string_view icon_name);


private:
};

} // namespace capf::gui
