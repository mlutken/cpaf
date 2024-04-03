#pragma once
#include <string>
#include <span>
#include <cstdint>

namespace cpaf::gui {

/**

## Convert png files to c array use xxd to get started
 $ xxd -i myicon.png > myicon.cpp

*/
struct icons_data {
    static  std::span<const unsigned char> circle_pause_256     ();
    static  std::span<const unsigned char> circle_play_256      ();

    static const std::span<const unsigned char>     png_data_256   (const std::string&  icon_name);


private:
};

} // namespace capf::gui
