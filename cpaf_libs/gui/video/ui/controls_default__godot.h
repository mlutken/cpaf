#pragma once

#include <cpaf_libs/gui/video/ui/controls.h>
// #include <cpaf_libs/gui/texture.h>

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GRAPHICS_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/gui/video/render/render__sdl2.h when CPAF_GRAPHICS_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

namespace cpaf::gui::video {

class controls_default_platform : public controls
{
public:
    using controls::controls;	// "Import" constructors to scope
    controls_default_platform(player& parent_player, config& cfg);

private:
    void    do_calc_geometry                () override;
    void    do_render                       () override;
    void    do_render_stream_state          () override;
};

} // namespace cpaf::gui::video
