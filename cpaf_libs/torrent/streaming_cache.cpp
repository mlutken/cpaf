#include "streaming_cache.h"

#include <iostream>
#include <thread>

using namespace std;
using namespace std::filesystem;

using namespace std::chrono;
using namespace std::chrono_literals;

constexpr int dbg_print_pieces_lower_than = -1;

namespace cpaf::torrent {

streaming_cache::streaming_cache(libtorrent::torrent_handle handle) :
    torrent_handle_(handle)
{

}

void streaming_cache::insert_piece_data(const libtorrent::read_piece_alert* rpa)
{
    scoped_lock lock(cache_mutex_);
    insert_piece_data_impl(rpa);
}

void streaming_cache::update_current_streaming_piece(libtorrent::piece_index_t piece)
{
    torrent_handle_.clear_piece_deadlines();
    cur_streaming_piece_ = piece;
    // TODO: Implement rest. For example prioritizing and reading pieces to cache
}

void streaming_cache::update_current_streaming_range(pieces_range_t range, int32_t deadline_in_ms)
{
    {
        scoped_lock lock(cache_mutex_);
        clear_current_streaming_range_impl();
        cur_streaming_range_ = range;
    }
    request_pieces(range, deadline_in_ms);
}

void streaming_cache::clear_current_streaming_range()
{
    scoped_lock lock(cache_mutex_);
    clear_current_streaming_range_impl();
}

bool streaming_cache::is_piece_requested(libtorrent::piece_index_t piece) const
{
    scoped_lock lock(cache_mutex_);
    return is_piece_requested_impl(piece);
}

bool streaming_cache::is_piece_downloaded(libtorrent::piece_index_t piece) const
{
    scoped_lock lock(cache_mutex_);
    return is_piece_downloaded_impl(piece);
}

bool streaming_cache::is_piece_in_cache(libtorrent::piece_index_t piece) const
{
    scoped_lock lock(cache_mutex_);
    return is_piece_in_cache_impl(piece);
}

bool streaming_cache::are_pieces_downloaded(const pieces_range_t& range) const
{
    scoped_lock lock(cache_mutex_);
    for (auto piece = range.piece_begin; piece != range.piece_end; ++piece) {
        if (!pieces_downloaded_.contains(piece)) {
            return false;
        }
    }
    return true;
}

bool streaming_cache::are_pieces_in_cache(const pieces_range_t& range) const
{
    scoped_lock lock(cache_mutex_);
    for (auto piece = range.piece_begin; piece != range.piece_end; ++piece) {
        if (!is_piece_in_cache_impl(piece)) {
            return false;
        }
    }
    return true;
}

/// Will block current thread until data ready or timeout reached
cache_pieces_t streaming_cache::get_pieces_data(const pieces_range_t& range, std::chrono::milliseconds timeout) const
{
    cancel_io_state_ = cancel_io_state_t::not_requested;
    request_pieces(range, 0);   // Make sure we start a request for the data first!
    const auto timeout_point = steady_clock::now() + timeout;
    do {
        const cache_pieces_t pieces_data = try_get_pieces_data_impl(range);
        if (pieces_data.is_valid()) {
            return pieces_data;
        }
        if (progress_callback_) {
            const bool should_abort = progress_callback_(-1); // -1 Means we do not know the progress
            if (should_abort) {
                break;
            }
        }
        if (cancel_io_state_ == cancel_io_state_t::requested) {
            cancel_io_state_ = cancel_io_state_t::completed;
            return cache_pieces_t();
        }
        this_thread::sleep_for(io_yield_time_);
    } while( timeout_point > steady_clock::now() );

    return cache_pieces_t();
}

cache_pieces_t streaming_cache::try_get_pieces_data(const pieces_range_t& range) const
{
    request_pieces(range, 0);   // Make sure we start a request for the data first!
    return try_get_pieces_data_impl(range);
}

void streaming_cache::set_piece_downloaded(libtorrent::piece_index_t piece)
{
    scoped_lock lock(cache_mutex_);
    pieces_downloaded_.insert(piece);
}


std::vector<libtorrent::piece_index_t> streaming_cache::all_downloaded_indices() const
{
    scoped_lock lock(cache_mutex_);
    return std::vector<libtorrent::piece_index_t>(pieces_downloaded_.begin(), pieces_downloaded_.end());
}

void streaming_cache::request_piece(libtorrent::piece_index_t piece, int32_t deadline_in_ms) const
{
//    if (piece < dbg_print_pieces_lower_than ) {
//        std::cerr << "!!! request_piece !!!  index: " << piece << " is_piece_downloaded_impl: '" << is_piece_downloaded_impl(piece) << "'\n";
//    }
    scoped_lock lock(cache_mutex_);
    if (is_piece_in_cache_impl(piece)) {
        return;
    }

    if (is_piece_downloaded_impl(piece)) {
        torrent_handle_.read_piece(piece);
        return;
    }

    set_piece_requested_impl(piece);
    prioritize_piece_impl(piece, deadline_in_ms);
}

void streaming_cache::request_pieces(const pieces_range_t& range, int32_t deadline_in_ms) const
{
    for (auto piece = range.piece_begin; piece != range.piece_end; ++piece) {
        request_piece(piece, deadline_in_ms);
    }
}

void streaming_cache::handle_piece_finished(const libtorrent::piece_finished_alert* pfa)
{
    const auto piece = pfa->piece_index;
    if (piece < dbg_print_pieces_lower_than ) {
        std::cerr << "!!! handle_piece_finished !!!  index: " << piece << " is_requested: '" << is_piece_requested_impl(piece) << "'\n";
    }

    scoped_lock lock(cache_mutex_);
    set_piece_downloaded_impl(piece);

    if (is_piece_requested_impl(piece)) {
        read_piece_impl(piece);
        clear_piece_requested_impl(piece);
    }
}

void streaming_cache::handle_piece_read(const libtorrent::read_piece_alert* rpa)
{
    const auto piece = rpa->piece;
    if (piece < dbg_print_pieces_lower_than ) {
        std::cerr << "!!! handle_piece_read !!! index: " << piece << "\n";
    }
    scoped_lock lock(cache_mutex_);
    insert_piece_data_impl(rpa);
}

void streaming_cache::dbg_print_downloaded_indices() const
{
    cerr << "dbg_print_downloaded_indices(): ";
    for (const auto& piece: all_downloaded_indices()) {
        cerr << piece << ", ";
    }
    cerr << endl;
}

void streaming_cache::dbg_print_piece_indices() const
{
    scoped_lock lock(cache_mutex_);
    cerr << "dbg_print_piece_indices(): ";
    for (const auto& cpd: cache_map_) {
        cerr << cpd.second.piece << ", ";
    }
    cerr << endl;
}

void streaming_cache::insert_piece_data_impl(const libtorrent::read_piece_alert* rpa)
{
    const auto piece = rpa->piece;
    cache_piece_data_t cpd(rpa);
    cache_map_[piece] = std::move(cpd);
}

cache_pieces_t streaming_cache::try_get_pieces_data_impl(const pieces_range_t& range) const
{
    scoped_lock lock(cache_mutex_);
    cache_pieces_t pieces_data;
    for (auto piece = range.piece_begin; piece != range.piece_end; ++piece) {
        auto piece_data = cache_map_[piece];
        if (!piece_data.is_valid()) {
            return cache_pieces_t();    // Return invalid/empty range
        }
        pieces_data.pieces.push_back(piece_data);
    }
    if (!pieces_data.is_valid()) {
        return pieces_data;
    }

    pieces_data.pieces[0].data_start_offset = range.piece_begin_data_start_offset();
    pieces_data.data_size                   = range.data_size;
    return pieces_data;
}

void streaming_cache::clear_current_streaming_range_impl()
{
    for (auto piece = cur_streaming_range_.piece_begin; piece != cur_streaming_range_.piece_end; ++piece) {
        torrent_handle_.reset_piece_deadline(piece);
    }
}


} // namespace cpaf::torrent
