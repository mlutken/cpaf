#pragma once

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GRAPHICS_SYSTEM_ID != CPAF_SYSTEM_ID_SDL2)
#error Only include cpaf_libs/video/render/render__sdl2.h when CPAF_GRAPHICS_SYSTEM_ID is CPAF_SYSTEM_ID_SDL2
#endif

#include <SDL2/SDL.h>
#include <cpaf_libs/video/render/render_base.h>

namespace cpaf::video {

struct platform_render_t {
    platform_render_t() = default;
    platform_render_t(const platform_render_t& other) = default;
    platform_render_t(SDL_Renderer* render) : render_(render) {}

    platform_render_t& operator=(SDL_Renderer* render) { render_ = render; return *this; }
    platform_render_t& operator=(const platform_render_t& other) = default;
    SDL_Renderer* render_ = nullptr;
};

struct platform_surface_t {
    platform_surface_t() = default;
    platform_surface_t(const platform_surface_t& other) = default;
    platform_surface_t(SDL_Surface* surface) : surface_(surface) {}

    platform_surface_t& operator=(SDL_Surface* surface) { surface_ = surface; return *this; }
    platform_surface_t& operator=(const platform_surface_t& other) = default;
    SDL_Surface* surface_ = nullptr;
};

struct platform_texture_t {
    platform_texture_t() = default;
    platform_texture_t(const platform_texture_t& other) = default;
    platform_texture_t(SDL_Texture* texture) : texture_(texture) {}

    platform_texture_t& operator=(SDL_Texture* texture) { texture_ = texture; return *this; }
    platform_texture_t& operator=(const platform_texture_t& other) = default;
    SDL_Texture* texture_ = nullptr;
};

//

class render_platform : public render_base
{
public:

    // static platform_render_t    create_video_render()

    render_platform();

private:
    void sdl_prepare_video_frame    (const av_frame& frame, av_frame& frame_display);
    void sdl_render_current_video_frame_texture     ();


    void                        do_platform_render_set      (const platform_render_t& platform_render) override;
    void                        do_platform_surface_set     (const platform_surface_t& platform_surface) override;
    void                        do_platform_video_texture_set     (const platform_texture_t& platform_texture) override;
    platform_render_t&          do_platform_render          () override { return platform_render_; }
    platform_surface_t&         do_platform_surface         () override { return platform_surface_; }
    platform_texture_t&         do_platform_video_texture   () override { return platform_video_texture_; }
    bool                        do_render_video_frame       (const av_frame& frame) override;

    platform_render_t           platform_render_;
    platform_surface_t          platform_surface_;
    platform_texture_t          platform_video_texture_;
};

} //END namespace cpaf::video

