#include "render_base.h"
#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/time/cpaf_time.h>
#include <cpaf_libs/gui/video/config.h>
#include <cpaf_libs/gui/video/player.h>

namespace cpaf::gui::video {


render_base::render_base(player& owning_player, config& cfg)
    : player_(owning_player)
    , config_(cfg)
{
    std::cerr << "FIXMENM render_base::render_base CONSTRUCTOR\n";
}

void render_base::render_geometry_set(const rect& render_geom)
{
    render_geometry_ = render_geom;
    video_render_geometry_ = render_geom;
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

void render_base::init(const system_window& win,
                       const rect& render_geom,
                       const cpaf::video::surface_dimensions_t& video_src_dimensions)
{
    main_window_ptr_ = &win;
    video_src_dimensions_ =  video_src_dimensions;
    do_init(win);
    render_geometry_set(render_geom);
    ////texture_render_dimensions_ = render_dimensions;
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

/// void render_base::texture_render_dimensions_set(const surface_dimensions_t& dimensions) {
///     do_render_dimensions_set(dimensions);
///     /// texture_render_dimensions_ = dimensions;
/// }

std::string render_base::subtitles_font_name() const
{
    return config_.str("subtitles", "font_name");
}

int32_t render_base::subtitles_font_size() const
{
    const auto size = config_.float_val("subtitles", "font_size") * subtitles_font_scale();
    return static_cast<int32_t>(size);
}

int32_t render_base::stream_state_font_size() const
{
    return 48;
}

color render_base::subtitles_font_color() const
{
    return config_.color("subtitles", "font_color");
}

color render_base::subtitles_bg_color() const
{
    return config_.color("subtitles", "bg_color");
}

float render_base::subtitles_font_scale() const
{
    return config_.float_val("subtitles", "font_scale");
}

float render_base::subtitles_relative_ypos() const
{
    return config_.float_val("subtitles", "relative_ypos");
}

bool render_base::subtitles_has_background() const
{
    return config_.bool_val("subtitles", "has_background");
}

bool render_base::subtitles_show() const
{
    return config_.bool_val("subtitles", "show");
///    return (player_.seek_state() == seek_state_t::ready) &&
    ///            config_.bool_val("subtitles", "show");
}

locale::translator& render_base::tr()
{
    return player_.tr();
}

bool render_base::subtitle_within_display_time(const cpaf::video::subtitle_frame& subtitle) const
{
    return subtitle.subtitle_within_display_time(player_.cur_media_time().subtitles_time_pos());
}

void render_base::create_frame_display()
{
    frame_display_ = video_codec_ctx().create_scaling_dst_frame();
}

//void render_base::on_configuration_changed()
//{
//    on_render_geometry_changed();
//}

} //END namespace cpaf::gui::video
