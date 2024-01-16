#include "render__sdl2.h"
#include <fmt/format.h>
#include "render.h"
#include <imguipp/imgui_rai.h>
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/system_render.h>
#include <cpaf_libs/video/av_codec_context.h>
#include <cpaf_libs/gui/assets/fonts/imgui_fonts.h>

#include <cpaf_libs/gui/platform_utils/sdl_convert.h>

using namespace std;
using namespace cpaf::video;

namespace cpaf::gui::video {

std::unique_ptr<render> render_platform::create_video_render(
    const cpaf::gui::system_window& win,
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
    if (sdl_subtitles_render_texture_) {
        SDL_DestroyTexture(sdl_subtitles_render_texture_);
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
    auto dst_rect = to_sdl_rect(render_geometry());
    SDL_RenderCopy(get_sdl_renderer(), sdl_frame_render_texture_, NULL, &dst_rect);
}

void render_platform::ensure_valid_render_texture(const cpaf::video::surface_dimensions_t& dimensions)
{
    if (dimensions != render_dimensions_) {
        if (sdl_frame_render_texture_) {
            SDL_DestroyTexture(sdl_frame_render_texture_);
        }
        render_dimensions_ = dimensions;

        sdl_frame_render_texture_ = SDL_CreateTexture(
            get_sdl_renderer(),
            SDL_PIXELFORMAT_YV12,
            SDL_TEXTUREACCESS_STREAMING,
            dimensions.width(),
            dimensions.height()
        );
        SDL_SetTextureBlendMode(sdl_frame_render_texture_, SDL_BLENDMODE_BLEND);
    }
}

void render_platform::ensure_valid_subtitles_graphics_texture(const subtitle_frame& subtitle)
{
    if ( !(subtitle.is_valid() && subtitle.format() == subtitle_frame::format_t::graphics)) {
        return;
    }
    if (sdl_subtitles_render_texture_) {
        SDL_DestroyTexture(sdl_subtitles_render_texture_);
    }

    sdl_subtitles_render_texture_ = SDL_CreateTexture(
        get_sdl_renderer(),
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        subtitle.ff_rect_w(),
        subtitle.ff_rect_h()
        );
    SDL_SetTextureBlendMode(sdl_frame_render_texture_, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget( get_sdl_renderer(), sdl_subtitles_render_texture_ );
    SDL_SetRenderTarget( get_sdl_renderer(), nullptr );


}

/// For text subtitles only
void render_platform::calc_subtitle_geometry()
{
    if ( !(current_subtitle_frame_.is_valid() && show_subtitles_) ) {
        return;
    }

    const int32_t font_size_pixels = font_size::to_pixels(subtitles_font_size_points_, main_window_ptr_);
    const ImFont* font = imgui_fonts::instance().get(subtitles_font_name_, font_size_pixels, subtitles_create_dist_);
    if (!font) { return; }

    const float line_dist = subtitles_line_dist_*font_size_pixels;
    const float x_pos = render_geometry().size().width() / 2;
    const float lowest_y = subtitles_relative_ypos_* render_geometry().size().height();
    const float max_width = render_geometry().size().width();
    const size_t lines_count = current_subtitle_frame_.lines_count();
    const size_t max_line_index = lines_count -1;
    for (size_t sub_index = 0; sub_index < lines_count; ++sub_index) {
        const std::string& line = current_subtitle_frame_.lines[sub_index];
        auto& geom = subtitle_render_geometries_[sub_index];
        geom.top_left().y(lowest_y - (max_line_index - sub_index)*(font_size_pixels+line_dist));
        geom.top_left().x(x_pos);

        auto render_size = font->CalcTextSizeA(font->FontSize, max_width, 0, line.data(), line.data() + line.size());
        render_size.x = render_size.x + 3*(render_size.x/line.size());
        geom.size() = {render_size.x, render_size.y};
    }

    if (current_subtitle_frame_.format() == subtitle_frame::format_t::graphics && current_subtitle_frame_.ff_subtitle_is_valid() ) {
        subtitles_dst_rect_.h = current_subtitle_frame_.ff_rect_h();
        subtitles_dst_rect_.h = current_subtitle_frame_.ff_rect_w();
    }

}

SDL_Renderer* render_platform::get_sdl_renderer() {
    return system_renderer_->native_renderer<SDL_Renderer>();
}

void render_platform::do_init(const system_window& win, const cpaf::video::surface_dimensions_t& dimensions)
{
    system_renderer_ = win.renderer_shared();
    ensure_valid_render_texture(dimensions);
}

void render_platform::do_init(std::shared_ptr<system_render> sys_renderer, const cpaf::video::surface_dimensions_t& dimensions)
{
    system_renderer_ = sys_renderer;
    ensure_valid_render_texture(dimensions);
}

void render_platform::do_render_dimensions_set(const cpaf::video::surface_dimensions_t& dimensions)
{
    ensure_valid_render_texture(dimensions);
}

void render_platform::do_clear_screen()
{
    auto dst_rect = to_sdl_rect(render_geometry());
    SDL_RenderFillRect(get_sdl_renderer(), &dst_rect);
}

bool render_platform::do_render_video_frame(const cpaf::video::av_frame& frame)
{
    prepare_native_video_frame(frame, frame_display());
    render_current_native_video_frame_texture();

    // FIXMENM DEBUG ONLY BEGIN
//    SDL_SetRenderDrawBlendMode( get_sdl_renderer(), SDL_BLENDMODE_BLEND );
//    SDL_SetRenderDrawColor( get_sdl_renderer(), 255, 255, 255, 255 ); // Set color to solid white
//    SDL_RenderDrawLine( get_sdl_renderer(), 0, 0, 300, 300 );
//    system_renderer_->set_color(color(0, 1, 0, 0.5));
//    system_renderer_->fill_rect(render_geometry_t(100,100,200,300));
    // FIXMENM DEBUG ONLY END

    do_render_subtitle();
    return true;
}

void render_platform::on_render_geometry_changed()
{
    calc_subtitle_geometry();
    ensure_valid_subtitles_graphics_texture(current_subtitle_frame_);
}

void render_platform::on_subtitle_changed()
{

}

void render_platform::do_render_subtitle()
{
    if ( !(current_subtitle_frame_.should_show() && show_subtitles_) ) {
        return;
    }
    if (current_subtitle_frame_.format() == subtitle_frame::format_t::text) {
        render_subtitle_text();
    }
    else if (current_subtitle_frame_.format() == subtitle_frame::format_t::graphics) {
        render_subtitle_graphics();
    }
}

void render_platform::render_subtitle_text()
{
    const int32_t font_size_pixels = font_size::to_pixels(subtitles_font_size_points_, main_window_ptr_);
    ImFont* font = imgui_fonts::instance().get(subtitles_font_name_, font_size_pixels, subtitles_create_dist_);
    if (!font) { return; }

    auto window_flags = ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoInputs;
    if (!subtitles_has_background()) {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    ImGui::Rai imrai;
    imrai.Font(font)
        .StyleColor(ImGuiCol_Border, reinterpret_cast<const ImVec4&>(subtitles_bg_color_))
        .StyleColor(ImGuiCol_WindowBg, reinterpret_cast<const ImVec4&>(subtitles_bg_color_))
        .StyleColor(ImGuiCol_Text, reinterpret_cast<const ImVec4&>(subtitles_text_color_))
        .StyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(4, 4))
        ;


    for (size_t sub_index = current_subtitle_frame_.lines_count(); sub_index > 0; ) {
        --sub_index;
        const string window_name = "subtitle"s + std::to_string(sub_index);
        const std::string& line = current_subtitle_frame_.lines[sub_index];
        const auto& geom = subtitle_render_geometries_[sub_index];

        ImGui::SetNextWindowPos({geom.top_left().x(), geom.top_left().y()}, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        ImGui::SetNextWindowSize({geom.size().width(), geom.size().height()}, ImGuiCond_::ImGuiCond_Always);
        ImGui::Begin(window_name.c_str(), &show_subtitles_, window_flags);
        ImGui::SetCursorPosY(0);
        ImGui::TextUnformatted(line.c_str());
        ImGui::End();
    }

}

struct rgbaPixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

void render_platform::render_subtitle_graphics()
{
    if (!sdl_subtitles_render_texture_) {
        return;
    }

    SDL_Rect sdl_rect {50,50,150,150};

    SDL_RenderCopy(get_sdl_renderer(), sdl_subtitles_render_texture_, NULL, &sdl_rect);

    std::cerr << "FIXMENM render subtitle: " << current_subtitle_frame_.dbg_str() << "\n";
    std::cerr << "pixel_count: " << current_subtitle_frame_.ff_bitmap_pixel_count()
              << " num rects: " << current_subtitle_frame_.ff_num_rects()
              << "\n"
              << " w, h: " << current_subtitle_frame_.ff_rect(0).w << ", " << current_subtitle_frame_.ff_rect(0).h
              << "\n"
        ;
    AVSubtitle& sub = current_subtitle_frame_.ff_subtitle();



//    std::cerr << " w, h: " << sub. << "\n";

//    for (unsigned int i = 0; i < sub.num_rects; ++ i) {
//        AVSubtitleRect* rect = sub.rects[i];
//        for (int y = 0; y < rect->h; ++ y) {
//            int dest_y = y + rect->y;

//            // data[0] holds index data
//            uint8_t *in_linedata = rect->data[0] + y * rect->linesize[0];

//            // In AVFrame, data[0] holds the pixel buffer directly
//            uint8_t *out_linedata = frame->data[0] + dest_y * frame->linesize[0];
//            rgbaPixel *out_pixels = reinterpret_cast<rgbaPixel*>(out_linedata);

//            for (int x = 0; x < rect->w; ++ x) {
//                // data[1] contains the color map
//                // compare libavcodec/dvbsubenc.c
//                uint8_t colidx = in_linedata[x];
//                uint32_t color = reinterpret_cast<uint32_t*>(rect->data[1])[colidx];

//                // Now store the pixel in the target buffer
//                out_pixels[x + rect->x] = rgbaPixel{
//                    .r = static_cast<uint8_t>((color >> 16) & 0xff),
//                    .g = static_cast<uint8_t>((color >>  8) & 0xff),
//                    .b = static_cast<uint8_t>((color >>  0) & 0xff),
//                    .a = static_cast<uint8_t>((color >> 24) & 0xff),
//                };
//            }
//        }
//    }

}

} //END namespace cpaf::gui::video
