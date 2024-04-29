#include "custom_io_base.h"
#include "torrent_io.h"
#include "my_file_io.h"
#include <cpaf_libs/unicode/cpaf_u8string_utils.h>

using namespace std::chrono;

namespace cpaf::video {

std::unique_ptr<custom_io_base> custom_io_base::create( const std::string& resource_path,
                                                        get_torrents_fn get_torrents_function
                                                       )
{
    const std::string protocol_name = protocol_from_uri(resource_path);

    std::unique_ptr<custom_io_base> ptr;
    if (protocol_name == "magnet") {
        ptr = std::make_unique<torrent_io>(get_torrents_function());
    }
    else if (protocol_name == "myfile") { ptr = std::make_unique<my_file_io>(); }
    return ptr;
}

custom_io_base::custom_io_base()
{
}


custom_io_base::~custom_io_base()
{
    cleanup();
}

bool custom_io_base::init(AVFormatContext* ff_format_context)
{
    ff_avio_context_buffer_ = static_cast<uint8_t*>(av_malloc(buffer_size()));
    if (!ff_avio_context_buffer_) {
        return false;
    }

    ff_avio_context_ = avio_alloc_context(ff_avio_context_buffer_, buffer_size(),
                                  0,
                                  this,
                                  &custom_io_base::read_packet_wrapper,
                                  &custom_io_base::write_packet_wrapper,
                                  &custom_io_base::seek_wrapper);

    if (!ff_avio_context_) {
        // TODO: Do we need to call
            // av_freep(&ff_avio_context_buffer_);
            // ?
            // Or is the destructor av_freep(&ff_avio_context_->buffer); enough?
            // Seems it requires that avio_alloc_context sets the ff_avio_context_->buffer before doing to much else!
        return false;
    }

    // Use our custom io in the given format_context
    if (ff_format_context) {
        ff_format_context->pb = ff_avio_context_;
    }
    return true;
}

std::chrono::microseconds custom_io_base::current_io_operation_duration() const
{
    if (last_io_op_started_time_ == time_point_t()) {
        return microseconds(0);
    }
    const auto duration = steady_clock::now() - last_io_op_started_time_;
    return duration_cast<microseconds>(duration);
}

/// @todo Do we need to call
/// av_freep(&ff_avio_context_buffer_);
/// ?
/// Or is the destructor av_freep(&ff_avio_context_->buffer); enough?
/// Seems it requires that avio_alloc_context sets the ff_avio_context_->buffer before doing to much else!
void custom_io_base::cleanup()
{
    if (ff_avio_context_) {
        av_freep(&ff_avio_context_->buffer);
        avio_context_free(&ff_avio_context_);
        ff_avio_context_ = nullptr;
    }

    // TODO: Is this needed? See function doc comments above!
    // if (ff_avio_context_buffer_) {
    //     av_freep(&ff_avio_context_buffer_);
    //     ff_avio_context_buffer_ = nullptr;
    // }

    ff_avio_context_buffer_ = nullptr;
}

int custom_io_base::read_packet(uint8_t* buf, int buf_size)
{
    mark_io_begin();
    const auto retval = do_read_packet(buf, buf_size);
    mark_io_done();

    return retval;
}

int custom_io_base::write_packet(uint8_t* buf, int buf_size)
{
    mark_io_begin();
    const auto retval = do_write_packet(buf, buf_size);
    mark_io_done();

    return retval;
}

int64_t custom_io_base::seek(int64_t offset, int whence)
{
    mark_io_begin();
    const auto retval = do_seek(offset, whence);
    mark_io_done();

    return retval;
}

bool custom_io_base::open(const std::string& resource_path)
{
    mark_io_begin();
    resource_path_ = resource_path;
    const auto retval = do_open(resource_path);
    mark_io_done();

    return retval;
}

void custom_io_base::close() {
    mark_io_begin();
    do_close();
    cleanup();
    mark_io_done();
}

int custom_io_base::do_write_packet(uint8_t* , int )
{
    return AVERROR_STREAM_NOT_FOUND;
}

int custom_io_base::read_packet_wrapper(void* opaque, uint8_t* buf, int buf_size)
{
    // AVERROR_EXTERNAL, AVERROR_EOF
    auto* self = static_cast<custom_io_base*>(opaque);
    return self->read_packet(buf, buf_size);
}

int custom_io_base::write_packet_wrapper(void* opaque, uint8_t* buf, int buf_size)
{
    auto* self = static_cast<custom_io_base*>(opaque);
    return self->write_packet(buf, buf_size);
}

int64_t custom_io_base::seek_wrapper(void* opaque, int64_t offset, int whence)
{
    auto* self = static_cast<custom_io_base*>(opaque);
    return self->seek(offset, whence);
}

} //END namespace cpaf::video
