#ifndef CPAF_VIDEO_AV_CODEC_PARAMETERS_H
#define CPAF_VIDEO_AV_CODEC_PARAMETERS_H


extern "C"
{
#include <libavcodec/avcodec.h>
}

#include <string>
#include <cpaf_libs/video/av_codec.h>

namespace cpaf::video {

/**
 *
 *  */
class av_codec_parameters
{
public:
	// --- Constructors etc. ---
	av_codec_parameters() = default;
	explicit av_codec_parameters(AVCodecParameters* ff_codec_parameters);

    bool        is_valid() const { return ff_codec_parameters_ != nullptr; }

	~av_codec_parameters();

	// --- Info functions ---
//	const std::string& resource_path	() const { return resource_path_; }

	// --- Debug functions ---
	void				dump () const;

private:
	AVCodecParameters*	ff_codec_parameters_ = nullptr;
};


} //END namespace cpaf::video


#endif //CPAF_VIDEO_AV_CODEC_PARAMETERS_H


