#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <memory>
#include <cstdint>
#include <concurrent/srsw_fifo.hpp>

struct AVSubtitle;

namespace cpaf::video {

class av_packet;

/**
typedef struct AVSubtitle {
https://github.com/libass/libass
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


    bool                            is_ff_subtitle_valid        () const { return ff_subtitle_ptr_.get(); }
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

    static constexpr size_t         max_lines                   {3};
private:
    std::unique_ptr<AVSubtitle>     ff_subtitle_ptr_            {nullptr};
    format_t                        format_                      = format_t::text;
};

using subtitles_queue = estl::srsw_fifo<subtitle_frame>;

} //END namespace cpaf::video



