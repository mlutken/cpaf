#pragma once

#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/av_frame.h>


namespace cpaf::video {
class av_format_context;
class av_codec_context;
class media_stream_time;
struct platform_render_t;
struct platform_surface_t;
struct platform_texture_t;
};

namespace cpaf::gui::video {
struct platform_render_t;
struct platform_surface_t;
struct platform_texture_t;


class render_base
{
public:
    render_base();
    void                        format_context_set          (cpaf::video::av_format_context* ctx)        { format_context_ptr_ = ctx; }
    void                        format_context_set          (cpaf::video::av_format_context& ctx)        { format_context_ptr_ = &ctx; }
    void                        video_codec_ctx_set         (cpaf::video::av_codec_context* ctx);
    void                        video_codec_ctx_set         (cpaf::video::av_codec_context& ctx);
    void                        init                        (const platform_render_t& platform_render,
                                                             const platform_texture_t& platform_texture,
                                                             const cpaf::video::surface_dimensions_t& dimensions );

    void                        ff_pixel_format_set         (AVPixelFormat pf)              { ff_pixel_format_ = pf;        }
    AVPixelFormat               ff_pixel_format             () const                        { return ff_pixel_format_;      }

    const cpaf::video::surface_dimensions_t& render_dimensions           () const                        { return render_dimensions_;      }

    void                        platform_render_set         (const platform_render_t& platform_render)  { do_platform_render_set(platform_render); }
    void                        platform_surface_set        (const platform_surface_t& platform_surface){ do_platform_surface_set(platform_surface); }
    void                        platform_video_texture_set  (const platform_texture_t& platform_texture){ do_platform_video_texture_set(platform_texture); }
    bool                        render_video_frame          (const cpaf::video::av_frame& frame)        { return do_render_video_frame(frame);  }

    platform_render_t&          platform_render             () { return do_platform_render();   }
    platform_surface_t&         platform_surface            () { return do_platform_surface();  }
    platform_texture_t&         platform_video_texture      () { return do_platform_video_texture();  }

protected:
    cpaf::video::av_format_context&         format_context              () { return *format_context_ptr_; }
    const cpaf::video::av_format_context&   format_context              () const { return *format_context_ptr_; }
    cpaf::video::av_codec_context&          video_codec_ctx             () { return *video_codec_ctx_ptr_; }
    cpaf::video::av_frame&                  frame_display               () { return frame_display_; }
    cpaf::video::av_frame                   frame_display_;

private:
    cpaf::video::av_format_context*         format_context_ptr_     = nullptr;
    cpaf::video::av_codec_context*          video_codec_ctx_ptr_    = nullptr;
    cpaf::video::media_stream_time*         current_media_time_ptr_ = nullptr;
    cpaf::video::surface_dimensions_t       render_dimensions_;
    AVPixelFormat                           ff_pixel_format_        = AV_PIX_FMT_YUV420P;

    void                        create_frame_display            ();

    virtual void                do_platform_render_set          (const platform_render_t& platform_render) = 0;
    virtual void                do_platform_surface_set         (const platform_surface_t& platform_surface) = 0;
    virtual void                do_platform_video_texture_set   (const platform_texture_t& platform_texture) = 0;
    virtual platform_render_t&  do_platform_render              () = 0;
    virtual platform_surface_t& do_platform_surface             () = 0;
    virtual platform_texture_t& do_platform_video_texture       () = 0;
    virtual bool                do_render_video_frame           (const cpaf::video::av_frame& frame) = 0;

};

} //END namespace cpaf::gui::video
