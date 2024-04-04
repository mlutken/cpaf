#include "controls.h"
#include <cpaf_libs/gui/video/player.h>

namespace cpaf::gui::video {

controls::controls(player& parent_player, config& cfg)
    : player_(parent_player)
    , config_(cfg)
{}

void controls::render()
{
    if (!visible()) {
        return;
    }
    do_render();

}

void controls::on_player_size_changed()
{
    do_calc_geometry();
}


} // namespace cpaf::gui::video

