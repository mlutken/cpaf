#pragma once

extern "C"
{
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>
}


#include <string>
#include <chrono>
#include <atomic>
#include <memory>
#include <functional>

#include <math/base/v2.hpp>
#include <video/audio_types_convert.h>

namespace cpaf::torrent {
    class torrents;
}



namespace cpaf::video {
using av_base_duration = std::chrono::duration<int64_t, std::ratio<1, AV_TIME_BASE>>;
using surface_dimensions_t = cpaf::math::v2<int32_t>;
using get_torrents_fn = std::function<std::shared_ptr<cpaf::torrent::torrents> ()>;

constexpr int32_t surface_dimension_auto = -1;
static constexpr size_t no_stream_index = std::numeric_limits<size_t>::max();

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

enum class seek_state_t : int8_t {
    ready,
    requested,
    flushing,
    flush_done
};

enum class stream_type_t : size_t {
    video = 0,
    audio,
    subtitle,
    keyframes,
    ads,
    SIZE = ads +1 			///< One past the last as we start from zero
};

enum class subtitle_source_t : int8_t {
    none,
    stream,
    text_file
};


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
inline constexpr size_t stream_type_index_size  () { return static_cast<size_t>(stream_type_t::SIZE);}

inline constexpr auto	to_int					(media_type mt) -> int					{ return static_cast<int>(mt);	}
//inline constexpr auto	to_int					(audio_sample_format_t format) -> int	{ return static_cast<int>(format);}


inline constexpr auto to_size_t(media_type mt) -> size_t
{
    return static_cast<size_t>(mt);
}

inline constexpr auto to_size_t(stream_type_t sti) -> size_t
{
    return static_cast<size_t>(sti);
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
std::string to_string						(seek_state_t ss);

surface_dimensions_t scale_surface_dimensions(const surface_dimensions_t& src_dimensions, const surface_dimensions_t& dst_dimensions);

std::string             protocol_from_uri   (const std::string& uri);

} //END namespace cpaf::video



