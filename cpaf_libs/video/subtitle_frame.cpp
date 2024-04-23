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

bool subtitle_frame::subtitle_within_display_time(std::chrono::microseconds current_time) const
{
    if (!is_valid()) {
        return false;
    }
    const auto should_display = presentation_time <= current_time && current_time <= presentation_time_end;
    return should_display;
}

bool subtitle_frame::subtitle_too_old(std::chrono::microseconds current_time) const
{
    if (!is_valid()) {
        return true;
    }
    return presentation_time_end < current_time;
}



bool subtitle_frame::ff_subtitle_is_valid() const {
    if (!ff_subtitle_ptr_) { return false; }
    if ((ff_subtitle_ptr_->num_rects == 0) ||
        (ff_subtitle_ptr_->rects == nullptr) ||
        (ff_subtitle_ptr_->rects[0] == nullptr)
        ) {
        return false;
    }
    AVSubtitleRect& r0 = *(ff_subtitle_ptr_->rects[0]);
    if (r0.type == SUBTITLE_NONE) { return false; }
    if (r0.type == SUBTITLE_BITMAP) {
        const bool valid_size = (r0.nb_colors > 0) && (r0.w > 0) && (r0.h > 0);
        const bool valid_data = (r0.data[0] != nullptr) && (r0.data[1] != nullptr) && (r0.linesize[0] > 0);
        return valid_size && valid_data;
    }
    else if (r0.type == SUBTITLE_TEXT) {
        return r0.text != nullptr;
    }
    else if (r0.type == SUBTITLE_ASS) {
        return r0.ass != nullptr;
    }

    return false;
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
    return !lines.empty() || ff_subtitle_is_valid();
}


void subtitle_frame::set_presentation_times(std::chrono::microseconds start, std::chrono::microseconds end)
{
    presentation_time = start;
    presentation_time_end = end;
}

void subtitle_frame::set_presentation_times_ms(uint32_t start_ms, uint32_t end_ms)
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

math::rectf subtitle_frame::ff_bitmap_rect() const
{
    return cpaf::math::rectf(ff_rect_x(), ff_rect_y(), ff_rect_w(), ff_rect_h());
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

uint16_t subtitle_frame::ff_rect_x() const
{
    return ff_rect(0).x;
}

uint16_t subtitle_frame::ff_rect_y() const
{
    return ff_rect(0).y;
}

uint16_t subtitle_frame::ff_rect_w() const
{
    return ff_rect(0).w;
}

uint16_t subtitle_frame::ff_rect_h() const
{
    return ff_rect(0).h;
}

uint32_t subtitle_frame::ff_data_line_size() const
{
    return static_cast<uint32_t>(ff_rect(0).linesize[0]);
}

const uint8_t* subtitle_frame::ff_pixel_data() const
{
    return ff_subtitle_ptr_->rects[0]->data[0];
}

const pixel_rgba_t* subtitle_frame::ff_pixel_color_map() const
{
    return reinterpret_cast<pixel_rgba_t*>(ff_subtitle_ptr_->rects[0]->data[1]);
}

pixel_rgba_t subtitle_frame::ff_pixel_lookup(uint8_t color_index) const
{
    return (ff_pixel_color_map())[color_index];
}

pixel_rgba_t subtitle_frame::ff_pixel(uint32_t x, uint32_t y) const
{
    const uint32_t index = x + y*ff_data_line_size();
    if (index >= ff_bitmap_pixel_count()) {
        return pixel_rgba_t();
    }
    const uint8_t* pixel_ptr = ff_pixel_data();
    const uint8_t color_index = pixel_ptr[index];
    return ff_pixel_lookup(color_index);
}

//AVSubtitle& subtitle_frame::ff_subtitle()
//{
//    if (!ff_subtitle_ptr_) {
//        std::make_unique<AVSubtitle>();
//    }
//    return *ff_subtitle_ptr_;
//}


} //END namespace cpaf::video
