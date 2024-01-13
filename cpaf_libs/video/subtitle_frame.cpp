#include "subtitle_frame.h"
#include <iostream>

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

namespace cpaf::video {

subtitle_frame::subtitle_frame()
{
}

subtitle_frame::subtitle_frame(std::string s0)
    : lines{std::move(s0)}
{

}

subtitle_frame::subtitle_frame(std::unique_ptr<AVSubtitle> ff_subtitle_ptr)
    : ff_subtitle_ptr_(std::move(ff_subtitle_ptr))
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
        std::cerr << "FIXMENM avsubtitle_free()\n";
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
}

//AVSubtitle& subtitle_frame::ff_subtitle()
//{
//    if (!ff_subtitle_ptr_) {
//        std::make_unique<AVSubtitle>();
//    }
//    return *ff_subtitle_ptr_;
//}


} //END namespace cpaf::video
