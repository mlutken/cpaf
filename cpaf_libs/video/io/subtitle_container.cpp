#include "subtitle_container.h"
#include <algorithm>
#include <ranges>
#include <sstream>
#include <fmt/format.h>

#include <KZip.hpp>
#include <cpaf_libs/time/cpaf_time.h>
#include <cpaf_libs/unicode/cpaf_u8string_utils.h>
#include <cpaf_libs/video/io/subtitle_text_file_data.h>

using namespace std;
using namespace cpaf::time;
using namespace cpaf::unicode;
using namespace std::chrono;


namespace cpaf::video {

string subtitle_container::frame_t::to_string() const
{
    string s = fmt::format("{}\n{} --> {}\n",
                       sequence_number,
                       format_h_m_s_ms(presentation_time),
                       format_h_m_s_ms(presentation_time_end)
                       );

    for (auto line: lines) {
        s.append(line);
        s.append("\n");
    }
    return s;
}

/**

@see https://en.cppreference.com/w/cpp/ranges/split_view
@see https://stackoverflow.com/questions/68756784/cannot-use-stdchronoparse-even-with-std-c2a-and-g-11
@see https://github.com/HowardHinnant/date
*/

std::unique_ptr<subtitle_container> subtitle_container::create_from_path(
    const std::string& resource_path,
    std::chrono::milliseconds timeout)
{
    auto container = std::make_unique<subtitle_container>();
    subtitle_text_file_data stfd;
    if (stfd.open(resource_path, timeout)) {
        container->parse_srt_file_data(stfd.srt_file_data());
    }

    return container;
}

/// @todo Implement subtitle_text_format_t detection!
subtitle_text_format_t subtitle_container::detect_format(std::string_view /*data_string_view*/)
{
    return subtitle_text_format_t::srt;
}

void subtitle_container::parse_file_data(std::string_view data_string_view)
{
    const auto format = subtitle_container::detect_format(data_string_view);

    if (format == subtitle_text_format_t::srt) {
        parse_srt_file_data(data_string_view);
    }
}

void subtitle_container::parse_srt_file_data(std::string_view data_string_view)
{
    using std::operator""sv;
    constexpr auto newline{"\n"sv};

    for (const auto word : std::views::split(data_string_view, newline)) {
        // with string_view's C++23 range constructor:
        string_view line(word);
//        fmt::println("LINE {}", line);
        parse_line(line);
    }
}

/// @todo Better implementation. Ie. detect more potential errors in parsing the srt file et.
bool subtitle_container::is_valid() const
{
    return !empty();
}

subtitle_container::const_iterator subtitle_container::find_last_before(std::chrono::microseconds ts) const
{
    auto iter = find_first_after(ts);
    if (iter > begin()) { --iter; }
///    if (iter > begin()) { --iter; }
///    if (iter > begin()) { --iter; }
    return iter;
}

subtitle_container::const_iterator subtitle_container::find_first_after(std::chrono::microseconds ts) const
{
    constexpr auto compare = [](const auto& frm, std::chrono::microseconds ts) -> bool {
        return frm.presentation_time < ts;
    };

    const auto it = std::lower_bound(subtitles_.begin(), subtitles_.end(), ts, compare);
    return it;
}

void subtitle_container::parse_line(std::string_view line)
{
    switch (parse_state_) {
    case state_t::new_frame:
    case state_t::seq_num:
    {
        if (line.empty()) {
            return;
        }
        else {
            parse_sequence_number(line);
            parse_state_ = state_t::ps_times;
            return;
        }
        break;
    }
    case state_t::ps_times:
    {
        parse_presentation_times(line);
        parse_state_ = state_t::text;
        return;
        break;
    }
    case state_t::text:
    {
        if (!parse_subtitle_line(line)) {
            parse_state_ = state_t::new_frame;
            push_current_frame();
        }
        return;
        break;
    }
    default:
        break;
    }
}

bool subtitle_container::parse_sequence_number(std::string_view line)
{
    long seq = -1;
    try {
        seq = std::stol(std::string(line));
        if (seq > -1) {
            cur_parse_frame_.sequence_number = seq;
            return true;
        }
    } catch (...) {
    }

    return false;
}

// 00:03:18,856 --> 00:03:20,856
bool subtitle_container::parse_presentation_times(std::string_view line)
{
    int32_t h1{}, h2{};
    int32_t m1{}, m2{};
    int32_t s1{}, s2{};
    int32_t ms1{}, ms2{};

    int ret = std::sscanf(line.data(), "%2d:%2d:%2d,%3d --> %2d:%2d:%2d,%3d",
                          &h1, &m1, &s1, &ms1, &h2, &m2, &s2, &ms2);

    cur_parse_frame_.presentation_time      = hours{h1} + minutes{m1} + seconds{s1} + milliseconds{ms1};
    cur_parse_frame_.presentation_time_end  = hours{h2} + minutes{m2} + seconds{s2} + milliseconds{ms2};

    return ret == 8;
}

/// @todo Enable html tags in subtitles again when subtitle render supports it!
bool subtitle_container::parse_subtitle_line(std::string_view line)
{
    string trimmed_line = clean_html_tags_copy(std::string(line), post_op::simplify_ws);
    if (trimmed_line.empty()) {
        return false;
    }
    cur_parse_frame_.lines.push_back(std::move(trimmed_line));
    return true;
}

void subtitle_container::push_current_frame()
{
    subtitles_.push_back(std::move(cur_parse_frame_));
    cur_parse_frame_ = frame_t{};
}




} // namespace cpaf::video
