#include "render__sdl2.h"
#include <fmt/format.h>
#include "render.h"
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/system_render.h>
#include <cpaf_libs/video/av_codec_context.h>
#include <cpaf_libs/gui/assets/fonts/imgui_fonts.h>

using namespace std;

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
            dimensions.x(),
            dimensions.y()
        );
    }
}

void render_platform::calc_subtitle_geometry()
{
    if ( !(current_subtitle_frame_.should_show() && show_subtitles_) ) {
        return;
    }
///    font_size_ = 28; // FIXMENM
///    subtitle_bg_color_.w() = 0; // FIXMENM

    const int32_t font_size_pixels = font_size::to_pixels(subtitles_font_size_points_, main_window_ptr_);
//    const int32_t font_size_pixels = subtitles_font_size_points_;
    const ImFont* font = imgui_fonts::instance().get(subtitles_font_name_, font_size_pixels, subtitles_create_dist_);
    if (!font) { return; }

    const float line_dist = subtitles_line_dist_*font_size_pixels;
    const float x_pos = render_geometry().size.width() / 2;
    const float lowest_y = subtitles_relative_ypos_* render_geometry().size.height();
    const float max_width = render_geometry().size.width();
    const size_t lines_count = current_subtitle_frame_.lines_count();
    const size_t max_line_index = lines_count -1;
    for (size_t sub_index = 0; sub_index < lines_count; ++sub_index) {
        const std::string& line = current_subtitle_frame_.lines[sub_index];
        auto& geom = subtitle_render_geometries_[sub_index];
        geom.top_left.y(lowest_y - (max_line_index - sub_index)*(font_size_pixels+line_dist));
        geom.top_left.x(x_pos);

        auto render_size = font->CalcTextSizeA(font->FontSize, max_width, 0, line.data(), line.data() + line.size());
        render_size.x = render_size.x + 3*(render_size.x/line.size());
        ///        render_size.y = render_size.y*1.1;
        geom.size = {render_size.x, render_size.y};
    }
}

void render_platform::calc_controls_geometry()
{
    const float x_pos = render_geometry().size.width() / 2;
    const float y_pos = controls_relative_ypos_* render_geometry().size.height();
    controls_render_geometry_.size = render_geometry().size*0.9;
    controls_render_geometry_.top_left = {x_pos, y_pos};
}

SDL_Rect render_platform::to_sdl_rect(render_geometry_t geom)
{
    return SDL_Rect{static_cast<int>(geom.top_left.x()),
                    static_cast<int>(geom.top_left.y()),
                    static_cast<int>(geom.size.width()),
                    static_cast<int>(geom.size.height())};
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

bool render_platform::do_render_video_frame(const cpaf::video::av_frame& frame)
{
    prepare_native_video_frame(frame, frame_display());
    render_current_native_video_frame_texture();
    return true;
}

void render_platform::on_render_geometry_changed()
{
    calc_subtitle_geometry();
    calc_controls_geometry();
}

void render_platform::do_render_subtitle()
{
    if ( !(current_subtitle_frame_.should_show() && show_subtitles_) ) {
        return;
    }
    const int32_t font_size_pixels = font_size::to_pixels(subtitles_font_size_points_, main_window_ptr_);
    ImFont* font = imgui_fonts::instance().get(subtitles_font_name_, font_size_pixels, subtitles_create_dist_);
    if (!font) { return; }


    ImGui::PushStyleColor(ImGuiCol_Border, reinterpret_cast<const ImVec4&>(subtitles_bg_color_));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, reinterpret_cast<const ImVec4&>(subtitles_bg_color_));
    ImGui::PushFont(font);
    ImGui::PushStyleColor(ImGuiCol_Text, reinterpret_cast<const ImVec4&>(subtitles_text_color_));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(4, 4)); // Just something smaller than what we would realistictly use as font size!

    for (size_t sub_index = current_subtitle_frame_.lines_count(); sub_index > 0; ) {
        --sub_index;
        const string window_name = "subtitle"s + std::to_string(sub_index);
        const std::string& line = current_subtitle_frame_.lines[sub_index];
        const auto& geom = subtitle_render_geometries_[sub_index];

        ImGui::SetNextWindowPos({geom.top_left.x(), geom.top_left.y()}, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
        ImGui::SetNextWindowSize({geom.size.width(), geom.size.height()}, ImGuiCond_::ImGuiCond_Always);
        ImGui::Begin(window_name.c_str(), &show_subtitles_, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoInputs);
        ImGui::SetCursorPosY(0);
        ImGui::TextUnformatted(line.c_str());
        ImGui::End();

     }

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

void render_platform::do_render_controls()
{
    return;
    bool show_controls = true;
    if ( !show_controls ) {
        return;
    }
    const int32_t font_size_pixels = font_size::to_pixels(controls_font_size_points_, main_window_ptr_);
    ImFont* font = imgui_fonts::instance().get(controls_font_name_, font_size_pixels, subtitles_create_dist_);
    if (!font) { return; }

    static int counter = 0;

    ImGui::PushStyleColor(ImGuiCol_Border, reinterpret_cast<const ImVec4&>(controls_bg_color_));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, reinterpret_cast<const ImVec4&>(controls_bg_color_));
    ImGui::PushFont(font);
    ImGui::PushStyleColor(ImGuiCol_Text, reinterpret_cast<const ImVec4&>(controls_text_color_));
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(4, 4)); // Just something smaller than what we would realistictly use as font size!


   // float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
   const string window_name = "controls";
   ImGui::Begin(window_name.c_str(), &show_controls, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);
   ImGui::PushButtonRepeat(true);
   if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { counter--; }
   // ImGui::SameLine(0.0f, spacing);
   // if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { counter++; }
   ImGui::PopButtonRepeat();
   // ImGui::SameLine();
   // ImGui::Text("%d", counter);
   ImGui::End();

    // ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();


}

} //END namespace cpaf::gui::video
