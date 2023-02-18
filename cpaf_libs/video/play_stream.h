#ifndef CPAF_VIDEO_PLAY_STREAM_H
#define CPAF_VIDEO_PLAY_STREAM_H


extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <string>
#include <cpaf_libs/video/av_format_context.h>

namespace cpaf::video {

/**
 *
 *  */
class play_stream
{
public:
             play_stream() = default;
    explicit play_stream(const std::string& resource_path);

    bool                        open                    (const std::string& resource_path);

    // --- Info functions ---
    const std::string&          resource_path			() const { return format_context_.resource_path(); }
    size_t                      streams_count			() const { return format_context_.streams_count();}
    size_t                      video_streams_count		() const { return format_context_.video_streams_count(); }
    size_t                      audio_streams_count		() const { return format_context_.audio_streams_count(); }
    size_t                      subtitle_streams_count	() const { return format_context_.subtitle_streams_count(); }
    size_t                      first_stream_index		(media_type mt) const { return format_context_.first_stream_index(mt); }
    size_t                      first_video_index		() const { return format_context_.first_video_index(); }
    size_t                      first_audio_index		() const { return format_context_.first_audio_index(); }
    size_t                      first_subtitle_index	() const { return format_context_.first_subtitle_index(); }
    size_t                      primary_index           () const { return format_context_.primary_index(); }
    media_type                  primary_media_type      () const { return format_context_.primary_media_type(); }

    av_format_context&          format_context()        { return format_context_; }
    const av_format_context&	format_context() const  { return format_context_; }


    // --- XXX Functions ---
    av_codec_parameters         codec_parameters        (size_t stream_index) const { return format_context_.codec_parameters(stream_index);}
    av_codec_context            codec_context			(size_t stream_index) const { return format_context_.codec_context(stream_index); }

private:
    av_format_context		format_context_;
};


} //END namespace cpaf::video


#endif //CPAF_VIDEO_PLAY_STREAM_H


