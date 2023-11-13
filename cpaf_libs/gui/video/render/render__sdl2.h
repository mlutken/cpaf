#pragma once

#include <memory>
#include <config/cpaf_platform_definitions.h>

#if (CPAF_GRAPHICS_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/gui/video/render/render__sdl2.h when CPAF_GRAPHICS_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

#include <SDL2/SDL.h>
#include <cpaf_libs/gui/video/render/render_base.h>

namespace cpaf::gui {
class system_window;
}

namespace cpaf::gui::video {

class render;

struct platform_render_t {
    platform_render_t() = default;
    platform_render_t(const platform_render_t& other) = default;
    platform_render_t(SDL_Renderer* render) : render_(render) {}

    platform_render_t& operator=(SDL_Renderer* render) { render_ = render; return *this; }
    platform_render_t& operator=(const platform_render_t& other) = default;
    SDL_Renderer* render_ = nullptr;
};


class render_platform : public render_base
{
public:
    static std::unique_ptr<render>   create_video_render(system_window& win, const cpaf::video::surface_dimensions_t& dimensions);

    ~render_platform();
    render_platform();

protected:


private:
    void prepare_native_video_frame    (const cpaf::video::av_frame& frame, cpaf::video::av_frame& frame_display);
    void render_current_native_video_frame_texture     ();
    void ensure_valid_render_texture   (const cpaf::video::surface_dimensions_t& dimensions);


    void                        do_init                     (system_window& win, const cpaf::video::surface_dimensions_t& dimensions ) override;
    void                        do_init                     (const platform_render_t& platform_render, const cpaf::video::surface_dimensions_t& dimensions ) override;
    void                        do_platform_render_set      (const platform_render_t& platform_render) override;
    platform_render_t&          do_platform_render          () override { return platform_render_; }
    void                        do_render_dimensions_set    (const cpaf::video::surface_dimensions_t& dimensions ) override;
    bool                        do_render_video_frame       (const cpaf::video::av_frame& frame) override;

    platform_render_t                   platform_render_;

    // SDL_Renderer*                       sdl_renderer_               {nullptr};
    SDL_Texture*                        sdl_frame_render_texture_   {nullptr};
};

} //END namespace cpaf::gui::video

