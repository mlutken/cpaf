
#include "av_util.h"

extern "C"
{
#include <libavutil/channel_layout.h>
}

// FIND: ^(.+)\s*$
// REPLACE: case \1: return "\1"; break;


using namespace std;
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

string to_string(pipeline_control_t pc)
{
    switch (pc) {
    case pipeline_control_t::none:              return "none"; break;
    case pipeline_control_t::normal_flow:       return "normal_flow"; break;
    case pipeline_control_t::first_after_flush: return "first_after_flush"; break;
    case pipeline_control_t::seek_pos:          return "seek_pos"; break;
    }
    return "";
}


string to_string(pipeline_state_t ps)
{
    switch (ps) {
    case pipeline_state_t::inactive:            return "inactive"; break;
    case pipeline_state_t::paused:              return "paused"; break;
    case pipeline_state_t::normal_flow:         return "normal_flow"; break;
////    case pipeline_state_t::flush_requested:     return "flush_requested"; break;
    case pipeline_state_t::flush_in_progress:   return "flush_in_progress"; break;
    case pipeline_state_t::flush_done:          return "flush_done"; break;
    case pipeline_state_t::seek_pos:            return "seek_pos"; break;
    }
    return "";
}



} //END namespace cpaf::video
