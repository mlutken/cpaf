#include "streaming_cache.h"

#include <iostream>

using namespace std;
using namespace std::filesystem;

namespace cpaf::torrent {

void streaming_cache::insert_piece_data(const libtorrent::read_piece_alert* rpa)
{
    if (rpa) {
        scoped_lock lock(cache_mutex_);
        if (rpa->piece == 0) {
            std::cerr << "FIXMENM breakpoint only\n";
        }
        cache_piece_data_t cpd(rpa);
        cache_map_[rpa->piece] = std::move(cpd);
    }
}

bool streaming_cache::has_piece(libtorrent::piece_index_t piece) const
{
    scoped_lock lock(cache_mutex_);
    return cache_map_.contains(piece);
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
        if (piece == 0) {
            std::cerr << "FIXMENM breakpoint only\n";
        }
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

bool streaming_cache::is_piece_downloaded(libtorrent::piece_index_t piece) const
{
    scoped_lock lock(cache_mutex_);
    return pieces_downloaded_.contains(piece);
}

std::vector<libtorrent::piece_index_t> streaming_cache::all_downloaded_indices() const
{
    scoped_lock lock(cache_mutex_);
    return std::vector<libtorrent::piece_index_t>(pieces_downloaded_.begin(), pieces_downloaded_.end());
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


//bool streaming_cache::push_data(const libtorrent::read_piece_alert* rpa)
//{
//    return fifo_.push(piece_data(rpa));
//}

} // namespace cpaf::torrent
