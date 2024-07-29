#include "render__godot.h"
#include <fmt/format.h>
#include "render.h"
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/system_render.h>
#include <cpaf_libs/video/av_codec_context.h>
#include <cpaf_libs/gui/video/player.h>
#include <cpaf_libs/gui/video/config.h>

using namespace std;
using namespace cpaf::video;

namespace cpaf::gui::video {

    std::unique_ptr<render> render_platform::create_video_render(
        player& owning_player,
        config& cfg,
        const cpaf::gui::system_window& win,
        const rect& render_geom,
        const cpaf::video::surface_dimensions_t& video_src_dimensions)
    {
        auto video_renderer = std::make_unique<render>(owning_player, cfg);
        video_renderer->init(win, render_geom, video_src_dimensions);

        return video_renderer;
}

render_platform::~render_platform()
{
}

render_platform::render_platform(player& owning_player, config& cfg)
    : render_base(owning_player, cfg)
{

}


void render_platform::fill_native_video_frame(
    const cpaf::video::av_frame& frame,
    cpaf::video::av_frame& frame_display
)
{
    video_codec_ctx().scale_video_frame(frame_display, frame);

    // // the area of the texture to be updated
    // SDL_Rect r;
    // r.x = 0;
    // r.y = 0;
    // r.w = video_src_dimensions().x();
    // r.h = video_src_dimensions().y();

    // // update the texture with the new pixel data

    // SDL_UpdateYUVTexture(
    //     sdl_frame_render_texture_,
    //     &r,
    //     frame_display.ff_frame()->data[0],
    //     frame_display.ff_frame()->linesize[0],
    //     frame_display.ff_frame()->data[1],
    //     frame_display.ff_frame()->linesize[1],
    //     frame_display.ff_frame()->data[2],
    //     frame_display.ff_frame()->linesize[2]
    // );

    // // SDL_UpdateYUVTexture(
    // //     sdl_frame_render_texture_,
    // //     &r,
    // //     frame.ff_frame()->data[0],
    // //     frame.ff_frame()->linesize[0],
    // //     frame.ff_frame()->data[1],
    // //     frame.ff_frame()->linesize[1],
    // //     frame.ff_frame()->data[2],
    // //     frame.ff_frame()->linesize[2]
    // //     );

    // SDL_SetRenderTarget( get_sdl_renderer(), sdl_frame_render_texture_ );
}

void render_platform::fill_native_subtitle_texture()
{
    if ( !(current_subtitle_frame_.is_valid() && current_subtitle_frame_.format() == subtitle_frame::format_t::graphics)) {
        return;
    }
}

void render_platform::ensure_valid_render_texture()
{
}

void render_platform::ensure_valid_subtitles_graphics_texture(const subtitle_frame& subtitle)
{
    if ( !(subtitle.is_valid() && subtitle.format() == subtitle_frame::format_t::graphics)) {
        return;
    }
}

void render_platform::calc_subtitle_geometry()
{
    if ( !(current_subtitle_frame_.is_valid() && subtitles_show()) ) {
        return;
    }
}

void render_platform::render_subtitle_text()
{
    bool show_subtitles = subtitles_show();
    if (!show_subtitles) {
        return;
    }

}

void render_platform::render_subtitle_graphics()
{
    if (!subtitles_show()) {
        return;
    }
}



// -------------------------
// --- Platform overides ---
// -------------------------

void render_platform::do_init(const system_window& win)
{
    system_renderer_ = win.renderer_shared();
    ensure_valid_render_texture();
}

void render_platform::do_init(std::shared_ptr<system_render> sys_renderer)
{
    system_renderer_ = sys_renderer;
    ensure_valid_render_texture();
}

void render_platform::do_clear_screen()
{
}

bool render_platform::do_render_video_frame(const cpaf::video::av_frame& frame)
{
    fill_native_video_frame(frame, frame_display());

    // const auto video_dst_rect = to_sdl_rect(video_render_geometry());
    // SDL_RenderCopy(get_sdl_renderer(), sdl_frame_render_texture_, NULL, &video_dst_rect);

    // if (subtitle_within_display_time(current_subtitle_frame_)) {
    //     if (current_subtitle_frame_.format() == subtitle_frame::format_t::text) {
    //         render_subtitle_text();
    //     }
    //     else if (current_subtitle_frame_.format() == subtitle_frame::format_t::graphics) {
    //         render_subtitle_graphics();
    //     }
    // }

    return true;
}


void render_platform::on_render_geometry_changed()
{
    calc_subtitle_geometry();
    ensure_valid_subtitles_graphics_texture(current_subtitle_frame_);
    fill_native_subtitle_texture();
}

void render_platform::on_subtitle_changed()
{

}


// if (keep_aspect_ratio_) {
//     const float wfac = static_cast<float>(video_src_dimensions().width()) / static_cast<float>(dimensions.width());
//     const float hfac = static_cast<float>(video_src_dimensions().height()) / static_cast<float>(dimensions.height());
//     dimensions = video_src_dimensions().uniform_scale_x(dimensions.x());
//     // if (wfac > hfac) {
//     //     dimensions = video_src_dimensions().uniform_scale_x(dimensions.x());
//     // }
//     // else {
//     //     dimensions = video_src_dimensions().uniform_scale_y(dimensions.y());
//     // }
// }



} //END namespace cpaf::gui::video
