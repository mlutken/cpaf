#include "file.h"

#include <iostream>
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

size_t file::read(void* buffer, std::size_t bytes_to_read) const
{
    size_t bytes_stil_to_copy = bytes_to_read;
    const auto data_pieces = get_pieces_data(offsett_, bytes_to_read);
    auto* byte_buf = static_cast<std::byte*>(buffer);

    auto start = data_pieces.piece_begin_start_offset;
    for (const auto& data_piece : data_pieces.pieces) {
        if (!data_piece.buffer) {
            std::cerr << "ERROR file::read() data_piece.buffer is NULL\n";
            return 0;
        }
        const size_t bytes_to_copy = std::min(static_cast<size_t>(data_piece.size), bytes_stil_to_copy);
        std::memcpy(byte_buf, &data_piece.buffer[start], bytes_to_copy);
        byte_buf += bytes_to_copy;  // Move destination buffer pointer
        start = 0; // Any following pieces for this data will have start at '0'
        bytes_stil_to_copy -= bytes_to_copy;
        if (bytes_stil_to_copy == 0) {
            break;
        }
    }

    return bytes_to_read - bytes_stil_to_copy;
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
    return 0 <= offsett_ && offsett_ < filesize;
}

std::int64_t file::size() const
{
    return files_storage().file_size(file_index_);
}

int64_t file::offset() const
{
    return files_storage().file_offset(file_index_);
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

cache_piece_data_t file::get_file_data(int64_t offset) const
{

    return parent_torrent_ptr_->get_piece_data(file_index_, offset);

}

cache_pieces_t file::get_pieces_data(int64_t offset, size_t size) const
{
    return parent_torrent_ptr_->get_pieces_data(file_index_, offset, size);
}

pieces_range_t file::get_pieces_range(int64_t offset, size_t size) const
{
    return parent_torrent_ptr_->get_pieces_range(file_index_, offset, size);
}

pieces_range_t file::get_pieces_read_ahead_range(libtorrent::piece_index_t from_piece, size_t read_ahead_size) const
{
    return parent_torrent_ptr_->get_pieces_read_ahead_range(file_index_, from_piece, read_ahead_size);
}


} // namespace torrent
} // namespace cpaf
