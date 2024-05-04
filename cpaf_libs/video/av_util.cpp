
#include "av_util.h"


extern "C"
{
#include <libavutil/channel_layout.h>
#include <libavutil/dict.h>
}

#include <iostream>
#include <fmt/format.h>

#include <cpaf_libs/unicode/cpaf_u8string_utils.h>
#include <cpaf_libs/torrent/torrent_utils.h>

// FIND: ^(.+)\s*$
// REPLACE: case \1: return "\1"; break;


using namespace std;
namespace cu = cpaf::unicode;

namespace cpaf::video {

string to_string(media_type_t mt)
{
    switch (mt) {
    case media_type_t::unknown:       return "unknown"; break;
    case media_type_t::video:         return "video"; break;
    case media_type_t::audio:         return "audio"; break;
    case media_type_t::data:          return "data"; break;
    case media_type_t::subtitle:      return "subtitle"; break;
    case media_type_t::attachment:    return "attachment"; break;
    case media_type_t::nb:            return "nb"; break;
    case media_type_t::SIZE:          return "END"; break;
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

string stream_info_t::dbg_str() const
{
    string s;
    s += fmt::format("--- Media type: {}, index: {} ---\n", to_string(media_type), stream_index);
    s += fmt::format("Language code: {}\n", language_code);
    s += fmt::format("--- Meta data ---\n");
    for (const auto& [key, val]: meta_data) {
        s += fmt::format("{}: {}\n", key, val);

    }
    return s;
}

void stream_info_t::dbg_print() const
{
    std::cerr << dbg_str();
}

std::map<string, string> read_av_dictionary(AVDictionary* ff_dict)
{
    if (!ff_dict) {
        return std::map<string, string>();
    }
    std::map<string, string> dict;
    AVDictionaryEntry* entry = NULL;

    // Iterate over dictionary entries
    while ((entry = av_dict_get(ff_dict, "", entry, AV_DICT_IGNORE_SUFFIX))) {
        dict[entry->key] = entry->value;
    }
    return dict;
}


string to_string(seek_state_t ss)
{
    switch (ss) {
        case seek_state_t::unknown:             return "unknown"; break;
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
    case stream_state_t::opening :          return "opening stream"; break;
    case stream_state_t::open :             return "open"; break;
    case stream_state_t::start_playing :    return "start playing"; break;
    case stream_state_t::playing :          return "playing"; break;
    case stream_state_t::closing:           return "closing"; break;
    }
    return "";
}




} //END namespace cpaf::video
