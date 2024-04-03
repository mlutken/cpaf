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
@see Color codes: https://github.com/astraw/svg_stack [crawler/cpaf/tools/bin/svg_stack.py]
*/
struct video_player_data {
    static std::span<const unsigned char> video_player_control_icons     ();
};

} // namespace capf::gui
