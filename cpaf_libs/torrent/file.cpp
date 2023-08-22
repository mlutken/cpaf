#include "file.h"

#include <cpaf_libs/torrent/torrent_utils.h>

namespace cpaf::torrent {

file::file(const std::string& uri_or_name, libtorrent::torrent_handle handle, files* parent_files_ptr) :
    handle_(handle),
    parent_files_ptr_(parent_files_ptr)
{
    if (string_is_uri(uri_or_name)) {
        uri_ = uri_or_name;
    }
    name_ = torrent_name(uri_or_name);
}


std::size_t file::read(void* /*buffer*/, std::size_t /*size_in_bytes*/)
{

    return 0;   // No bytes read!
}

int file::seek(int64_t /*offset*/, int /*whence*/)
{

    return 1; // Error
}

bool file::has_meta_data() const
{
    return cpaf::torrent::has_meta_data(handle_);
}

bool file::is_fully_downloaded() const
{
    return cpaf::torrent::is_fully_downloaded(handle_);
}

} // namespace cpaf::torrent
