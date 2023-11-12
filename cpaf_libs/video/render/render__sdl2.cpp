#include "render__sdl2.h"

#include <cpaf_libs/video/av_codec_context.h>

namespace cpaf::gui::video {

render_platform::render_platform()
    : render_base()
{

}


void render_platform::sdl_prepare_video_frame(const cpaf::video::av_frame& frame,
                         cpaf::video::av_frame& frame_display
                         )
{
    video_codec_ctx().scale_video_frame(frame_display, frame);
    // the area of the texture to be updated
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = render_dimensions().x();
    rect.h = render_dimensions().y();

    // update the texture with the new pixel data
    SDL_UpdateYUVTexture(
                platform_video_texture_.texture_,
                &rect,
                frame_display.ff_frame()->data[0],
            frame_display.ff_frame()->linesize[0],
            frame_display.ff_frame()->data[1],
            frame_display.ff_frame()->linesize[1],
            frame_display.ff_frame()->data[2],
            frame_display.ff_frame()->linesize[2]
            );
}

void render_platform::sdl_render_current_video_frame_texture()
{
    SDL_RenderClear(platform_render_.render_);
    SDL_RenderCopy(platform_render_.render_, platform_video_texture_.texture_, NULL, NULL);
//    SDL_Rect dest_rect = sdl2::texture_destination_rect(image_bitmap_texture, 30, 30);
//    SDL_RenderCopy(sdl_renderer, image_bitmap_texture, NULL, &dest_rect);
    SDL_RenderPresent(platform_render_.render_);
}

void render_platform::do_platform_render_set(const platform_render_t& platform_render)
{
    platform_render_ = platform_render;
}

void render_platform::do_platform_surface_set(const platform_surface_t& platform_surface)
{
    platform_surface_ = platform_surface;
}

void render_platform::do_platform_video_texture_set(const platform_texture_t& platform_texture)
{
    platform_video_texture_ = platform_texture;
}

bool render_platform::do_render_video_frame(const cpaf::video::av_frame& frame)
{
    sdl_prepare_video_frame(frame, frame_display());
    sdl_render_current_video_frame_texture();
    return true;
}

} //END namespace cpaf::gui::video
