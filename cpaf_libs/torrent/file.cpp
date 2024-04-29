#include "file.h"

#include <iostream>
#include <fmt/format.h>
#include <cpaf_libs/torrent/torrent.h>

using namespace std;
using namespace std::filesystem;

namespace cpaf {
namespace torrent {

file::file(libtorrent::file_index_t file_index, libtorrent::torrent_handle handle, torrent* parent_torrent_ptr) :
    file_index_(file_index),
    handle_(handle),
    parent_torrent_ptr_(parent_torrent_ptr)
{

}

size_t file::read(void* buffer, const std::size_t bytes_to_read, const std::chrono::milliseconds timeout)
{
    size_t bytes_stil_to_copy = bytes_to_read;
    const auto data_pieces = get_pieces_data(offsett_, bytes_to_read, timeout); // Will block current thread until data ready or timeout reached
    auto* byte_buf = static_cast<std::byte*>(buffer);

    for (const auto& data_piece : data_pieces.pieces) {
        if (!data_piece.buffer) {
            std::cerr << "ERROR file::read() data_piece.buffer is NULL\n";
            return 0;
        }
        if (!data_piece.is_valid()) {
            std::cerr << "ERROR file::read() data_piece is invalid!\n";
            return 0;
        }
        const size_t bytes_to_copy = std::min(static_cast<size_t>(data_piece.bytes_left()), bytes_stil_to_copy);
        std::memcpy(byte_buf, data_piece.buffer_begin(), bytes_to_copy);
        byte_buf += bytes_to_copy;  // Move destination buffer pointer
        bytes_stil_to_copy -= bytes_to_copy;
        if (bytes_stil_to_copy == 0) {
            break;
        }
    }

    if (streaming_mode_active()) {
        const auto read_ahead_range = get_pieces_read_ahead_range(data_pieces.last_piece_index());
//        fmt::println("FIXMENM: Read [{}] : Request read ahead: {}", (int)data_pieces.last_piece_index(), read_ahead_range.dbg_string());
        parent_torrent_ptr_->request_pieces(read_ahead_range, 0);
    }

    const size_t bytes_read = bytes_to_read - bytes_stil_to_copy;
    offsett_ += static_cast<int64_t>(bytes_read);
    return bytes_read;
}

// from stdio.h
//#define SEEK_SET	0	/* Seek from beginning of file.  */
//#define SEEK_CUR	1	/* Seek from current position.  */
//#define SEEK_END	2	/* Seek from end of file.  */

int file::seek(int64_t offset, int whence)
{
    const auto filesize = size();
    switch (whence) {
    case SEEK_SET:
        offsett_ = offset;
    break;
    case SEEK_CUR:
        offsett_ = offsett_ + offset;
    break;
    case SEEK_END:
        offsett_ = filesize - abs(offset);
    break;
    }

    const bool is_valid_offset = (0 <= offsett_) && (offsett_ < filesize);
    if (is_valid_offset) {
        request_pieces_from_offset();
        if (use_blocking_seek_) {
            auto pieces = get_pieces_data(offsett_, 1024*16, blocking_seek_timeout_);
            return pieces.is_valid() ? 0 : -1;
        }
        else {
            return 0;
        }
    }
    return -1;
}

void file::request_pieces_from_offset()
{
    const bool is_valid_offset = (0 <= offsett_) && (offsett_ < size());
    if (is_valid_offset && streaming_mode_active()) {
        const auto read_ahead_range = get_pieces_read_ahead_range(offsett_);
        // fmt::println("FIXMENM: Seek: Request read ahead: {}", read_ahead_range.dbg_string());
        parent_torrent_ptr_->request_pieces(read_ahead_range, 0);
    }
}

void file::remove_parent_torrent()
{
    parent_torrent_ptr_->remove();
}

void file::pause_parent_torrent()
{
    parent_torrent_ptr_->pause();
}

/// @todo cpaf::torrent::file::close() not implemented !!!
void file::close()
{
    std::cerr << "TODO TODO cpaf::torrent::file::close()\n";
}

/** Is file is ready for streaming?
 *  Test if given the current file offset and read_ahead_size_ are the pieces then ready in the cache, so we can start streaming this file.
 *  @note This function only makes sense if the file is in streaming mode!
*/
bool file::are_streaming_pieces_in_cache() const
{
    const auto read_ahead_range = get_pieces_read_ahead_range(offsett_);
    return parent_torrent_ptr_->are_pieces_in_cache(read_ahead_range);
}

std::int64_t file::size() const
{
    return files_storage().file_size(file_index_);
}

std::string file::name() const
{
    return string(files_storage().file_name(file_index_));
}

std::string file::path() const
{
    return string(files_storage().file_path(file_index_));
}

libtorrent::piece_index_t file::piece_index_start() const
{
    return files_storage().piece_index_at_file(file_index_);
}

libtorrent::peer_request file::file_offset_to_peer_request(int64_t offset, int size) const
{
    return files_storage().map_file(file_index_, offset, size);
}

const libtorrent::file_storage& file::files_storage() const
{
    return handle_.torrent_file()->files();
}

/// Will block current thread until data ready or timeout reached
cache_pieces_t file::get_pieces_data(int64_t offset, size_t size, std::chrono::milliseconds timeout) const
{
    return parent_torrent_ptr_->get_pieces_data(file_index_, offset, size, timeout);
}

pieces_range_t file::get_pieces_range(int64_t offset, size_t size) const
{
    return parent_torrent_ptr_->get_pieces_range(file_index_, offset, size);
}

pieces_range_t file::get_pieces_read_ahead_range(libtorrent::piece_index_t from_piece, size_t read_ahead_size) const
{
    return parent_torrent_ptr_->get_pieces_read_ahead_range(file_index_, from_piece, read_ahead_size);
}

pieces_range_t file::get_pieces_read_ahead_range(libtorrent::piece_index_t from_piece) const
{
    return parent_torrent_ptr_->get_pieces_read_ahead_range(file_index_, from_piece, read_ahead_size());
}

pieces_range_t file::get_pieces_read_ahead_range(int64_t offset) const
{
    return parent_torrent_ptr_->get_pieces_read_ahead_range(file_index_, offset, read_ahead_size());
}


} // namespace torrent
} // namespace cpaf
