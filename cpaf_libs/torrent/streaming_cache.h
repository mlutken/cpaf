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

    explicit streaming_cache(lt::torrent_handle handle);

    void                                insert_piece_data               (const lt::read_piece_alert* rpa);
    void                                update_current_streaming_piece  (lt::piece_index_t piece);


    bool                                is_piece_in_cache               (lt::piece_index_t piece) const;
    bool                                are_pieces_in_cache             (const pieces_range_t& range) const;
    cache_piece_data_t                  get_piece_data                  (lt::piece_index_t piece) const;
    cache_pieces_t                      get_pieces_data                 (const pieces_range_t& range) const;

    void                                set_piece_downloaded            (lt::piece_index_t piece) ;
    bool                                is_piece_downloaded             (lt::piece_index_t piece) const;
    bool                                are_pieces_downloaded           (const pieces_range_t& range) const;
    std::vector<lt::piece_index_t>      all_downloaded_indices          () const;

    bool                                read_piece                      (lt::piece_index_t piece) const;
    bool                                read_pieces                     (const pieces_range_t& range) const;

    void                                prioritize_piece                (lt::piece_index_t piece, int32_t deadline_in_ms = 0) const;
    void                                prioritize_pieces               (const pieces_range_t& range, int32_t deadline_in_ms = 0) const;

    void                                dbg_print_downloaded_indices    () const;
    void                                dbg_print_piece_indices         () const;

private:
    using pieces_downloaded_set_t = std::unordered_set<lt::piece_index_t>;

    lt::torrent_handle                                                  torrent_handle_;
    mutable std::unordered_map<lt::piece_index_t, cache_piece_data_t>   cache_map_;
    pieces_downloaded_set_t                                             pieces_downloaded_;
    mutable std::mutex                                                  cache_mutex_;
    lt::piece_index_t                                                   cur_streaming_piece_ = 0;

};

} // namespace cpaf::torrent

