#pragma once

#include <memory>
#include <array>
#include <config/cpaf_platform_definitions.h>

#if (CPAF_GRAPHICS_SYSTEM_ID != CPAF_SYSTEM_ID_GODOT)
#error Only include cpaf_libs/gui/video/render/render__godot.h when CPAF_GRAPHICS_SYSTEM_ID is CPAF_SYSTEM_ID_GODOT
#endif

#include <cpaf_libs/gui/video/render/render_base.h>

namespace cpaf::gui {
class system_window;
}

namespace cpaf::gui::video {
class player;
class render;

class render_platform : public render_base
{
public:
    static std::unique_ptr<render> create_video_render(player& owning_player,
                                                       config& cfg,
                                                       const system_window& win,
                                                       const rect& render_geom,
                                                       const surface_dimensions_t& video_src_dimensions);

    ~render_platform();
    explicit render_platform(player& owning_player, config& cfg);

protected:


private:
    using subtitle_render_geometries_t = std::array<rect, cpaf::video::subtitle_frame::max_lines>;

    void            fill_native_video_frame                     (const cpaf::video::av_frame& frame,
                                                                 cpaf::video::av_frame& frame_display);
    void            fill_native_subtitle_texture                ();
    void            ensure_valid_render_texture                 ();
    void            ensure_valid_subtitles_graphics_texture     (const cpaf::video::subtitle_frame& subtitle);
    void            calc_subtitle_geometry                      ();

    void            render_subtitle_text        ();
    void            render_subtitle_graphics    ();



    void            do_init                     (const system_window& win) override;
    void            do_init                     (std::shared_ptr<cpaf::gui::system_render> sys_renderer) override;
    void            do_clear_screen             () override;
    bool            do_render_video_frame       (const cpaf::video::av_frame& frame) override;
    void            on_render_geometry_changed  () override;
    void            on_subtitle_changed         () override;


    std::shared_ptr<system_render>      system_renderer_;
    subtitle_render_geometries_t        subtitle_render_geometries_;
};

} //END namespace cpaf::gui::video

