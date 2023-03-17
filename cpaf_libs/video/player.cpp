#include "player.h"

namespace cpaf::video {

player::player()
{

}

/// @todo need to support reading "hybrid" resources that (potentially) specify different streams
///       for video, audio, key_frames etc.. For now we simply open a single primary stream with this.
bool player::open(const std::string& resource_path)
{
    return open_primary_stream(resource_path);
}

bool player::has_source_stream(stream_type_t sti) const
{
    return source_stream(sti) == nullptr;
}

// TODO: A little convoluted this implementation
const play_stream* player::source_stream(stream_type_t sti) const
{
    const auto index = to_size_t(sti);
    if (source_streams_[index]) {
        return source_streams_[index].get();
    }
    if (primary_source_stream_) {
        if (sti == stream_type_t::video && primary_source_stream_->has_media_type(media_type::video)) {
            return primary_source_stream_.get();
        }
        else if (sti == stream_type_t::audio && primary_source_stream_->has_media_type(media_type::audio)) {
            return primary_source_stream_.get();
        }
        else if (sti == stream_type_t::subtitle && primary_source_stream_->has_media_type(media_type::subtitle)) {
            return primary_source_stream_.get();
        }
    }
    return nullptr;
}

// TODO: A little convoluted this implementation
play_stream* player::source_stream(stream_type_t sti)
{
    const auto index = to_size_t(sti);
    if (source_streams_[index]) {
        return source_streams_[index].get();
    }
    if (primary_source_stream_) {
        if (sti == stream_type_t::video && primary_source_stream_->has_media_type(media_type::video)) {
            return primary_source_stream_.get();
        }
        else if (sti == stream_type_t::audio && primary_source_stream_->has_media_type(media_type::audio)) {
            return primary_source_stream_.get();
        }
        else if (sti == stream_type_t::subtitle && primary_source_stream_->has_media_type(media_type::subtitle)) {
            return primary_source_stream_.get();
        }
    }
    return nullptr;
}

bool player::open_stream(const std::string& resource_path, stream_type_t sti)
{
    const auto index = to_size_t(sti);
    source_streams_[index] = std::make_unique<play_stream>();
    const auto open_ok = source_streams_[index]->open(resource_path);

    return open_ok;
}

bool player::open_primary_stream(const std::string& resource_path)
{
    primary_source_stream_ = std::make_unique<play_stream>();
    const auto open_ok = primary_source_stream_->open(resource_path);
    return open_ok;
}

} //END namespace cpaf::video

