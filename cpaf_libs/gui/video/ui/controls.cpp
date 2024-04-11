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

locale::translator& controls::tr()
{
    return player_.tr();
}

const locale::translator& controls::tr() const
{
    return player_.tr();
}


} // namespace cpaf::gui::video

