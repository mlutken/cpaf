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

void render_base::init(system_window& win, const cpaf::video::surface_dimensions_t& dimensions)
{
    do_init(win, dimensions);
}

void render_base::init(std::shared_ptr<cpaf::gui::system_render> sys_renderer, const cpaf::video::surface_dimensions_t& dimensions)
{
    do_init(sys_renderer, dimensions);
}

// void render_base::init(const platform_render_t& platform_render, const cpaf::video::surface_dimensions_t& dimensions)
// {
//     do_init(platform_render, dimensions);
// }

void render_base::create_frame_display()
{
    frame_display_ = video_codec_ctx().create_scaling_dst_frame();
}

} //END namespace cpaf::gui::video
