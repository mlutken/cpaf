#include "subtitle_frame.h"
#include <iostream>
#include <exception>
#include <fmt/format.h>
#include <cpaf_libs/time/cpaf_time.h>

extern "C"
{
#include <libavcodec/avcodec.h>
//#include <libavutil/imgutils.h>
//#include <libavutil/avstring.h>
//#include <libavutil/time.h>
//#include <libavutil/opt.h>
//#include <libavformat/avformat.h>
//#include <libswscale/swscale.h>
//#include <libswresample/swresample.h>
}


using namespace std;
using namespace cpaf;
using namespace cpaf::time;

namespace cpaf::video {

subtitle_frame::subtitle_frame()
{
}

subtitle_frame::subtitle_frame(std::unique_ptr<AVSubtitle> ff_subtitle_ptr)
    : ff_subtitle_ptr_(std::move(ff_subtitle_ptr))
{
    if (!ff_subtitle_ptr_) { return; }
    AVSubtitle& sub = *ff_subtitle_ptr_;

    if (sub.format == 0) {
        format_ = format_t::graphics;
        return;
    }
}

subtitle_frame::subtitle_frame(std::string s0)
    : lines{std::move(s0)}
{

}

subtitle_frame& subtitle_frame::operator=(subtitle_frame&& moving) noexcept
{
    moving.swap(*this);
    return *this;
}

subtitle_frame::subtitle_frame(subtitle_frame&& moving) noexcept
{
    moving.swap(*this);
}

subtitle_frame::~subtitle_frame()
{
    if (ff_subtitle_ptr_) {
        avsubtitle_free(ff_subtitle_ptr_.get());
//        std::cerr << "FIXMENM avsubtitle_free()\n";
    }
}

subtitle_frame::subtitle_frame(std::string s0, std::string s1)
    : lines{std::move(s0), std::move(s1)}
{

}

subtitle_frame::subtitle_frame(std::string s0, std::string s1, std::string s2)
    : lines{std::move(s0), std::move(s1), std::move(s2)}
{

}

bool subtitle_frame::is_valid() const {
    return !lines.empty() || ff_subtitle_ptr_;
}


void subtitle_frame::set_presentaion_times(std::chrono::microseconds start, std::chrono::microseconds end)
{
    presentation_time = start;
    presentation_time_end = end;
}

void subtitle_frame::set_presentaion_times_ms(uint32_t start_ms, uint32_t end_ms)
{
    presentation_time = std::chrono::microseconds(start_ms);
    presentation_time_end = std::chrono::microseconds(end_ms);
}

bool subtitle_frame::should_show() const {
    return !lines.empty() || ff_subtitle_ptr_;
}

void subtitle_frame::swap(subtitle_frame& src) noexcept
{
    std::swap(lines, src.lines);
    std::swap(presentation_time, src.presentation_time);
    std::swap(presentation_time_end, src.presentation_time_end);
    std::swap(sequence_number, src.sequence_number);
    std::swap(ff_subtitle_ptr_, src.ff_subtitle_ptr_);
    std::swap(format_, src.format_);
}

AVSubtitle& subtitle_frame::ff_subtitle()
{
    if (!ff_subtitle_ptr_) { throw std::bad_exception(); }
    return *ff_subtitle_ptr_;
}

string subtitle_frame::dbg_str() const
{
    auto str = fmt::format("Subtitle time: {} -> {}", format_h_m_s(presentation_time), format_h_m_s(presentation_time_end));
    if (!lines.empty()) {
        str += "\n";
    }

    if (format() == format_t::text){
        for (const auto& line: lines) {
            str += "    " + line + "\n";
        }
    }
    return str;
}

uint32_t subtitle_frame::ff_bitmap_pixel_count() const
{
    AVSubtitle& sub = *ff_subtitle_ptr_;
    int32_t pixel_count = 0;
    for (unsigned int i = 0; i < sub.num_rects; ++ i) {
        AVSubtitleRect* rect = sub.rects[i];
        pixel_count += rect->h * rect->w;
    }
    return static_cast<uint32_t>(pixel_count);
}

uint32_t subtitle_frame::ff_num_rects() const {
    return ff_subtitle_ptr_->num_rects;
}

const AVSubtitleRect& subtitle_frame::ff_rect(uint32_t i) const
{
//    DVDSubContext
    return *(ff_subtitle_ptr_->rects[i]);
}

int32_t subtitle_frame::ff_rect_x() const
{
    return ff_rect(0).x;
}

int32_t subtitle_frame::ff_rect_y() const
{
    return ff_rect(0).y;
}

int32_t subtitle_frame::ff_rect_w() const
{
    return ff_rect(0).w;
}

int32_t subtitle_frame::ff_rect_h() const
{
    return ff_rect(0).h;
}

//AVSubtitle& subtitle_frame::ff_subtitle()
//{
//    if (!ff_subtitle_ptr_) {
//        std::make_unique<AVSubtitle>();
//    }
//    return *ff_subtitle_ptr_;
//}


} //END namespace cpaf::video
