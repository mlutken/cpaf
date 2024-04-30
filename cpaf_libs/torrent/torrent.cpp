#include "torrent.h"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <cpaf_libs/torrent/torrent_utils.h>
#include <cpaf_libs/torrent/torrents.h>

using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;


namespace cpaf::torrent {

torrent::torrent(const std::string& uri_or_name, libtorrent::torrent_handle handle, torrents* parent_torrents_ptr) :
    handle_(handle),
    piece_data_cache_(handle),
    parent_torrents_ptr_(parent_torrents_ptr)
{
    if (is_magnet_uri(uri_or_name)) {
        uri_ = uri_or_name;
    }
    name_ = torrent_name(uri_or_name);
}


file torrent::open(libtorrent::file_index_t file_index)
{
    return file(file_index, handle_, this);
}

file torrent::open(const string_view file_path)
{
    return open(file_path_to_index(file_path));
}

file torrent::open_streaming(libtorrent::file_index_t file_index, size_t read_ahead_size)
{
    auto f = open(file_index);
    f.read_ahead_size_set(read_ahead_size);
    return f;
}

file torrent::open_streaming(std::string_view file_path, size_t read_ahead_size)
{
    return open_streaming(file_path_to_index(file_path), read_ahead_size);
}

file torrent::open_largest_file_streaming(size_t read_ahead_size)
{
    return open_streaming(largest_file_index(), read_ahead_size);
}

bool torrent::wait_for_meta_data(std::chrono::milliseconds timeout)
{
    cancel_io_state_ = cancel_io_state_t::not_requested;
    const auto timeout_point = steady_clock::now() + timeout;
    do {
        if (has_meta_data()) {
            return true;
        }
        if (cancel_io_state_ == cancel_io_state_t::requested) {
            cancel_io_state_ = cancel_io_state_t::completed;
            return false;
        }
        this_thread::sleep_for(io_yield_time_);
    } while(timeout_point > steady_clock::now());

    return false;
}

/** Request to cancel ongoing IO operation.
 *
 *  Both the wait_for_meta_data() and/or get_pieces_data() will be cancelled.
 *  Most likely you only have either of these two blocking operations outstanding.
 *
 *  Call torrent::cancel_io_completed() if you want to know if this was the cause.
 *
 *  @return True if either wait_for_meta_data() or get_pieces_data() was cancelled.
 *  */
void torrent::cancel_current_io_operation()
{
    piece_data_cache_.cancel_current_io_operation();
    cancel_io_state_ = cancel_io_state_t::requested;
}

/** Return true in when requested cancel is completed.
 *  @note Only makes sense to call if one just previously has called cancel_current_io_operation()
 *
 *  @return True if either wait_for_meta_data() or get_pieces_data() was cancelled.
 *  */
bool torrent::cancel_io_completed() const
{
    return  piece_data_cache_.cancel_io_state() == cancel_io_state_t::completed ||
            cancel_io_state_ == cancel_io_state_t::completed
        ;
}

void torrent::remove()
{
    parent_torrents_ptr_->remove_torrent(*this);
}


/// @todo	Torrents that are auto-managed may be automatically resumed again. It
/// 	does not make sense to pause an auto-managed torrent without making it
/// 	not auto-managed first. Torrents are auto-managed by default when added
/// 	to the session. For more information, see queuing_.
void torrent::pause()
{
    handle_.pause();
}


/// @todo This name thing does not really work as it should. Think we need to wait for metadata
string torrent::name() const
{
    const auto torinfo_ptr = handle_.torrent_file();
    if (!torinfo_ptr) {
        return name_;
    }
    return torinfo_ptr->name();
}

std::string torrent::largest_file_name() const
{
    return cpaf::torrent::largest_file_name(handle_);
}

libtorrent::file_index_t torrent::largest_file_index() const
{
    return cpaf::torrent::largest_file_index(handle_);
}

int64_t torrent::file_size(libtorrent::file_index_t file_index) const
{
    return cpaf::torrent::file_size(handle_, file_index);
}

std::filesystem::path torrent::base_local_file_dir() const
{
    return cpaf::torrent::base_local_file_dir(handle_);
}

std::filesystem::path torrent::largest_file_local_file_path() const
{
    return cpaf::torrent::largest_file_local_file_path(handle_);
}

std::filesystem::path torrent::local_file_path() const
{
    return cpaf::torrent::torrent_local_file_path(handle_);
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

libtorrent::file_index_t torrent::file_path_to_index(std::string_view file_path) const
{
    const auto all_paths = all_file_paths();
    const auto path_it = std::ranges::find(all_paths, file_path);
    if (path_it != all_paths.end()) {
        return path_it - all_paths.begin();
    }

    const auto all_names = all_file_names();
    const auto name_it = std::ranges::find(all_names, file_path);
    if (name_it != all_names.end()) {
        return name_it - all_names.begin();
    }
    return libtorrent::file_index_t(-1);
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


libtorrent::piece_index_t torrent::piece_index_end() const
{
    const auto torinfo_ptr = handle_.torrent_file();
    if (!torinfo_ptr) { return 0; }
    return torinfo_ptr->last_piece();
}

void torrent::ensure_piece_range_valid(pieces_range_t& range) const
{
    if (range.piece_end > piece_index_end()) { range.piece_end =  piece_index_end(); }
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
    ensure_piece_range_valid(range);
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
    ensure_piece_range_valid(range);
    return range;
}

pieces_range_t torrent::get_pieces_read_ahead_range(libtorrent::file_index_t file_index, int64_t offset, size_t read_ahead_size) const
{
    const auto piece = file_offset_to_piece_index(file_index, offset);
    return get_pieces_read_ahead_range(file_index, piece, read_ahead_size);
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
