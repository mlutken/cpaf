#pragma once

#include <array>
#include <memory>
#include <cpaf_libs/audio/cpaf_audio_device_base.h>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/play_stream.h>
#include <cpaf_libs/video/media_stream_time.h>

namespace cpaf::video {

class player
{
public:
    using audio_play_callback_t  = cpaf::audio::device_base::play_callback_t;

    // START: TEMPORARY REFACTOR functions ONLY!

    pipeline_threads&           pipeline_threads_temp_only  () { return primary_source_stream_->pipeline_threads_temp_only(); }

    // END  : TEMPORARY REFACTOR functions ONLY!


    player();
    bool                        open                    (const std::string& resource_path);
    bool                        open                    (const std::string& resource_path, stream_type_t sti);

    bool                        has_source_stream       (stream_type_t sti) const;
    const play_stream*          source_stream           (stream_type_t sti) const;
    play_stream*                source_stream           (stream_type_t sti);

    play_stream&                primary_stream          () { return *primary_source_stream_; }
    const std::string&          primary_resource_path	() const { return primary_resource_path_; }

    media_stream_time&          cur_media_time          ()          { return cur_media_time_; }
    const media_stream_time&    cur_media_time          () const    { return cur_media_time_; }

    // ---------------------------------------------
    // --- Interfacing to surrounding app/system ---
    // ---------------------------------------------
    audio_play_callback_t       audio_callback_get      ();
    bool                        video_frame_update      (av_frame& current_frame, render& video_render);


    // --------------------
    // --- Play control ---
    // --------------------
    void                        seek_position           (const std::chrono::microseconds& stream_pos, seek_dir dir);
    void                        seek_position           (const std::chrono::microseconds& stream_pos);
    void                        seek_relative           (const std::chrono::microseconds& delta_time);
    void                        pause_playback          ();
    void                        resume_playback         ();


private:
    // ---------------------------------
    // --- PRIVATE: Helper functions ---
    // ---------------------------------
    bool                        open_stream             (const std::string& resource_path, stream_type_t sti);
    bool                        open_primary_stream     (const std::string& resource_path);


    // ----------------------------
    // --- PRIVATE: Member vars ---
    // ----------------------------
//    std::array<play_stream*, stream_type_index_size()>                  source_stream_ptrs_ = {nullptr, nullptr, nullptr, nullptr, nullptr};
    std::array<std::unique_ptr<play_stream>, stream_type_index_size()>  source_streams_ = {nullptr, nullptr, nullptr, nullptr, nullptr};
    std::unique_ptr<play_stream>                                        primary_source_stream_;
    media_stream_time                                                   cur_media_time_;
    std::string                                                         primary_resource_path_;

};

} //END namespace cpaf::video

