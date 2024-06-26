#pragma once

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <string>
#include <vector>
#include <map>
#include <set>
#include <array>
#include <queue>
#include <unordered_map>
#include <chrono>
#include <mutex>
#include <memory>
#include <thread>

#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/av_codec_context.h>
#include <cpaf_libs/video/av_packet.h>


namespace cpaf::video {

class custom_io_base;

/**
 *
 *  @see http://dranger.com/ffmpeg/tutorial01.html,
 *  */
class av_format_context
{
public:
    using get_packet_fun = std::function<av_packet()>;

     // --- Constructors etc. ---
    explicit av_format_context(get_torrents_fn get_torrents_function,
                               std::atomic<stream_state_t>* stream_state_ptr);

    ~av_format_context();
    bool                    open                        (const std::string& resource_path);
    void                    close                       ();
    void                    cancel_current_io           ();
    void                    selected_media_index_set    (media_type_t mt, size_t stream_index);
    size_t                  media_type_to_index         (media_type_t mt) const { return selected_stream_per_media_type_[to_size_t(mt)]; }
    size_t                  primary_index               () const { return primary_stream_index_; }
    void                    primary_index_set           (size_t stream_index);
    media_type_t            primary_media_type          () const { return stream_media_type(primary_index()); }

    size_t                  video_index                 () const { return media_type_to_index(media_type_t::video);}
    size_t                  audio_index                 () const { return media_type_to_index(media_type_t::audio);}
    size_t                  subtitle_index              () const { return media_type_to_index(media_type_t::subtitle);}
    void                    video_index_set             (size_t stream_index) { selected_media_index_set(media_type_t::video, stream_index);}
    void                    audio_index_set             (size_t stream_index) { selected_media_index_set(media_type_t::audio, stream_index);}
    void                    subtitle_index_set          (size_t stream_index) { selected_media_index_set(media_type_t::subtitle, stream_index);}
    void                    set_default_selected_streams();
    void                    set_default_primary_stream  ();

    // --- Info functions ---
    const std::string&      resource_path               () const { return resource_path_; }
    size_t                  streams_count               () const;
    size_t                  streams_count               (media_type_t mt) const;
    size_t                  video_streams_count         () const { return stream_indices_per_media_type_[to_size_t(media_type_t::video)].size(); }
    size_t                  audio_streams_count         () const { return stream_indices_per_media_type_[to_size_t(media_type_t::audio)].size(); }
    size_t                  subtitle_streams_count      () const { return stream_indices_per_media_type_[to_size_t(media_type_t::subtitle)].size(); }
    size_t                  first_stream_index          (media_type_t mt) const;
    size_t                  first_video_index           () const { return first_stream_index(media_type_t::video); }
    size_t                  first_audio_index           () const { return first_stream_index(media_type_t::audio); }
    size_t                  first_subtitle_index        () const { return first_stream_index(media_type_t::subtitle); }
    AVCodecID				codec_id                    (size_t stream_index) const;
    media_type_t            stream_media_type           (size_t stream_index) const;
    std::set<media_type_t>  set_of_each_media_type      (const std::set<media_type_t>& types_to_skip = {media_type_t::subtitle}) const;
    bool                    is_torrent                  () const;
    const stream_info_vec&  streams_info                () const { return streams_info_; }

    std::atomic<stream_state_t>& stream_state           ()          { return global_stream_state_ptr_ ? *global_stream_state_ptr_ : local_stream_state_; }
    const std::atomic<stream_state_t>&  stream_state    () const    { return global_stream_state_ptr_ ? *global_stream_state_ptr_ : local_stream_state_; }

    // --- Seek Functions ---
    bool                    seek_time_pos               (std::chrono::microseconds stream_pos);
    bool                    seek_time_pos               (std::chrono::microseconds stream_pos, seek_dir dir);
    bool                    seek_time_pos               (size_t stream_index, std::chrono::microseconds stream_pos, seek_dir dir);
    bool                    seek_time_pos               (media_type_t mt, std::chrono::microseconds stream_pos, seek_dir dir);

    // --- Codec Functions ---
    av_codec_context		codec_context               (size_t stream_index) const;
    av_codec_context		codec_context               (media_type_t selected_media) const;
    av_codec_context		codec_context_video         () const    { return codec_context(media_type_t::video); }
    av_codec_context		codec_context_audio         () const    { return codec_context(media_type_t::audio); }
    av_codec_context		codec_context_subtitle      () const    { return codec_context(media_type_t::subtitle); }

//    av_codec                codec                   (size_t stream_index) const;
    // --- Packet Functions ---
    av_packet                   read_packet             () const;
    av_packet                   read_packet_selected    () const;
    bool                        packet_in_selected_stream(const av_packet& packet) const;
    media_type_t                read_packet_to_queue    ();
    bool                        read_each_type_to_queues(const std::set<media_type_t>& types_to_read);
    media_type_t                max_packet_queue_type   (const std::set<media_type_t>& media_types_to_test);
    bool                        read_packets_to_queues  (media_type_t mt, uint32_t fill_to_level);
    const packet_queue_t&       packet_queue            (media_type_t mt) const { return packet_queue_per_media_type_[to_size_t(mt)]; }
    const packet_queue_t&       packet_queue_const      (media_type_t mt) const { return packet_queue_per_media_type_[to_size_t(mt)]; }
    void                        packet_queue_pop        (media_type_t mt);
    av_packet                   packet_queue_front      (media_type_t mt);
    av_packet                   packet_queue_pop_front  (media_type_t mt);
    get_packet_fun              get_packet_function     (media_type_t mt);
    std::chrono::microseconds   packet_queue_pts        (media_type_t mt) const;
    // std::chrono::milliseconds   packet_queue_pts_ms     (media_type_t mt) const;
    void                        flush_packet_queues     ();

    // --- Time functions ---
    std::chrono::microseconds   time_from_stream_time   (size_t stream_index, int64_t stream_time_duration) const;
    std::chrono::microseconds   time_from_stream_time   (size_t stream_index, const av_packet& packet) const;
    void                        set_packet_presentation_time (size_t stream_index, av_packet& packet) const;
    std::chrono::microseconds   presentation_time       (const av_packet& packet) const;
    std::chrono::milliseconds   presentation_time_ms    (const av_packet& packet) const;
    std::chrono::microseconds   decode_time             (const av_packet& packet) const;
    std::chrono::milliseconds   decode_time_ms          (const av_packet& packet) const;

    std::chrono::microseconds   presentation_time       (const av_frame& frame) const;
    std::chrono::milliseconds   presentation_time_ms    (const av_frame& frame) const;
    std::chrono::microseconds   best_effort_pts         (const av_frame& frame) const;
    std::chrono::milliseconds   best_effort_pts_ms      (const av_frame& frame) const;

    std::chrono::microseconds   total_time              () const;
    std::chrono::microseconds
                                current_io_operation_duration() const;

    // --- Debug functions ---
    void                        dump                    () const;
    std::string                 queues_info             () const;


private:
    // --- Helper functions ---
    packet_queue_t&             packet_queue_get        (media_type_t mt) { return packet_queue_per_media_type_[to_size_t(mt)]; }

    void                        read_codec_contexts         ();
    size_t                      default_primary_stream_index() const;
    AVStream*                   ff_stream                   (size_t stream_index) const;
    AVCodecParameters*          ff_codec_parameters         (size_t stream_index) const;
    const AVCodec*              ff_find_decoder             (size_t stream_index) const;
    AVRational                  stream_time_base            (size_t stream_index) const { return ff_format_context_->streams[stream_index]->time_base; }
    bool                        custom_io_open_progress_cb  (float progress);
    bool                        custom_io_data_progress_cb  (float progress);


    get_torrents_fn                                         get_torrents_function_;
    std::atomic<stream_state_t>*                            global_stream_state_ptr_= nullptr;
    std::unique_ptr<custom_io_base>                         custom_io_ptr_;
    std::atomic<stream_state_t>                             local_stream_state_     = stream_state_t::inactive;
    std::atomic<bool>                                       cancel_current_io_      = false;
    AVFormatContext*                                        ff_format_context_      = nullptr;
    std::array<size_t, media_type_size()>                   selected_stream_per_media_type_;
    std::array<std::vector<size_t>, media_type_size()>      stream_indices_per_media_type_;
    std::array<packet_queue_t, media_type_size()>           packet_queue_per_media_type_;
    stream_info_vec                                         streams_info_;
    std::string                                             resource_path_;
    size_t                                                  primary_stream_index_   = no_stream_index;
    size_t                                                  packet_queue_capacity_  = 200;
    std::mutex                                              packet_queues_mutex_;
};


} //END namespace cpaf::video

