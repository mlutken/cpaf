#pragma once

#include <memory>
#include <config/cpaf_platform_definitions.h>

#if (CPAF_GRAPHICS_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/gui/video/render/render__sdl2.h when CPAF_GRAPHICS_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

#include <imgui.h>
#include <SDL2/SDL.h>
#include <cpaf_libs/gui/video/render/render_base.h>

namespace cpaf::gui {
class system_window;
}

namespace cpaf::gui::video {

class render;

class render_platform : public render_base
{
public:
    static std::unique_ptr<render>   create_video_render(const system_window& win, const cpaf::video::surface_dimensions_t& dimensions);

    ~render_platform();
    render_platform();

protected:


private:
    void    prepare_native_video_frame      (const cpaf::video::av_frame& frame, cpaf::video::av_frame& frame_display);
    void    render_current_native_video_frame_texture     ();
    void    ensure_valid_render_texture     (const cpaf::video::surface_dimensions_t& dimensions);
    void    render_subtitle_line            (pos_2df pos, std::string_view str);
    ImVec2  subtitle_line_geometry          (ImFont* font, std::string_view line) const;

    static SDL_Rect             to_sdl_rect                 (render_geometry_t geom);

    SDL_Renderer*               get_sdl_renderer                ();


    void                        do_init                     (const system_window& win, const cpaf::video::surface_dimensions_t& dimensions ) override;
    void                        do_init                     (std::shared_ptr<cpaf::gui::system_render> sys_renderer, const cpaf::video::surface_dimensions_t& dimensions ) override;
    void                        do_render_dimensions_set    (const cpaf::video::surface_dimensions_t& dimensions ) override;
    bool                        do_render_video_frame       (const cpaf::video::av_frame& frame) override;
    void                        do_render_subtitle          (std::string_view str) override;
    void                        on_subtitle_changed         () override;
    void                        do_render_subtitle          () override;

    std::shared_ptr<system_render>      system_renderer_;
    SDL_Texture*                        sdl_frame_render_texture_   {nullptr};

    ImVec4                              subtitle_color_{1,1,1,1};
};

} //END namespace cpaf::gui::video

