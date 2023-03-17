#pragma once

#include <array>
#include <memory>
#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/play_stream.h>

namespace cpaf::video {

class player
{
public:
    player();
    bool                        open                    (const std::string& resource_path);

    bool                        has_source_stream       (stream_type_t sti) const;
    const play_stream*          source_stream           (stream_type_t sti) const;
    play_stream*                source_stream           (stream_type_t sti);

private:

    bool                        open_stream             (const std::string& resource_path, stream_type_t sti);
    bool                        open_primary_stream     (const std::string& resource_path);


    // ----------------------------
    // --- PRIVATE: Member vars ---
    // ----------------------------
//    std::array<play_stream*, stream_type_index_size()>                  source_stream_ptrs_ = {nullptr, nullptr, nullptr, nullptr, nullptr};
    std::array<std::unique_ptr<play_stream>, stream_type_index_size()>  source_streams_ = {nullptr, nullptr, nullptr, nullptr, nullptr};
    std::unique_ptr<play_stream>                                        primary_source_stream_;

};

} //END namespace cpaf::video

