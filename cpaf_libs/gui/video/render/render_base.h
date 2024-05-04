#pragma once

#include <memory>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/locale/translator.h>
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
class player;
class config;

//struct platform_render_t;
//struct platform_texture_t;


enum class subtitle_pos_t { buttom, top };

class render_base
{
public:
    using subtitle_frame = cpaf::video::subtitle_frame;
    using surface_dimensions_t = cpaf::video::surface_dimensions_t;
    explicit render_base(player& owning_player, config& cfg);
    rect                        render_geometry             () const { return render_geometry_; }
    rect                        video_render_geometry       () const { return video_render_geometry_; }
    void                        render_geometry_set         (const rect& render_geom);
    void                        init                        (const system_window& win,
                                                             const rect& render_geom,
                                                             const surface_dimensions_t& video_src_dimensions);
    void                        ff_pixel_format_set         (AVPixelFormat pf)              { ff_pixel_format_ = pf;        }
    AVPixelFormat               ff_pixel_format             () const                        { return ff_pixel_format_;      }

    void                        clear_screen                ()                              { do_clear_screen();  }
    bool                        render_video_frame          (const cpaf::video::av_frame& frame);
    const subtitle_frame&       current_subtitle            () const { return current_subtitle_frame_; }
    void                        clear_current_subtitle      ();
    void                        set_current_subtitle        (cpaf::video::subtitle_frame&& subtitle);

    std::string                 subtitles_font_name         () const;
    int32_t                     subtitles_font_size         () const;
    int32_t                     stream_state_font_size      () const;
    color                       subtitles_font_color        () const;
    color                       subtitles_bg_color          () const;
    float                       subtitles_font_scale        () const;
    float                       subtitles_relative_ypos     () const;
    bool                        subtitles_has_background    () const;
    bool                        subtitles_show              () const;
    bool                        keep_aspect_ratio           () const;

protected:
    static constexpr float                  general_margin              {4};

    cpaf::locale::translator&               tr                          ();
    bool                                    subtitle_within_display_time(const cpaf::video::subtitle_frame& subtitle) const;
    const surface_dimensions_t&             video_src_dimensions        () const { return video_src_dimensions_; }
    void                                    update_video_render_geometry();

//    void                                    on_configuration_changed    ();

    player&                                 player_;
    config&                                 config_;
    uint16_t                                subtitles_create_dist_      {3}; // Create a new subtitle if dist in pixels is greater then this
    float                                   subtitles_line_dist_        {0.25};

    cpaf::video::subtitle_frame             current_subtitle_frame_     {};
    const system_window*                    main_window_ptr_            = nullptr;
private:
    AVPixelFormat                           ff_pixel_format_            = AV_PIX_FMT_YUV420P;
    rect                                    render_geometry_            {};
    rect                                    video_render_geometry_      {};
    cpaf::video::surface_dimensions_t       video_src_dimensions_       {0,0};

    virtual void                do_init                             (const system_window& win) = 0;
    virtual void                do_init                             (std::shared_ptr<cpaf::gui::system_render> sys_renderer) = 0;
    virtual void                do_clear_screen                     () = 0;
    virtual bool                do_render_video_frame               (const cpaf::video::av_frame& frame) = 0;
    virtual void                on_render_geometry_changed          () = 0;
    virtual void                on_subtitle_changed                 () = 0;

};

} //END namespace cpaf::gui::video
