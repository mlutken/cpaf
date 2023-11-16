#pragma once

#include <memory>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/av_frame.h>


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


class render_base
{
public:
    virtual ~render_base() = default;
    render_base();
    void                        format_context_set          (cpaf::video::av_format_context* ctx)        { format_context_ptr_ = ctx; }
    void                        format_context_set          (cpaf::video::av_format_context& ctx)        { format_context_ptr_ = &ctx; }
    void                        video_codec_ctx_set         (cpaf::video::av_codec_context* ctx);
    void                        video_codec_ctx_set         (cpaf::video::av_codec_context& ctx);
    void                        init                        (system_window& win,
                                                             const cpaf::video::surface_dimensions_t& dimensions );
    void                        init                        (std::shared_ptr<cpaf::gui::system_render> sys_renderer,
                                                             const cpaf::video::surface_dimensions_t& dimensions );
    // void                        init                        (const platform_render_t& platform_render,
    //                                                          const cpaf::video::surface_dimensions_t& dimensions );

    void                        ff_pixel_format_set         (AVPixelFormat pf)              { ff_pixel_format_ = pf;        }
    AVPixelFormat               ff_pixel_format             () const                        { return ff_pixel_format_;      }

    const cpaf::video::surface_dimensions_t& render_dimensions           () const                        { return render_dimensions_;      }

    void                        platform_render_set         (const platform_render_t& platform_render)  { do_platform_render_set(platform_render); }
    bool                        render_video_frame          (const cpaf::video::av_frame& frame)        { return do_render_video_frame(frame);  }

    platform_render_t&          platform_render             () { return do_platform_render();   }
    void                        render_dimensions_set       (const cpaf::video::surface_dimensions_t& dimensions ) { return do_render_dimensions_set(dimensions);  }

protected:
    cpaf::video::av_format_context&         format_context              () { return *format_context_ptr_; }
    const cpaf::video::av_format_context&   format_context              () const { return *format_context_ptr_; }
    cpaf::video::av_codec_context&          video_codec_ctx             () { return *video_codec_ctx_ptr_; }
    cpaf::video::av_frame&                  frame_display               () { return frame_display_; }
    cpaf::video::av_frame                   frame_display_;
    cpaf::video::surface_dimensions_t       render_dimensions_;

private:
    cpaf::video::av_format_context*         format_context_ptr_     = nullptr;
    cpaf::video::av_codec_context*          video_codec_ctx_ptr_    = nullptr;
    cpaf::video::media_stream_time*         current_media_time_ptr_ = nullptr;
    AVPixelFormat                           ff_pixel_format_        = AV_PIX_FMT_YUV420P;

    void                        create_frame_display            ();

    virtual void                do_init                         (system_window& win, const cpaf::video::surface_dimensions_t& dimensions ) = 0;
    virtual void                do_init                         (std::shared_ptr<cpaf::gui::system_render> sys_renderer, const cpaf::video::surface_dimensions_t& dimensions ) = 0;
    virtual void                do_init                         (const platform_render_t& platform_render, const cpaf::video::surface_dimensions_t& dimensions ) = 0;
    virtual void                do_platform_render_set          (const platform_render_t& platform_render) = 0;
    virtual platform_render_t&  do_platform_render              () = 0;
    virtual void                do_render_dimensions_set        (const cpaf::video::surface_dimensions_t& dimensions ) = 0;
    virtual bool                do_render_video_frame           (const cpaf::video::av_frame& frame) = 0;

};

} //END namespace cpaf::gui::video
