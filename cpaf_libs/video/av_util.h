#pragma once

extern "C"
{
#include <libswresample/swresample.h>
struct AVDictionary;
}


#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <atomic>
#include <memory>
#include <functional>
#include <string>

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

enum class media_type_t : int8_t {
    unknown		= AVMEDIA_TYPE_UNKNOWN,			///< NB: Is -1!! Usually treated as AVMEDIA_TYPE_DATA
    video		= AVMEDIA_TYPE_VIDEO,
    audio		= AVMEDIA_TYPE_AUDIO,
    data		= AVMEDIA_TYPE_DATA,			///< Opaque data information usually continuous
    subtitle	= AVMEDIA_TYPE_SUBTITLE,
    attachment	= AVMEDIA_TYPE_ATTACHMENT,		///< Opaque data information usually sparse
    nb			= AVMEDIA_TYPE_NB,
    SIZE		= AVMEDIA_TYPE_NB +2 			///< One past last. The 'magic' +2 is because AVMEDIA_TYPE_UNKNOWN is -1 and not zero
};


inline media_type_t operator++(media_type_t& mt) {
    if (mt == media_type_t::SIZE) {
        return mt;
    }
    using int_type = typename std::underlying_type<media_type_t>::type;
    mt = static_cast<media_type_t>(static_cast<int_type>(mt) + 1);
    return mt;
}

enum class seek_dir : int8_t {
    forward,
    backward
};

enum class seek_state_t : int8_t {
    unknown,
    ready,
    requested,
    flushing,
    flush_done,
    waiting_for_sync
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

enum class subtitle_text_format_t : int8_t {
    unknown,
    srt
};

struct subtitle_source_entry_t
{
    std::string         path;
    std::string         language_code;
    std::string         language_name;
    subtitle_source_t   source;
};

using subtitle_source_entries_t = std::vector<subtitle_source_entry_t>;

enum class stream_state_t : int8_t {
    inactive,
    opening,
    open,
    playing,
    waiting_for_data,
    playing_local,
    finished
};

enum class subtitle_file_state_t : int8_t {
    inactive,
    error_downloading,
    downloading,
    playing
};


struct pixel_rgba_t {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
};


static_assert(sizeof(uint32_t) == sizeof(pixel_rgba_t), "ERROR pixel_rgba_t is NOT 4 bytes!" );


inline bool media_type_valid (AVMediaType ff_mt) {
    return AVMEDIA_TYPE_UNKNOWN < ff_mt && ff_mt <= AVMEDIA_TYPE_NB;
}

inline bool media_type_valid (media_type_t mt) {
    return mt != media_type_t::unknown && mt != media_type_t::SIZE;
}

inline constexpr int    media_type_first_int    () { return static_cast<int>(media_type_t::unknown); }
inline constexpr size_t media_type_first_size_t () { return static_cast<size_t>(media_type_t::unknown);}

inline constexpr int    media_type_size_int     () { return static_cast<int>(media_type_t::SIZE); }
inline constexpr size_t media_type_size         () { return static_cast<size_t>(media_type_t::SIZE);}
inline constexpr size_t stream_type_index_size  () { return static_cast<size_t>(stream_type_t::SIZE);}

inline constexpr auto	to_int					(media_type_t mt) -> int				{ return static_cast<int>(mt);	}
inline constexpr auto	to_int					(stream_state_t ss) -> int          { return static_cast<int>(ss);  }


inline constexpr auto to_size_t(media_type_t mt) -> size_t
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

std::map<std::string, std::string>
            read_av_dictionary              (AVDictionary* ff_dict);
std::string to_string						(media_type_t mt);
std::string to_string						(seek_state_t ss);
std::string to_string						(stream_state_t ss);

surface_dimensions_t scale_surface_dimensions(const surface_dimensions_t& src_dimensions, const surface_dimensions_t& dst_dimensions);

std::string             protocol_from_uri   (const std::string& uri);

} //END namespace cpaf::video



