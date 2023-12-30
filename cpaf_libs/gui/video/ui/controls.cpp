#include "controls.h"
#include <cpaf_libs/gui/video/player.h>

namespace cpaf::gui::video {

controls::controls(player& parent_player)
    : player_(parent_player)
{}

void controls::render()
{
    if (!visible()) {
        return;
    }
    do_render();

}

} // namespace cpaf::gui::video

