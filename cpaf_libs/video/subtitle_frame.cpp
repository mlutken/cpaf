#include "subtitle_frame.h"



using namespace std;

namespace cpaf::video {

subtitle_frame::subtitle_frame(std::string s0)
    : lines{std::move(s0)}
{

}

subtitle_frame::subtitle_frame(std::string s0, std::string s1)
    : lines{std::move(s0), std::move(s1)}
{

}

subtitle_frame::subtitle_frame(std::string s0, std::string s1, std::string s2)
    : lines{std::move(s0), std::move(s1), std::move(s2)}
{

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



} //END namespace cpaf::video
