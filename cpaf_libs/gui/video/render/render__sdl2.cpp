#include "render__sdl2.h"
#include <fmt/format.h>
#include "render.h"
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/system_render.h>
#include <cpaf_libs/video/av_codec_context.h>
#include <cpaf_libs/gui/assets/fonts/built_in_fonts.h>

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
    // SDL_RenderClear(get_sdl_renderer()); // TODO: Perhaps not clear at this point but leave up to application!
    auto dst_rect = to_sdl_rect(render_geometry());
    SDL_RenderCopy(get_sdl_renderer(), sdl_frame_render_texture_, NULL, &dst_rect);
//    SDL_Rect dest_rect = sdl2::texture_destination_rect(image_bitmap_texture, 30, 30);
//    SDL_RenderCopy(sdl_renderer, image_bitmap_texture, NULL, &dest_rect);
    // SDL_RenderPresent(get_sdl_renderer());
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

void render_platform::render_subtitle_line(pos_2df pos, std::string_view str)
{

    ImGui::SetNextWindowPos({pos.x(), pos.y()}, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ImGui::SetNextWindowSize({400, 20}, ImGuiCond_::ImGuiCond_Always);

    ImGui::Begin("video_render_subtitle", &m_show_subtitle, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
    ImGui::PushStyleColor(ImGuiCol_Text, subtitle_color_);
//    ImGui::TextColored(ImVec4{255,255,255,1}, "%s", str.data());
    ImGui::Text("%s", str.data());
    ImGui::PopStyleColor(ImGuiCol_Text);
    ImGui::End();
}

ImVec2 render_platform::subtitle_line_geometry(ImFont* font, std::string_view line) const
{
//    static constexpr float max_relative_line_width = 0.8;

//    auto size = ImGui::CalcTextSize(line.data());

//    CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** remaining = NULL) const; // utf8

//    auto size = font->CalcTextSizeA( render_geometry_.size.width(), renderline.data(), line.data() + line.size());
    const float max_width = render_geometry().size.width();
//    const float max_width = std::numeric_limits<float>::max();

    auto size = font->CalcTextSizeA(font->FontSize, max_width, 0, line.data(), line.data() + line.size());
    size.x = size.x + (3*size.x)/line.size();
    size.y = size.y*1.1;

//    const std::string s("i");
//    auto size = font->CalcTextSizeA(font->FontSize, max_width, 0, s.c_str(), s.c_str() + s.size());
//    size.x = size.x * line.size();

//    std::cerr << "FIXMENM w, h: " << size.x << " ; " << size.y << "\n";
//    std::cerr << "FIXMENM max_width: " << max_width << "\n";
    //    size_2df size = size_2df (render_geometry_.size.width(), 20) * max_relative_line_width;

    return size;
}

SDL_Rect render_platform::to_sdl_rect(render_geometry_t geom)
{
    return SDL_Rect{geom.top_left.x(), geom.top_left.y(), geom.size.width(), geom.size.height()};
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

void render_platform::do_render_subtitle(std::string_view str)
{
    m_show_subtitle = !str.empty();

//    render_subtitle_line(subtitle_pos(), str);
//    return;

//    std::cerr << "FIXMENM render_subtitle: " << str << "\n";


    ImFont* font = imgui_fonts::get("manrope", 96);
    auto pos = subtitle_pos();
    auto line_size = subtitle_line_geometry(font, str);
//    ImGui::SetNextWindowPos({400, 500}, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ImGui::SetNextWindowPos({pos.x(), pos.y()}, ImGuiCond_::ImGuiCond_Always, {0.5, 0.5} );
    ImGui::SetNextWindowSize(line_size, ImGuiCond_::ImGuiCond_Always);

//    ImGui::Begin("video_render_subtitle", &m_show_subtitle, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
//    ImGui::TextColored(subtitle_color_, "%s", str.data());
//    ImGui::End();

    ImGui::PushStyleColor(ImGuiCol_Border, {0,0.5,0,1});
    ImGui::PushStyleColor(ImGuiCol_WindowBg, {0,0,0,0});
    ImGui::Begin("video_render_subtitle", &m_show_subtitle, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
//    ImGui::Begin("video_render_subtitle", &m_show_subtitle, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoInputs);

    ImGui::PushFont(font);
    ImGui::PushStyleColor(ImGuiCol_Text, subtitle_color_);
    ImGui::SetCursorPosY(0);
//    fmt::println("FIXMENM CursorPosY: {}", ImGui::GetCursorPosY());

//    std::cerr << "FIXMENM CursorPosY: " <<  ImGui::GetCursorPosY() << "\n";
//    ImGui::Text("%s", str.data());
    ImGui::TextUnformatted(str.data());
    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

}

void render_platform::on_subtitle_changed()
{

}

void render_platform::do_render_subtitle()
{

}

} //END namespace cpaf::gui::video
