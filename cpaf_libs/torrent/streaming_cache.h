#pragma once

#include <mutex>
#include <vector>
#include <unordered_map>
#include <libtorrent/alert_types.hpp>
#include <cpaf_libs/torrent/torrent_utils.h>

namespace cpaf::torrent {



class streaming_cache
{
public:


    void                                insert_piece_data               (const lt::read_piece_alert* rpa);


    bool                                has_piece                       (lt::piece_index_t piece) const;
    cache_piece_data_t                  get_piece_data                  (lt::piece_index_t piece) const;
    cache_pieces_t                      get_pieces_data                 (const pieces_range_t& range) const;

    void                                set_piece_downloaded            (lt::piece_index_t piece) ;
    bool                                is_piece_downloaded             (lt::piece_index_t piece) const;
    std::vector<lt::piece_index_t>      all_downloaded_indices          () const;

    void                                dbg_print_downloaded_indices    () const;
    void                                dbg_print_piece_indices         () const;

private:
    using pieces_downloaded_set_t = std::unordered_set<lt::piece_index_t>;

    mutable std::unordered_map<lt::piece_index_t, cache_piece_data_t>   cache_map_;
    pieces_downloaded_set_t                                             pieces_downloaded_;
    mutable std::mutex                                                  cache_mutex_;

};

} // namespace cpaf::torrent

