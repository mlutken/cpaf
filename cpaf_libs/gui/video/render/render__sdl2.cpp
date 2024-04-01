#include "render__sdl2.h"
#include <fmt/format.h>
#include "render.h"
#include <imguipp/imgui_rai.h>
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/system_render.h>
#include <cpaf_libs/video/av_codec_context.h>
#include <cpaf_libs/gui/assets/fonts/imgui_fonts.h>
#include <cpaf_libs/gui/platform_utils/sdl_convert.h>
#include <cpaf_libs/gui/video/player.h>
#include <cpaf_libs/gui/video/config.h>

using namespace std;
using namespace cpaf::video;

namespace cpaf::gui::video {

std::unique_ptr<render> render_platform::create_video_render(
    player& owning_player,
    config& cfg,
    const cpaf::gui::system_window& win,
    const cpaf::video::surface_dimensions_t& dimensions)
{
    auto video_renderer = std::make_unique<render>(owning_player, cfg);
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
    // the area of the texture to be updated
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = render_dimensions().x();
    r.h = render_dimensions().y();

    // update the texture with the new pixel data
    SDL_UpdateYUVTexture(
        sdl_frame_render_texture_,
        &r,
        frame_display.ff_frame()->data[0],
        frame_display.ff_frame()->linesize[0],
        frame_display.ff_frame()->data[1],
        frame_display.ff_frame()->linesize[1],
        frame_display.ff_frame()->data[2],
        frame_display.ff_frame()->linesize[2]
    );

    SDL_SetRenderTarget( get_sdl_renderer(), sdl_frame_render_texture_ );
}

void render_platform::fill_native_subtitle_texture()
{
    if ( !(current_subtitle_frame_.is_valid() && current_subtitle_frame_.format() == subtitle_frame::format_t::graphics)) {
        return;
    }
    pixel_rgba_t* dst_pixels;
    [[maybe_unused]]int pitch;

    SDL_LockTexture( sdl_subtitles_render_texture_, nullptr, (void**)&dst_pixels, &pitch );

    const uint8_t* src_pixels = current_subtitle_frame_.ff_pixel_data();
    const auto pixel_count = current_subtitle_frame_.ff_bitmap_pixel_count();
    const pixel_rgba_t* color_map = current_subtitle_frame_.ff_pixel_color_map();

    for(uint32_t i = 0; i < pixel_count; ++i) {
        const auto color_index = src_pixels[i];
        auto src_pixel = color_map[color_index];
        dst_pixels[i] = src_pixel;
    }

    SDL_UnlockTexture(sdl_subtitles_render_texture_);
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
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        subtitle.ff_rect_w(),
        subtitle.ff_rect_h()
        );
    SDL_SetTextureBlendMode(sdl_frame_render_texture_, SDL_BLENDMODE_BLEND);
}

void render_platform::calc_subtitle_geometry()
{
    if ( !(current_subtitle_frame_.is_valid() && subtitles_show()) ) {
        return;
    }

    const int32_t font_size_pixels = font_size::to_pixels(subtitles_font_size(), main_window_ptr_);
    const ImFont* font = imgui_fonts::instance().get(subtitles_font_name(), font_size_pixels, subtitles_create_dist_);
    if (!font) { return; }

    const float line_dist = subtitles_line_dist_*font_size_pixels;
    const float x_pos = render_geometry().size().width() / 2;
    const float lowest_y = subtitles_relative_ypos() * render_geometry().size().height();
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
        const auto scale = (render_geometry().size() / player_.video_src_dimensions_float())*subtitles_font_scale();
        const auto subtitle_size = current_subtitle_frame_.ff_bitmap_rect().size()*scale;
        const auto subtitle_rect = rect({x_pos - subtitle_size.width()*0.5f,lowest_y - subtitle_size.height()}, subtitle_size);
        subtitles_dst_rect_ = to_sdl_rect(subtitle_rect);
    }
}


SDL_Renderer* render_platform::get_sdl_renderer() {
    return system_renderer_->native_renderer<SDL_Renderer>();
}

void render_platform::render_subtitle_text()
{
    bool show_subtitles = subtitles_show();
    if (!show_subtitles) {
        return;
    }

    const int32_t font_size_pixels = font_size::to_pixels(subtitles_font_size(), main_window_ptr_);
    ImFont* font = imgui_fonts::instance().get(subtitles_font_name(), font_size_pixels, subtitles_create_dist_);
    if (!font) { return; }

    auto window_flags = ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoInputs;
    if (!subtitles_has_background()) {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    const color subtitles_bg_col = subtitles_bg_color();
    const color subtitles_font_col = subtitles_font_color();
    ImGui::Rai imrai;
    imrai.Font(font)
        .StyleColor(ImGuiCol_Border, reinterpret_cast<const ImVec4&>(subtitles_bg_col))
        .StyleColor(ImGuiCol_WindowBg, reinterpret_cast<const ImVec4&>(subtitles_bg_col))
        .StyleColor(ImGuiCol_Text, reinterpret_cast<const ImVec4&>(subtitles_font_col))
        .StyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(4, 4))
        ;


    for (size_t sub_index = current_subtitle_frame_.lines_count(); sub_index > 0; ) {
        --sub_index;
        const string window_name = "subtitle"s + std::to_string(sub_index);
        const std::string& line = current_subtitle_frame_.lines[sub_index];
        const auto& geom = subtitle_render_geometries_[sub_index];

        ImGui::SetNextWindowPos({geom.top_left().x(), geom.top_left().y()}, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        ImGui::SetNextWindowSize({geom.size().width(), geom.size().height()}, ImGuiCond_::ImGuiCond_Always);
        ImGui::Begin(window_name.c_str(), &show_subtitles, window_flags);
        ImGui::SetCursorPosY(0);
        ImGui::TextUnformatted(line.c_str());
        ImGui::End();
    }

}

void render_platform::render_subtitle_graphics()
{
    if (!sdl_subtitles_render_texture_) {
        return;
    }

    SDL_RenderCopy(get_sdl_renderer(), sdl_subtitles_render_texture_, NULL, &subtitles_dst_rect_);
    SDL_SetRenderTarget( get_sdl_renderer(), nullptr );
}


// -------------------------
// --- Platform overides ---
// -------------------------

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
    fill_native_video_frame(frame, frame_display());

    auto dst_rect = to_sdl_rect(render_geometry());
    SDL_RenderCopy(get_sdl_renderer(), sdl_frame_render_texture_, NULL, &dst_rect);

    if (current_subtitle_frame_.format() == subtitle_frame::format_t::text) {
        render_subtitle_text();
    }
    else if (current_subtitle_frame_.format() == subtitle_frame::format_t::graphics) {
        render_subtitle_graphics();
    }


//    // FIXMENM DEBUG ONLY BEGIN
//    SDL_SetRenderDrawBlendMode( get_sdl_renderer(), SDL_BLENDMODE_BLEND );
//    SDL_SetRenderDrawColor( get_sdl_renderer(), 255, 255, 255, 255 ); // Set color to solid white
//    SDL_RenderDrawLine( get_sdl_renderer(), 0, 0, 300, 300 );
//    system_renderer_->set_color(color(0, 1, 0, 0.5));
//    system_renderer_->fill_rect(rect(100,100,200,300));
//    // FIXMENM DEBUG ONLY END

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

} //END namespace cpaf::gui::video
