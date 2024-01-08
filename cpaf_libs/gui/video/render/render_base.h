#pragma once

#include <memory>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/av_frame.h>
#include <cpaf_libs/video/subtitle_frame.h>
#include <cpaf_libs/gui/gui_types.h>
#include <cpaf_libs/gui/color.h>
#include <cpaf_libs/gui/fonts/font_size.h>


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
    void                        subtitle_font_set           (std::string font_name, uint16_t font_size_points);

    void                        init                        (const system_window& win,
                                                             const cpaf::video::surface_dimensions_t& dimensions );
    void                        init_only_for_old_playground(std::shared_ptr<cpaf::gui::system_render> sys_renderer,
                                                             const cpaf::video::surface_dimensions_t& dimensions );
    void                        ff_pixel_format_set         (AVPixelFormat pf)              { ff_pixel_format_ = pf;        }
    AVPixelFormat               ff_pixel_format             () const                        { return ff_pixel_format_;      }

    const cpaf::video::surface_dimensions_t& render_dimensions           () const                   { return render_dimensions_;      }

    void                        clear_screen                ()                              { do_clear_screen();  }
    bool                        render_video_frame          (const cpaf::video::av_frame& frame);
    void                        render_subtitle             (const cpaf::video::subtitle_frame& subtitle);

    bool                        show_subtitles              () const                        { return show_subtitles_;   }
    void                        show_subtitles_set          (bool show)                     { show_subtitles_ = show;   }
    bool                        subtitles_has_background    () const                        { return subtitles_has_background_;   }
    void                        subtitles_has_background_set(bool has_bgr)                  { subtitles_has_background_ = has_bgr;   }

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

    std::string                             subtitles_font_name_        {"manrope"};
    color                                   subtitles_text_color_       {1,1,1,1};
    color                                   subtitles_bg_color_         {0,0,0,1};
    uint16_t                                subtitles_font_size_points_ {16};
    uint16_t                                subtitles_create_dist_      {3}; // Create a new subtitle if dist in pixels is greater then this
    float                                   subtitles_relative_ypos_    {0.95};
    float                                   subtitles_line_dist_        {0.25};
    bool                                    show_subtitles_             {true};
    bool                                    subtitles_has_background_   {true};


    cpaf::video::subtitle_frame             current_subtitle_frame_     {};
    const system_window*                    main_window_ptr_            = nullptr;
private:
    cpaf::video::av_format_context*         format_context_ptr_         = nullptr;
    cpaf::video::av_codec_context*          video_codec_ctx_ptr_        = nullptr;
    cpaf::video::media_stream_time*         current_media_time_ptr_     = nullptr;
    AVPixelFormat                           ff_pixel_format_            = AV_PIX_FMT_YUV420P;
    pos_2df                                 subtitle_relative_pos_      {0.5, 0.9};
    render_geometry_t                       render_geometry_            {};

    void                        create_frame_display                ();

    virtual void                do_init                             (const system_window& win, const cpaf::video::surface_dimensions_t& dimensions ) = 0;
    virtual void                do_init                             (std::shared_ptr<cpaf::gui::system_render> sys_renderer, const cpaf::video::surface_dimensions_t& dimensions ) = 0;
    virtual void                do_render_dimensions_set            (const cpaf::video::surface_dimensions_t& dimensions ) = 0;
    virtual void                do_clear_screen                     () = 0;
    virtual bool                do_render_video_frame               (const cpaf::video::av_frame& frame) = 0;
    virtual void                on_render_geometry_changed          () = 0;
    virtual void                do_render_subtitle                  () = 0;

};

} //END namespace cpaf::gui::video
