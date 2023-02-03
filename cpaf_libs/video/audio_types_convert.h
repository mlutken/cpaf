#ifndef CPAF_VIDEO_AUDIO_TYPES_CONVERT_H
#define CPAF_VIDEO_AUDIO_TYPES_CONVERT_H

#include <cstdint>
#include <string>

extern "C"
{
#include <libavutil/samplefmt.h>
}


namespace cpaf::video {

int32_t         sample_format_byte_count        (AVSampleFormat format);
int32_t         ff_channel_layout_to_count      (const uint64_t channel_layout);
std::string     to_string						(AVSampleFormat format);
std::string     ff_channel_layout_to_string     (const uint64_t channel_layout);
std::string     ff_channel_layout_name          (const uint64_t channel_layout, int32_t nb_channels = -1);

} //END namespace cpaf::video


#endif //CPAF_VIDEO_AUDIO_TYPES_CONVERT_H


