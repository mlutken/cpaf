#include "file.h"

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
    const auto data_pieces = get_file_data(offsett_, bytes_to_read);

    for (const auto& data_piece : data_pieces) {
        const size_t bytes_to_copy = std::min(static_cast<size_t>(data_piece.size), bytes_stil_to_copy);
        memcpy(buffer, &data_piece.buffer[0], bytes_to_copy);
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

libtorrent::peer_request file::map_file(int64_t offset, int size) const
{
    return files_storage().map_file(file_index_, offset, size);
}

const libtorrent::file_storage& file::files_storage() const
{
    return handle_.torrent_file()->files();
}

cache_piece_data file::get_file_data(int64_t offset) const
{
    cache_piece_data pd;

    return pd;

}

std::vector<cache_piece_data> file::get_file_data(int64_t offset, int size) const
{
    std::vector<cache_piece_data> data_pieces;


    return data_pieces;
}


} // namespace torrent
} // namespace cpaf
