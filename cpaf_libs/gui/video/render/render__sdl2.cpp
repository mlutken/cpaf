#include "render__sdl2.h"
#include "render.h"

#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/video/av_codec_context.h>

namespace cpaf::gui::video {

std::unique_ptr<render> render_platform::create_video_render(
    cpaf::gui::system_window& win,
    const cpaf::video::surface_dimensions_t& dimensions)
{
    auto video_renderer = std::make_unique<render>();
    video_renderer->init(win, dimensions);

    return video_renderer;
}

render_platform::~render_platform()
{
    if (sdl_frame_render_texture_) {
        SDL_DestroyTexture(sdl_frame_render_texture_);
    }
}

render_platform::render_platform()
    : render_base()
{

}


void render_platform::prepare_native_video_frame(
    const cpaf::video::av_frame& frame,
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
        sdl_frame_render_texture_,
        &rect,
        frame_display.ff_frame()->data[0],
        frame_display.ff_frame()->linesize[0],
        frame_display.ff_frame()->data[1],
        frame_display.ff_frame()->linesize[1],
        frame_display.ff_frame()->data[2],
        frame_display.ff_frame()->linesize[2]
    );
}

void render_platform::render_current_native_video_frame_texture()
{
    // SDL_RenderClear(platform_render_.render_); // TODO: Perhaps not clear at this point but leave up to application!
    SDL_RenderCopy(platform_render_.render_, sdl_frame_render_texture_, NULL, NULL);
//    SDL_Rect dest_rect = sdl2::texture_destination_rect(image_bitmap_texture, 30, 30);
//    SDL_RenderCopy(sdl_renderer, image_bitmap_texture, NULL, &dest_rect);
    // SDL_RenderPresent(platform_render_.render_);
}

void render_platform::ensure_valid_render_texture(const cpaf::video::surface_dimensions_t& dimensions)
{
    if (dimensions != render_dimensions_) {
        if (sdl_frame_render_texture_) {
            SDL_DestroyTexture(sdl_frame_render_texture_);
        }
        render_dimensions_ = dimensions;

        sdl_frame_render_texture_ = SDL_CreateTexture(
            platform_render_.render_,
            SDL_PIXELFORMAT_YV12,
            SDL_TEXTUREACCESS_STREAMING,
            dimensions.x(),
            dimensions.y()
        );
    }
}

void render_platform::do_init(system_window& win, const cpaf::video::surface_dimensions_t& dimensions)
{
    platform_render_ = platform_render_t{ win.native_renderer<SDL_Renderer>()};
    ensure_valid_render_texture(dimensions);
}

void render_platform::do_init(const platform_render_t& platform_render, const cpaf::video::surface_dimensions_t& dimensions)
{
    platform_render_ = platform_render;
    ensure_valid_render_texture(dimensions);
}

void render_platform::do_platform_render_set(const platform_render_t& platform_render)
{
    platform_render_ = platform_render;
}

void render_platform::do_render_dimensions_set(const cpaf::video::surface_dimensions_t& dimensions)
{
    ensure_valid_render_texture(dimensions);
}

bool render_platform::do_render_video_frame(const cpaf::video::av_frame& frame)
{
    prepare_native_video_frame(frame, frame_display());
    render_current_native_video_frame_texture();
    return true;
}

} //END namespace cpaf::gui::video
