
#include "av_util.h"

extern "C"
{
#include <libavutil/channel_layout.h>
}

#include <cpaf_libs/unicode/cpaf_u8string_utils.h>
#include <cpaf_libs/torrent/torrent_utils.h>

// FIND: ^(.+)\s*$
// REPLACE: case \1: return "\1"; break;


using namespace std;
namespace cu = cpaf::unicode;

namespace cpaf::video {

string to_string(media_type mt)
{
    switch (mt) {
    case media_type::unknown:       return "unknown"; break;
    case media_type::video:         return "video"; break;
    case media_type::audio:         return "audio"; break;
    case media_type::data:          return "data"; break;
    case media_type::subtitle:      return "subtitle"; break;
    case media_type::attachment:    return "attachment"; break;
    case media_type::nb:            return "nb"; break;
    case media_type::SIZE:          return "END"; break;
    }
    return "";
}

surface_dimensions_t scale_surface_dimensions(const surface_dimensions_t& src_dimensions, const surface_dimensions_t& dst_dimensions)
{
    if (dst_dimensions.x() <= 0 && dst_dimensions.y() <= 0) {
        return src_dimensions;
    }

    surface_dimensions_t dimensions;
    if (dst_dimensions.y() <= 0) {
        dimensions = src_dimensions.uniform_scale_x(dst_dimensions.x());
    }
    else if (dst_dimensions.x() <= 0) {
        dimensions = src_dimensions.uniform_scale_y(dst_dimensions.y());
    }
    else {
        dimensions = dst_dimensions;
    }
    return dimensions;
}

string protocol_from_uri(const std::string& uri)
{
    if (torrent::is_magnet_uri(uri)) {
        return "magnet";
    }
    return cu::substring_between(uri, "", ":");
}

string to_string(seek_state_t ss)
{
    switch (ss) {
        case seek_state_t::ready:               return "ready"; break;
        case seek_state_t::requested:           return "requested"; break;
        case seek_state_t::flushing:            return "flushing"; break;
        case seek_state_t::flush_done:          return "flush_done"; break;
        case seek_state_t::waiting_for_sync:    return "waiting_for_sync"; break;
    }
    return "unknown";
}

string to_string(stream_state_t ss)
{
    switch (ss) {
    case stream_state_t::inactive :         return "inactive"; break;
    case stream_state_t::opening :          return "opening stream ..."; break;
    case stream_state_t::open :             return "open"; break;
    case stream_state_t::playing :          return "playing fixmenm yes ..."; break;
    case stream_state_t::waiting_for_data : return "waiting for data ..."; break;
    case stream_state_t::playing_local :    return "playing_local"; break;
    case stream_state_t::finished :         return "finished"; break;
    }
    return "";
}



} //END namespace cpaf::video
