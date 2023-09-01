#include "streaming_cache.h"

#include <iostream>

using namespace std;
using namespace std::filesystem;

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
    cur_streaming_piece_ = piece;
    // TODO: Implement rest. For example prioritizing and reading pieces to cache
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
        if (!cache_map_.contains(piece)) {
            return false;
        }
    }
    return true;
}

cache_piece_data_t streaming_cache::get_piece_data(libtorrent::piece_index_t piece) const
{
    scoped_lock lock(cache_mutex_);
    auto cpd = cache_map_[piece];
    return cpd;
}

cache_pieces_t streaming_cache::get_pieces_data(const pieces_range_t& range) const
{
    scoped_lock lock(cache_mutex_);
    cache_pieces_t pieces_data;
    for (auto piece = range.piece_begin; piece != range.piece_end; ++piece) {
//        if (piece == 0) {
//            std::cerr << "FIXMENM breakpoint only\n";
//        }
        auto cpd = cache_map_[piece];
        pieces_data.pieces.push_back(cpd);
    }
    pieces_data.piece_begin_start_offset    = range.piece_begin_start_offset;
//    pieces_data.data_size                   = range.data_size;
    return pieces_data;
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
    if (piece < 3) {
        std::cerr << "!!! request_piece !!!  index: " << piece << " is_piece_downloaded_impl: '" << is_piece_downloaded_impl(piece) << "'\n";
    }
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
    if (piece < 3) {
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
    if (piece < 3) {
        std::cerr << "!!! handle_piece_read !!! index: " << piece << "\n";
    }
    scoped_lock lock(cache_mutex_);
    insert_piece_data_impl(rpa);
}


void streaming_cache::prioritize_piece(libtorrent::piece_index_t piece, int32_t deadline_in_ms) const
{
    scoped_lock lock(cache_mutex_);
    if (is_piece_downloaded_impl(piece)) {
        return;
    }

    prioritize_piece_impl(piece, deadline_in_ms);
}

void streaming_cache::prioritize_pieces(const pieces_range_t& range, int32_t deadline_in_ms) const
{
    for (auto piece = range.piece_begin; piece != range.piece_end; piece++) {
        prioritize_piece(piece, deadline_in_ms);
    }
}

bool streaming_cache::read_piece(libtorrent::piece_index_t piece) const
{
    if (is_piece_in_cache(piece)) {
        return true;
    }
    if (is_piece_downloaded(piece)) {
        torrent_handle_.read_piece(piece);
        return true;
    }
    return false;
}

bool streaming_cache::read_pieces(const pieces_range_t& range) const
{
    cerr << "FIXMENM streaming_cache::read_pieces(range)\n";
    bool all_read = true;
    for (auto piece = range.piece_begin; piece != range.piece_end; ++piece) {
        cerr << " piece: '" << piece << "\n";
        const bool could_read = read_piece(piece);
        all_read = all_read && could_read;
    }
    cerr << "FIXMENM DONE streaming_cache::read_pieces(range)\n";
    return all_read;
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
//    if (piece == 0) {
//        std::cerr << "FIXMENM breakpoint only\n";
//    }
    cache_piece_data_t cpd(rpa);
    cache_map_[piece] = std::move(cpd);
}


//bool streaming_cache::push_data(const libtorrent::read_piece_alert* rpa)
//{
//    return fifo_.push(piece_data(rpa));
//}

} // namespace cpaf::torrent
