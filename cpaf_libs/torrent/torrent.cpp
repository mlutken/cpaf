#include "torrent.h"

#include <iostream>
#include <cpaf_libs/torrent/torrent_utils.h>
#include <cpaf_libs/torrent/torrents.h>

using namespace std;


namespace cpaf::torrent {

torrent::torrent(const std::string& uri_or_name, libtorrent::torrent_handle handle, torrents* parent_torrents_ptr) :
    handle_(handle),
    piece_data_cache_(handle),
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

file torrent::open_streaming(libtorrent::file_index_t file_index, size_t read_ahead_size)
{
    auto f = open(file_index);
    f.read_ahead_size_set(read_ahead_size);
    return f;
}

file torrent::open_largest_file_streaming(size_t read_ahead_size)
{
    return open_streaming(largest_file_index(), read_ahead_size);
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

lt::index_range<libtorrent::file_index_t> torrent::all_file_indices() const
{
    auto torinfo_ptr = handle_.torrent_file();
    if (!torinfo_ptr) {
        return lt::index_range<libtorrent::file_index_t>();
    }
    const auto& storage = torinfo_ptr->files();
    return storage.file_range();
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

bool torrent::prepare_streaming()
{
    if (!has_meta_data()) {
        return false;
    }
    pieces_range_t range = get_pieces_range(largest_file_index(), 0, default_read_ahead_size_);
    piece_data_cache_.request_pieces(range);

    return true;
}

void torrent::request_pieces(const pieces_range_t& range, int32_t deadline_in_ms) const
{
    piece_data_cache_.request_pieces(range, deadline_in_ms);
}

libtorrent::piece_index_t torrent::file_offset_to_piece_index(libtorrent::file_index_t file_index, int64_t offset) const
{
    lt::peer_request pr = files_storage().map_file(file_index, offset, 1);
    return pr.piece;
}

libtorrent::peer_request torrent::file_offset_to_peer_request(libtorrent::file_index_t file_index, int64_t offset, size_t size) const
{
    return files_storage().map_file(file_index, offset, size);
}

/// Will block current thread until data ready or timeout reached
cache_pieces_t torrent::get_pieces_data(libtorrent::file_index_t file_index, int64_t offset, size_t size, std::chrono::milliseconds timeout) const
{
    const auto range = get_pieces_range(file_index, offset, size);
    return piece_data_cache_.get_pieces_data(range, timeout);
}

pieces_range_t torrent::get_pieces_range(libtorrent::file_index_t file_index, int64_t offset, size_t size) const
{
    const auto piece_len = piece_length();
    if (piece_len <= 0) { return pieces_range_t(); }

    pieces_range_t range;

    range.file_index = file_index;
    range.data_size = size;

    libtorrent::peer_request  pr = file_offset_to_peer_request(file_index, offset, size);
    range.piece_begin_start_offset = pr.start;
    range.piece_begin = pr.piece;

    // Set default piece_end to one-past the piece_begin
    range.piece_end = range.piece_begin;
    range.piece_end++;

    //
    auto size_to_get = static_cast<int64_t>(size);
    const int64_t bytes_in_first_piece = piece_len - pr.start;
    if (bytes_in_first_piece >= size_to_get) {
        return range;
    }

    // If we get here we need more than one piece to represent the data

    size_to_get = size_to_get - bytes_in_first_piece;
    const auto modulus_rest = size_to_get % piece_len;
    int64_t additional_pieces_to_get = size_to_get / piece_len;
    if (modulus_rest !=0 ){
        ++additional_pieces_to_get;
    }

    if (additional_pieces_to_get > num_pieces()) {
        return pieces_range_t();
    }

    // Increase range
    range.piece_end = range.piece_end + lt::piece_index_t(additional_pieces_to_get);

    return range;
}

pieces_range_t torrent::get_pieces_read_ahead_range(libtorrent::file_index_t file_index, libtorrent::piece_index_t from_piece, size_t read_ahead_size) const
{
    const auto piece_len = piece_length();
    if (piece_len <= 0) { return pieces_range_t(); }

    pieces_range_t range;
    range.file_index = file_index;
    range.data_size = read_ahead_size;
    range.piece_begin = from_piece;

    // Set default piece_end to one-past the piece_begin
    range.piece_end = range.piece_begin;
    range.piece_end++;


    const auto additional_size_to_read_ahead = static_cast<int64_t>(read_ahead_size) - piece_len;
    const auto modulus_rest = additional_size_to_read_ahead % piece_len;
    int64_t additional_pieces_to_get = additional_size_to_read_ahead / piece_len;
    if (modulus_rest !=0 ){
        ++additional_pieces_to_get;
    }


    range.piece_end = range.piece_end + lt::piece_index_t(additional_pieces_to_get);

    return range;
}

pieces_range_t torrent::get_pieces_read_ahead_range(libtorrent::file_index_t file_index, int64_t offset, size_t read_ahead_size) const
{
    const auto piece = file_offset_to_piece_index(file_index, offset);
    return get_pieces_range(file_index, piece, read_ahead_size);
}

bool torrent::are_pieces_in_cache(const pieces_range_t& range) const
{
    return piece_data_cache_.are_pieces_in_cache(range);
}

void torrent::handle_piece_finished(const libtorrent::piece_finished_alert* pfa)
{
    piece_data_cache_.handle_piece_finished(pfa);
}

void torrent::handle_piece_read(const libtorrent::read_piece_alert* rpa)
{
    piece_data_cache_.handle_piece_read(rpa);
}

void torrent::handle_torrent_finished(const libtorrent::torrent_finished_alert* tfa)
{
    std::cerr << "!!! handle_torrent_finished !!! Torrent Name: " << tfa->torrent_name() << "\n";
}

void torrent::handle_torrent_error(const libtorrent::torrent_error_alert* tea)
{
    std::cerr << "!!! handle_torrent_error !!! Torrent Name: " << tea->torrent_name() << "\n";
}

void torrent::dbg_print_downloaded_indices() const
{
    piece_data_cache_.dbg_print_downloaded_indices();
}

void torrent::dbg_print_cache_piece_indices() const
{
    piece_data_cache_.dbg_print_piece_indices();
}


const libtorrent::file_storage& torrent::files_storage() const
{
    return handle_.torrent_file()->files();
}

} // namespace cpaf::torrent
