
#include "media_stream_time.h"

#include <cpaf_libs/video/av_frame.h>

using namespace std;
namespace cpaf::video {

media_stream_time::media_stream_time() {
    reset_start_time();
}

media_stream_time::media_stream_time(const std::chrono::microseconds& reset_offset) {
    reset_start_time(reset_offset);
}

void media_stream_time::release_after_reset() {
    start_time_was_reset_ = false;
}

std::chrono::microseconds media_stream_time::time_to_next_frame(const av_frame& next_frame) const {
    return next_frame.presentation_time() - current_time_pos();
}

std::chrono::microseconds media_stream_time::time_to_next_frame(const std::chrono::microseconds& next_time_pos) const {
    return next_time_pos - current_time_pos();
}

std::chrono::microseconds media_stream_time::current_time_pos() const {
    if (start_time_was_reset_) {
        return reset_offset_;
    }
    if (time_is_paused_) {
        return paused_time_pos_;
    }
    const auto tp_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(tp_now - time_point_start_) + reset_offset_;
}

std::chrono::microseconds media_stream_time::video_time_pos() const
{
    return current_time_pos() + video_offset_;
}


void media_stream_time::pause_time()
{
    paused_time_pos_ = current_time_pos();
    time_is_paused_ = true;
//    time_point_paused_ = std::chrono::high_resolution_clock::now();
}

void media_stream_time::resume_time()
{
    if (time_is_paused_) {
        adjust_time(paused_time_pos_);
        time_is_paused_ = false;
    }
}


void media_stream_time::reset_start_time() {
    reset_offset_ = 0ms;
    time_point_start_ = std::chrono::high_resolution_clock::now();
    start_time_was_reset_ = true;
}

void media_stream_time::reset_start_time(const std::chrono::microseconds& reset_offset) {
    reset_offset_ = reset_offset;
    time_point_start_ = std::chrono::high_resolution_clock::now();
    start_time_was_reset_ = true;
}

void media_stream_time::adjust_time(const std::chrono::microseconds& reset_offset) {
    reset_offset_ = reset_offset;
    time_point_start_ = std::chrono::high_resolution_clock::now();
    start_time_was_reset_ = false;
}


void media_stream_time::dbg_print(const std::string& text) const
{
    std::cerr << "[" << text << "]"
              << ", Time: " << current_time_pos_ms().count() << " ms"
              << "\n";
}





} //END namespace cpaf::video
