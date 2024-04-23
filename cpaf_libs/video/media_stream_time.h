#pragma once


extern "C"
{
}

#include <string>
#include <chrono>
#include <atomic>

//#include <cpaf_libs/video/audio_types_convert.h>

namespace cpaf::video {
class av_frame;

class media_stream_time
{
public:
    using time_point = std::chrono::high_resolution_clock::time_point;

    media_stream_time();

    explicit media_stream_time(const std::chrono::microseconds& reset_offset);

    const time_point& time_point_start () const { return time_point_start_; }

    void release_after_reset ();

    std::chrono::microseconds   time_to_next_frame          (const av_frame& next_frame) const;
    std::chrono::microseconds   time_to_next_frame          (const std::chrono::microseconds& next_time_pos) const;
    std::chrono::microseconds   current_time_pos            () const;
    std::chrono::microseconds   video_time_pos              () const;
    std::chrono::microseconds   subtitles_time_pos          () const;
    std::chrono::microseconds   subtitles_adjust_offset     () const { return subtitles_adjust_offset_; }
    std::chrono::milliseconds   current_time_pos_ms         () const { return std::chrono::duration_cast<std::chrono::milliseconds>(current_time_pos()); }
    std::chrono::milliseconds   video_time_pos_ms           () const { return std::chrono::duration_cast<std::chrono::milliseconds>(video_time_pos()); }
    const std::atomic_bool&     time_is_paused              () const { return time_is_paused_; }

    void                        pause_time                  ();
    void                        resume_time                 ();
    void                        reset_start_time            ();
    void                        reset_start_time            (const std::chrono::microseconds& reset_offset);
    void                        adjust_time                 (const std::chrono::microseconds& reset_offset);
    void                        video_offset_set            (const std::chrono::microseconds& video_offset)     { video_offset_ = video_offset;   }
    void                        subtitles_offset_set        (const std::chrono::microseconds& subtitles_offset) { subtitles_adjust_offset_ = subtitles_offset;   }

    void                        dbg_print                   (const std::string& text = "Media time") const;


private:
    time_point                  time_point_start_;
    time_point                  time_point_paused_;
    time_point                  test_time_point_start_;
    std::chrono::microseconds   reset_offset_               {0};
    std::chrono::microseconds   paused_time_pos_            {0};
    std::chrono::microseconds   video_offset_               {0};
    std::chrono::microseconds   subtitles_adjust_offset_    {0};
    bool                        start_time_was_reset_       {true};
    std::atomic_bool            time_is_paused_             {false};
};

} //END namespace cpaf::video




