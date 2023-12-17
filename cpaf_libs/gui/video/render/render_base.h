#pragma once

#include <memory>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/av_frame.h>
#include <cpaf_libs/video/subtitle_frame.h>
#include <cpaf_libs/gui/gui_types.h>
#include <cpaf_libs/gui/color.h>


namespace cpaf::gui {
class system_window;
class system_render;
}

namespace cpaf::video {
class av_format_context;
class av_codec_context;
class media_stream_time;
struct platform_render_t;
struct platform_surface_t;
};

namespace cpaf::gui::video {
struct platform_render_t;
struct platform_texture_t;

enum class subtitle_pos_t { buttom, top };

class render_base
{
public:
    virtual ~render_base() = default;
    render_base();
    render_geometry_t           render_geometry             () const { return render_geometry_; }
    void                        render_geometry_set         (render_geometry_t render_geom);
    void                        format_context_set          (cpaf::video::av_format_context* ctx)        { format_context_ptr_ = ctx; }
    void                        format_context_set          (cpaf::video::av_format_context& ctx)        { format_context_ptr_ = &ctx; }
    void                        video_codec_ctx_set         (cpaf::video::av_codec_context* ctx);
    void                        video_codec_ctx_set         (cpaf::video::av_codec_context& ctx);
    void                        subtitle_color_set          (const color& text_color, const color& bg_color);
    void                        subtitle_font_set           (std::string font_name, uint16_t font_size);

    void                        init                        (const system_window& win,
                                                             const cpaf::video::surface_dimensions_t& dimensions );
    void                        init                        (std::shared_ptr<cpaf::gui::system_render> sys_renderer,
                                                             const cpaf::video::surface_dimensions_t& dimensions );
    void                        ff_pixel_format_set         (AVPixelFormat pf)              { ff_pixel_format_ = pf;        }
    AVPixelFormat               ff_pixel_format             () const                        { return ff_pixel_format_;      }

    const cpaf::video::surface_dimensions_t& render_dimensions           () const                   { return render_dimensions_;      }

    bool                        render_video_frame          (const cpaf::video::av_frame& frame)    { return do_render_video_frame(frame);  }
    void                        render_subtitle             (const cpaf::video::subtitle_frame& subtitle);

//    void                        render_subtitle             (std::string_view str)                  { do_render_subtitle(str);  }

    /// @todo Currently unused, See render_geometry_set()
    void                        render_dimensions_set       (const cpaf::video::surface_dimensions_t& dimensions ) { return do_render_dimensions_set(dimensions);  }

protected:
    cpaf::video::av_format_context&         format_context              () { return *format_context_ptr_; }
    const cpaf::video::av_format_context&   format_context              () const { return *format_context_ptr_; }
    cpaf::video::av_codec_context&          video_codec_ctx             () { return *video_codec_ctx_ptr_; }
    cpaf::video::av_frame&                  frame_display               () { return frame_display_; }
    cpaf::video::av_frame                   frame_display_;
    cpaf::video::surface_dimensions_t       render_dimensions_;
    pos_2df                                 subtitle_pos                () const;

    bool                                    show_subtitles_         {true};
    cpaf::video::subtitle_frame             current_subtitle_frame_ {};
    color                                   subtitle_text_color_    {1,1,1,1};
    color                                   subtitle_bg_color_      {0,0,0,1};
    std::string                             font_name_              {"manrope"};
    uint16_t                                font_size_              {28};
    float                                   subtitle_line_dist_     {0.25};
    float                                   subtitle_relative_ypos_ {0.92};

private:
    cpaf::video::av_format_context*         format_context_ptr_     = nullptr;
    cpaf::video::av_codec_context*          video_codec_ctx_ptr_    = nullptr;
    cpaf::video::media_stream_time*         current_media_time_ptr_ = nullptr;
    AVPixelFormat                           ff_pixel_format_        = AV_PIX_FMT_YUV420P;
    pos_2df                                 subtitle_relative_pos_  {0.5, 0.9};
    render_geometry_t                       render_geometry_        {};

    void                        create_frame_display                ();

    virtual void                do_init                             (const system_window& win, const cpaf::video::surface_dimensions_t& dimensions ) = 0;
    virtual void                do_init                             (std::shared_ptr<cpaf::gui::system_render> sys_renderer, const cpaf::video::surface_dimensions_t& dimensions ) = 0;
    virtual void                do_render_dimensions_set            (const cpaf::video::surface_dimensions_t& dimensions ) = 0;
    virtual bool                do_render_video_frame               (const cpaf::video::av_frame& frame) = 0;
    virtual void                do_render_subtitle                  (std::string_view str) = 0;
    virtual void                on_subtitle_changed                 () = 0;
    virtual void                do_render_subtitle                  () = 0;

};

} //END namespace cpaf::gui::video
