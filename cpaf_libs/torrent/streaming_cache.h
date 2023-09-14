#pragma once

#include <mutex>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <libtorrent/alert_types.hpp>
#include <cpaf_libs/torrent/torrent_utils.h>

namespace cpaf::torrent {



class streaming_cache
{
public:

    explicit streaming_cache(lt::torrent_handle handle);

    void                                insert_piece_data               (const lt::read_piece_alert* rpa);
    void                                update_current_streaming_piece  (lt::piece_index_t piece);
    void                                update_current_streaming_range  (pieces_range_t range, int32_t deadline_in_ms);
    void                                clear_current_streaming_range   ();


    bool                                is_piece_requested              (lt::piece_index_t piece) const;
    bool                                is_piece_downloaded             (lt::piece_index_t piece) const;
    bool                                is_piece_in_cache               (lt::piece_index_t piece) const;

    bool                                are_pieces_downloaded           (const pieces_range_t& range) const;
    bool                                are_pieces_in_cache             (const pieces_range_t& range) const;

    cache_pieces_t                      get_pieces_data                 (const pieces_range_t& range, std::chrono::milliseconds timeout) const;
    cache_pieces_t                      try_get_pieces_data             (const pieces_range_t& range) const;

    void                                set_piece_downloaded            (lt::piece_index_t piece) ;
    std::vector<lt::piece_index_t>      all_downloaded_indices          () const;

    void                                request_piece                   (lt::piece_index_t piece, int32_t deadline_in_ms = 0) const;
    void                                request_pieces                  (const pieces_range_t& range, int32_t deadline_in_ms = 0) const;
    void                                handle_piece_finished           (const lt::piece_finished_alert* pfa);
    void                                handle_piece_read               (const lt::read_piece_alert* rpa);

    void                                dbg_print_downloaded_indices    () const;
    void                                dbg_print_piece_indices         () const;

private:

    bool                                is_piece_requested_impl         (lt::piece_index_t piece) const { return pieces_requested_.contains(piece); }
    bool                                is_piece_downloaded_impl        (lt::piece_index_t piece) const { return pieces_downloaded_.contains(piece); }
    bool                                is_piece_in_cache_impl          (lt::piece_index_t piece) const { return cache_map_[piece].is_valid(); }

    void                                set_piece_requested_impl        (lt::piece_index_t piece) const { pieces_requested_.insert(piece); }
    void                                set_piece_downloaded_impl       (lt::piece_index_t piece)       { pieces_downloaded_.insert(piece); }
    void                                insert_piece_data_impl          (const lt::read_piece_alert* rpa);

    bool                                clear_piece_requested_impl      (lt::piece_index_t piece) const { return pieces_requested_.erase(piece); }
    bool                                clear_piece_downloaded_impl     (lt::piece_index_t piece)       { return pieces_downloaded_.erase(piece); }
    bool                                clear_piece_in_cache_impl       (lt::piece_index_t piece)       { return cache_map_.erase(piece); }


    void                                prioritize_piece_impl           (lt::piece_index_t piece, int32_t deadline_in_ms) const  { torrent_handle_.set_piece_deadline(piece, deadline_in_ms, lt::torrent_handle::alert_when_available); }
    void                                read_piece_impl                 (lt::piece_index_t piece) const                          { torrent_handle_.read_piece(piece); }
    cache_pieces_t                      try_get_pieces_data_impl        (const pieces_range_t& range) const;

    void                                clear_current_streaming_range_impl ();

    using pieces_indices_set_t = std::unordered_set<lt::piece_index_t>;

    lt::torrent_handle                                                  torrent_handle_;
    mutable std::unordered_map<lt::piece_index_t, cache_piece_data_t>   cache_map_;
    pieces_indices_set_t                                                pieces_downloaded_;
    mutable pieces_indices_set_t                                        pieces_requested_;
    mutable std::mutex                                                  cache_mutex_;
    lt::piece_index_t                                                   cur_streaming_piece_ = 0;
    pieces_range_t                                                      cur_streaming_range_;

};

} // namespace cpaf::torrent

