#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <memory>
#include <cstdint>


namespace cpaf::video {

struct subtitle_frame
{
    static constexpr size_t         max_lines               {3};

    std::vector<std::string>        lines                   {};
    std::chrono::microseconds       presentation_time       {0};
    std::chrono::microseconds       presentation_time_end   {0};
    uint32_t                        sequence_number         {0};


    subtitle_frame() = default;
    explicit subtitle_frame(std::string s0);
    subtitle_frame(std::string s0, std::string s1);
    subtitle_frame(std::string s0, std::string s1, std::string s2);

    size_t                          lines_count                 () const {return std::min(lines.size(), max_lines);}
    void                            set_presentaion_times       (std::chrono::microseconds start, std::chrono::microseconds end);
    void                            set_presentaion_times_ms    (uint32_t start_ms, uint32_t end_ms);
    std::chrono::milliseconds       presentation_time_ms        () const { return std::chrono::duration_cast<std::chrono::milliseconds>(presentation_time); }
    std::chrono::milliseconds       presentation_time_end_ms    () const { return std::chrono::duration_cast<std::chrono::milliseconds>(presentation_time_end); }
    bool                            should_show                 () const { return !lines.empty(); }
};

} //END namespace cpaf::video



