#include "torrent.h"

#include <iostream>
#include <cpaf_libs/torrent/torrent_utils.h>
#include <cpaf_libs/torrent/torrents.h>

using namespace std;


namespace cpaf::torrent {

torrent::torrent(const std::string& uri_or_name, libtorrent::torrent_handle handle, torrents* parent_torrents_ptr) :
    handle_(handle),
    parent_torrents_ptr_(parent_torrents_ptr)
{
    if (string_is_uri(uri_or_name)) {
        uri_ = uri_or_name;
    }
    name_ = torrent_name(uri_or_name);
    cerr << "torrent::CONSTRUCTOR: '" << name_ << "'\n";
}

file torrent::open(libtorrent::file_index_t file_index)
{
    return file(file_index, handle_, this);
}


std::size_t torrent::read(void* /*buffer*/, std::size_t /*size_in_bytes*/)
{

    return 0;   // No bytes read!
}

int torrent::seek(int64_t /*offset*/, int /*whence*/)
{

    return 1; // Error
}

std::string torrent::largest_file_name() const
{
    return cpaf::torrent::largest_file_name(handle_);
}

libtorrent::file_index_t torrent::largest_file_index() const
{
    return cpaf::torrent::largest_file_index(handle_);
}

std::filesystem::path torrent::largest_file_local_file_path() const
{
    return cpaf::torrent::largest_file_local_file_path(handle_, parent_torrents_ptr_->base_torrents_path());
}

std::vector<std::string> torrent::all_file_names() const
{
    return cpaf::torrent::all_file_names(handle_);
}

std::vector<std::string> torrent::all_file_paths() const
{
    return cpaf::torrent::all_file_paths(handle_);
}

int torrent::number_of_files() const
{
    const auto torinfo_ptr = handle_.torrent_file();
    if (!torinfo_ptr) { return -1; }
    return torinfo_ptr->num_files();
}

libtorrent::file_index_t torrent::file_index_at_offset(int64_t offset) const
{
    const auto torinfo_ptr = handle_.torrent_file();
    if (!torinfo_ptr) { return -1; }
    return files_storage().file_index_at_offset(offset);

}

libtorrent::file_index_t torrent::file_index_at_piece(libtorrent::piece_index_t piece) const
{
    const auto torinfo_ptr = handle_.torrent_file();
    if (!torinfo_ptr) { return -1; }
    return files_storage().file_index_at_piece(piece);
}

std::vector<libtorrent::file_slice> torrent::map_block(libtorrent::piece_index_t piece, int64_t offset, int64_t size) const
{
    const auto torinfo_ptr = handle_.torrent_file();
    if (!torinfo_ptr) { return std::vector<libtorrent::file_slice>(); }
    return files_storage().map_block(piece, offset, size);
}

int torrent::piece_length() const
{
    const auto torinfo_ptr = handle_.torrent_file();
    if (!torinfo_ptr) { return 0; }
    return torinfo_ptr->piece_length();
}

int torrent::num_pieces() const
{
    const auto torinfo_ptr = handle_.torrent_file();
    if (!torinfo_ptr) { return 0; }
    return torinfo_ptr->num_pieces();
}

bool torrent::has_meta_data() const
{
    return cpaf::torrent::has_meta_data(handle_);
}

bool torrent::is_fully_downloaded() const
{
    return cpaf::torrent::is_fully_downloaded(handle_);
}

void torrent::read_piece(libtorrent::piece_index_t piece) const
{
    handle_.read_piece(piece);
}

void torrent::set_piece_downloaded(libtorrent::piece_index_t piece)
{
    pieces_downloaded_.insert(piece);
}

bool torrent::is_piece_downloaded(libtorrent::piece_index_t piece) const
{
    return pieces_downloaded_.contains(piece);
}

bool torrent::read_all_downloaded_pieces() const
{
    if (!has_meta_data()) {
        return false;
    }
    for (const auto& piece: pieces_downloaded_) {
        handle_.read_piece(piece);
    }
    return true;
}


const libtorrent::file_storage& torrent::files_storage() const
{
    return handle_.torrent_file()->files();
}

} // namespace cpaf::torrent
