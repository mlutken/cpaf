#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <memory>
#include <cstdint>
#include <concurrent/srsw_fifo.hpp>
#include <cpaf_libs/math/primitives2d/rectangle.hpp>
#include <cpaf_libs/video/av_util.h>

struct AVSubtitle;
struct AVSubtitleRect;

namespace cpaf::video {

class av_packet;

/**
typedef struct AVSubtitle {
https://github.com/libass/libass
https://stackoverflow.com/questions/54125207/dump-subtitle-from-avsubtitle-in-the-file
https://stackoverflow.com/questions/60551861/avsubtitlerect-dvbsub-format-explanation

https://superuser.com/questions/1688760/extracting-dvb-subtitles-with-ffmpeg-fails-with-empty-file-nothing-encoded

current_subtitle_frame_.ff_subtitle_ptr_->num_rects
current_subtitle_frame_.ff_subtitle_ptr_->rects[0].nb_colors
current_subtitle_frame_.ff_subtitle_ptr_->rects[0].x
current_subtitle_frame_.ff_subtitle_ptr_->rects[0].y
current_subtitle_frame_.ff_subtitle_ptr_->rects[0].w
current_subtitle_frame_.ff_subtitle_ptr_->rects[0].h
current_subtitle_frame_.ff_subtitle_ptr_->rects[0].type
current_subtitle_frame_.ff_subtitle_ptr_->rects[0].data[0]
current_subtitle_frame_.ff_subtitle_ptr_->rects[0].data[1]
current_subtitle_frame_.ff_subtitle_ptr_->rects[0].linesize
current_subtitle_frame_.ff_subtitle_ptr_->rects[0].text
current_subtitle_frame_.ff_subtitle_ptr_->rects[0].ass
current_subtitle_frame_.ff_subtitle_ptr_->rects[0].flags


*/
class subtitle_frame
{
public:
    enum class format_t {text, graphics};


    std::vector<std::string>        lines                   {};
    std::chrono::microseconds       presentation_time       {0};
    std::chrono::microseconds       presentation_time_end   {0};
    uint32_t                        sequence_number         {0};

    subtitle_frame();
    explicit subtitle_frame(std::unique_ptr<AVSubtitle> ff_subtitle_ptr);
    explicit subtitle_frame(std::string s0);
    subtitle_frame(std::string s0, std::string s1);
    subtitle_frame(std::string s0, std::string s1, std::string s2);
    subtitle_frame (const subtitle_frame&) = delete;
    subtitle_frame& operator=(const subtitle_frame&)  = delete;

    subtitle_frame (subtitle_frame&& moving) noexcept;
    subtitle_frame& operator=(subtitle_frame&& moving) noexcept;
    ~subtitle_frame();


    bool                            ff_subtitle_is_valid        () const { return ff_subtitle_ptr_.get(); }
    bool                            is_valid                    () const;
    format_t                        format                      () const { return format_; }
    size_t                          lines_count                 () const {return std::min(lines.size(), max_lines);}
    void                            set_presentaion_times       (std::chrono::microseconds start, std::chrono::microseconds end);
    void                            set_presentaion_times_ms    (uint32_t start_ms, uint32_t end_ms);
    std::chrono::milliseconds       presentation_time_ms        () const { return std::chrono::duration_cast<std::chrono::milliseconds>(presentation_time); }
    std::chrono::milliseconds       presentation_time_end_ms    () const { return std::chrono::duration_cast<std::chrono::milliseconds>(presentation_time_end); }
    bool                            should_show                 () const;
    void                            swap                        (subtitle_frame& src) noexcept;
    AVSubtitle&                     ff_subtitle                 ();

    std::string                     dbg_str                     () const;
    void                            format_set                  (format_t format) { format_ = format; }

    cpaf::math::rectf               ff_bitmap_rect              () const;
    uint32_t                        ff_bitmap_pixel_count       () const;
    uint32_t                        ff_num_rects                () const;
    const AVSubtitleRect&           ff_rect                     (uint32_t i) const;
    uint16_t                        ff_rect_x                   () const;
    uint16_t                        ff_rect_y                   () const;
    uint16_t                        ff_rect_w                   () const;
    uint16_t                        ff_rect_h                   () const;

    uint32_t                        ff_data_line_size           () const;
    const uint8_t*                  ff_pixel_data               () const ;
    const pixel_rgba_t*             ff_pixel_color_map          () const ;
    pixel_rgba_t                    ff_pixel_lookup             (uint8_t color_index) const;
    pixel_rgba_t                    ff_pixel                    (uint32_t x, uint32_t y) const;

    static constexpr size_t         max_lines                   {3};
private:
    std::unique_ptr<AVSubtitle>     ff_subtitle_ptr_            {nullptr};
    format_t                        format_                      = format_t::text;
};

using subtitles_queue = estl::srsw_fifo<subtitle_frame>;

} //END namespace cpaf::video



