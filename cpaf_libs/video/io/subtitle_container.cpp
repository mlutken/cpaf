#include "subtitle_container.h"
#include <algorithm>
#include <ranges>
#include <fmt/format.h>

#include <cpaf_libs/unicode/cpaf_u8string_utils.h>

using namespace std;
using namespace cpaf::unicode;
using namespace std::chrono;


namespace cpaf::video {


/**

@see https://en.cppreference.com/w/cpp/ranges/split_view
@see https://stackoverflow.com/questions/68756784/cannot-use-stdchronoparse-even-with-std-c2a-and-g-11
@see https://github.com/HowardHinnant/date
*/
void subtitle_container::parse_srt_file_data(std::string_view data_string_view)
{
    using std::operator""sv;
    constexpr auto newline{"\n"sv};

//    auto line_beg = data_string_view.begin();
//    auto line_end = data_string_view.begin();
//    const auto end = data_string_view.end();

//    while (line_beg < end) {
//        line_end = std::find(line_beg, end, '\n');
//        string_view line(line_beg, line_end);
//    //string_view line(&(*line_beg), static_cast<size_t>(std::distance(line_beg, line_end)));
//        fmt::println("Line: {}", line);

//        line_beg = ++line_end;
//    }



    for (const auto word : std::views::split(data_string_view, newline)) {
        // with string_view's C++23 range constructor:
        string_view line(word);
        fmt::println("LINE {}", line);
        parse_line(line);
    }
    fmt::println("");

}

void subtitle_container::parse_line(std::string_view line)
{

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

bool subtitle_container::parse_presentation_times(std::string_view line)
{
//    using sys_seconds = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;
//    sys_seconds tp;
//    std::chrono::parse("%a, %d %b %Y %T %z", tp);

//    std::chrono::parse();
}

bool subtitle_container::parse_subtitle_line(std::string_view line)
{
    if (line.empty()) {
        parse_state_ = state_t::frame_done;
        return true;
    }
    cur_parse_frame_.lines.push_back(std::string(line));
    return true;
}

void subtitle_container::push_current_frame()
{
    subtitles_.push_back(std::move(cur_parse_frame_));
    cur_parse_frame_ = frame_t{};
}



} // namespace cpaf::video
