#include "render_base.h"
#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/time/cpaf_time.h>

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
    main_window_ptr_ = &win;
    do_init(win, dimensions);
    render_dimensions_ = dimensions;
}

void render_base::init_only_for_old_playground(std::shared_ptr<cpaf::gui::system_render> sys_renderer,
                                               const cpaf::video::surface_dimensions_t& dimensions)
{
    do_init(sys_renderer, dimensions);
    render_dimensions_ = dimensions;
}

bool render_base::render_video_frame(const cpaf::video::av_frame& frame) {
    if (!frame) {
        std::cerr << "!!! ERROR video render NO frame!\n";
        clear_screen();
        return false;
    }

    const bool res = do_render_video_frame(frame);
    return res;
}

void render_base::clear_current_subtitle()
{
    if (current_subtitle_frame_.is_valid()) {
        current_subtitle_frame_ = cpaf::video::subtitle_frame();
    }
}

void render_base::set_current_subtitle(cpaf::video::subtitle_frame&& subtitle)
{
//    if (subtitle.sequence_number != current_subtitle_frame_.sequence_number ||
//        subtitle.presentation_time != current_subtitle_frame_.presentation_time ||
//        subtitle.should_show() != current_subtitle_frame_.should_show() ) {
//        current_subtitle_frame_ = std::move(subtitle);
//        on_render_geometry_changed();
//    }

    current_subtitle_frame_ = std::move(subtitle);
    if (current_subtitle_frame_.should_show()) {

// DEBUG ONLY BEGIN
//        std::cerr << "FIXMENM set_current_subtitle: " << current_subtitle_frame_.dbg_str() << "\n";
//        current_subtitle_frame_.lines.push_back(cpaf::time::format_h_m_s(current_subtitle_frame_.presentation_time));
//        current_subtitle_frame_.format_set(subtitle_frame::format_t::text);
// DEBUG ONLY BEGIN END
        on_render_geometry_changed();
    }
}

void render_base::render_dimensions_set(const surface_dimensions_t& dimensions) {
    do_render_dimensions_set(dimensions);
    render_dimensions_ = dimensions;
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
