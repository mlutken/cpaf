#ifndef CPAF_VIDEO_AV_UTIL_H
#define CPAF_VIDEO_AV_UTIL_H


extern "C"
{
#include <libavutil/samplefmt.h>
}

#include <string>
#include <chrono>
#include <atomic>

#include <math/base/v2.hpp>
#include <video/audio_types_convert.h>

namespace cpaf::video {
using pipeline_index_t = std::uint64_t;
using atomic_pipeline_index_t = std::atomic<pipeline_index_t>;
using av_base_duration = std::chrono::duration<int64_t, std::ratio<1, AV_TIME_BASE>>;
using surface_dimensions_t = cpaf::math::v2<int32_t>;

enum class media_type : int8_t {
    unknown		= AVMEDIA_TYPE_UNKNOWN,			///< NB: Is -1!! Usually treated as AVMEDIA_TYPE_DATA
    video		= AVMEDIA_TYPE_VIDEO,
    audio		= AVMEDIA_TYPE_AUDIO,
    data		= AVMEDIA_TYPE_DATA,			///< Opaque data information usually continuous
    subtitle	= AVMEDIA_TYPE_SUBTITLE,
    attachment	= AVMEDIA_TYPE_ATTACHMENT,		///< Opaque data information usually sparse
    nb			= AVMEDIA_TYPE_NB,
    SIZE		= AVMEDIA_TYPE_NB +2 			///< One past last. The 'magic' +2 is because AVMEDIA_TYPE_UNKNOWN is -1 and not zero
};


inline media_type operator++(media_type& mt) {
    if (mt == media_type::SIZE) {
        return mt;
    }
    using int_type = typename std::underlying_type<media_type>::type;
    mt = static_cast<media_type>(static_cast<int_type>(mt) + 1);
    return mt;
}

enum class seek_dir : int8_t {
    forward,
    backward
};

enum class pipeline_control_t : int8_t {
    none,
    normal_flow,
    first_after_flush,
    seek_pos
};

enum class pipeline_state_t : int8_t {
    inactive,
    paused,
    normal_flow,
    flush_in_progress,
    flush_done,
    seek_pos
};

//inline bool operator<(const pipeline_control_t& pc_left, const pipeline_control_t& pc_right) {
//    using int_type = typename std::underlying_type<pipeline_control_t>::type;
//    const int_type left_int = static_cast<int_type>(pc_left);
//    const int_type right_int = static_cast<int_type>(pc_right);
//    return left_int < right_int;
//}


inline bool media_type_valid (AVMediaType ff_mt) {
    return AVMEDIA_TYPE_UNKNOWN < ff_mt && ff_mt <= AVMEDIA_TYPE_NB;
}

inline bool media_type_valid (media_type mt) {
    return mt != media_type::unknown && mt != media_type::SIZE;
}

inline constexpr int    media_type_first_int    () { return static_cast<int>(media_type::unknown); }
inline constexpr size_t media_type_first_size_t () { return static_cast<size_t>(media_type::unknown);}

inline constexpr int    media_type_size_int     () { return static_cast<int>(media_type::SIZE); }
inline constexpr size_t media_type_size         () { return static_cast<size_t>(media_type::SIZE);}

inline constexpr auto	to_int					(media_type mt) -> int					{ return static_cast<int>(mt);	}
//inline constexpr auto	to_int					(audio_sample_format_t format) -> int	{ return static_cast<int>(format);}


inline constexpr auto to_size_t(media_type mt) -> size_t
{
    return static_cast<size_t>(mt);
}

inline constexpr auto illegal_stream_index() -> size_t
{
    return std::numeric_limits<size_t>::max();
}

inline constexpr std::chrono::microseconds illegal_timestamp()
{
    return std::chrono::microseconds::max();
}

std::string to_string						(media_type mt);
std::string to_string						(pipeline_control_t pc);
std::string to_string						(pipeline_state_t ps);

} //END namespace cpaf::video


#endif //CPAF_VIDEO_AV_UTIL_H


