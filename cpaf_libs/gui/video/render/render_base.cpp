#include "render_base.h"
#include <cpaf_libs/video/av_format_context.h>

namespace cpaf::gui::video {

render_base::render_base()
{

}

void render_base::render_geometry_set(render_geometry_t render_geom)
{
    render_geometry_ = render_geom;
    on_render_geometry_changed();
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

void render_base::subtitle_color_set(const color& text_color, const color& bg_color)
{
    subtitles_text_color_ = text_color;
    subtitles_bg_color_ = bg_color;
}

void render_base::subtitle_font_set(std::string font_name, uint16_t font_size_points)
{
    subtitles_font_name_ = std::move(font_name);
    subtitles_font_size_points_ = font_size_points;
    on_render_geometry_changed();
}

void render_base::init(const system_window& win, const cpaf::video::surface_dimensions_t& dimensions)
{
    do_init(win, dimensions);
}

void render_base::init(std::shared_ptr<cpaf::gui::system_render> sys_renderer, const cpaf::video::surface_dimensions_t& dimensions)
{
    do_init(sys_renderer, dimensions);
}

void render_base::render_subtitle(const cpaf::video::subtitle_frame& subtitle)
{
    if (subtitle.sequence_number != current_subtitle_frame_.sequence_number ||
        subtitle.presentation_time != current_subtitle_frame_.presentation_time ||
        subtitle.should_show() != current_subtitle_frame_.should_show() ) {
        current_subtitle_frame_ = subtitle;
        on_render_geometry_changed();
    }
    do_render_subtitle();
}

pos_2df render_base::subtitle_pos() const
{
    return subtitle_relative_pos_ * render_geometry_.size;
}


void render_base::create_frame_display()
{
    frame_display_ = video_codec_ctx().create_scaling_dst_frame();
}

} //END namespace cpaf::gui::video
