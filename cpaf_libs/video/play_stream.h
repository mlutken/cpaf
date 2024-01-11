#pragma once

#include <string>
#include <cpaf_libs/video/av_format_context.h>
#include <cpaf_libs/video/av_samples_queue.h>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/gui/video/pipeline_threads/pipeline_threads.h>

namespace cpaf::video {

/**
 *
 *  */
class play_stream
{
public:
    // -------------------------
    // --- Constructors etc. ---
    // -------------------------
             explicit play_stream(get_torrents_fn get_torrents_function);
             ~play_stream();
    bool                        open                    (const std::string& resource_path);

     void                       video_index_set             (size_t stream_index) { format_context_.video_index_set(stream_index);}
     void                       audio_index_set             (size_t stream_index) { format_context_.audio_index_set(stream_index);}
     void                       subtitle_index_set          (size_t stream_index) { format_context_.subtitle_index_set(stream_index);}

    // ----------------------
    // --- Info functions ---
    // ----------------------
    const std::string&          resource_path			() const { return format_context_.resource_path(); }
    size_t                      streams_count			() const { return format_context_.streams_count();}
    size_t                      streams_count			(media_type mt) const   { return format_context_.streams_count(mt);}
    bool                        has_media_type			(media_type mt) const   { return streams_count(mt) > 0;}
    size_t                      video_streams_count		() const { return format_context_.video_streams_count(); }
    size_t                      audio_streams_count		() const { return format_context_.audio_streams_count(); }
    size_t                      subtitle_streams_count	() const { return format_context_.subtitle_streams_count(); }
    size_t                      first_stream_index		(media_type mt) const { return format_context_.first_stream_index(mt); }
    size_t                      first_video_index		() const { return format_context_.first_video_index(); }
    size_t                      first_audio_index		() const { return format_context_.first_audio_index(); }
    size_t                      first_subtitle_index	() const { return format_context_.first_subtitle_index(); }
    size_t                      primary_index           () const { return format_context_.primary_index(); }
    media_type                  primary_media_type      () const { return format_context_.primary_media_type(); }
    const surface_dimensions_t& render_dimensions       () const { return render_dimensions_; }
    std::chrono::microseconds   total_time              () const { return format_context_.total_time(); }


    // ---------------------
    // --- Context/codec ---
    // ---------------------
    av_format_context&          format_context          ()                          { return format_context_; }
    const av_format_context&	format_context          () const                    { return format_context_; }
    av_codec_parameters         codec_parameters        (size_t stream_index) const { return format_context_.codec_parameters(stream_index);}
    av_codec_context            codec_context			(size_t stream_index) const { return format_context_.codec_context(stream_index); }
    av_codec_context            codec_context           (media_type selected_media) const { return format_context_.codec_context(selected_media); }
    av_codec_context            codec_context_video     () const    { return format_context_.codec_context_video(); }
    av_codec_context            codec_context_audio     () const    { return format_context_.codec_context_audio(); }
    av_codec_context            codec_context_subtitle  () const    { return format_context_.codec_context_subtitle(); }

    // -----------------------
    // --- Video functions ---
    // -----------------------
    void                        render_dimensions_set   (const surface_dimensions_t& dim);
    void                        render_width_set        (int32_t render_width);
    void                        render_height_set       (int32_t render_height);

private:
    av_format_context           format_context_;
    surface_dimensions_t        render_dimensions_;
};


} //END namespace cpaf::video

