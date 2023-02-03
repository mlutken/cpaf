#ifndef CPAF_VIDEO_AV_CODEC_H
#define CPAF_VIDEO_AV_CODEC_H


extern "C"
{
#include <libavcodec/avcodec.h>
}

#include <string>

namespace cpaf::video {

/**
 *
 *  */
class av_codec
{
public:
	// --- Constructors etc. ---
    av_codec() = default;
    explicit av_codec(AVCodec* ff_codec);

    ~av_codec();

	// --- Info functions ---
    bool        is_valid() const { return ff_codec_ != nullptr; }
//	const std::string& resource_path	() const { return resource_path_; }

	// --- Debug functions ---
	void				dump () const;

private:
    AVCodec*	ff_codec_ = nullptr;
};


} //END namespace cpaf::video


#endif //CPAF_VIDEO_AV_CODEC_H


