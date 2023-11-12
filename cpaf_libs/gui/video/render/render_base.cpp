#include "render_base.h"
#include <cpaf_libs/video/av_format_context.h>

namespace cpaf::gui::video {

render_base::render_base()
{

}

void render_base::video_codec_ctx_set(cpaf::video::av_codec_context* ctx)
{
    video_codec_ctx_ptr_ = ctx;
    create_frame_display();
}

void render_base::video_codec_ctx_set(cpaf::video::av_codec_context& ctx)
{
    video_codec_ctx_ptr_ = &ctx;
    create_frame_display();
}

void render_base::init(const platform_render_t& platform_render, const platform_texture_t& platform_texture, const cpaf::video::surface_dimensions_t& dimensions)
{
    render_dimensions_ = dimensions;
    do_platform_render_set(platform_render);
    do_platform_video_texture_set(platform_texture);
}

void render_base::create_frame_display()
{
    frame_display_ = video_codec_ctx().create_scaling_dst_frame();
}

} //END namespace cpaf::gui::video
