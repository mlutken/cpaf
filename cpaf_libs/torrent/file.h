#pragma once

#include <vector>
#include <chrono>
#include <libtorrent/torrent_handle.hpp>
#include <cpaf_libs/torrent/torrent_utils.h>

namespace cpaf::torrent {

class torrent;


class file
{
    friend class torrent;
private:
    /// Use torrent::open() to create instances of (torrent) file(s)!
    explicit                            file                (lt::file_index_t file_index, lt::torrent_handle handle, torrent* parent_torrent_ptr);
public:
    file        ()                  = default;
    file        (const file& other) = default;

    file&                               operator=                       (const file& other) = default;

    int64_t                             read                            (void* buffer, std::size_t bytes_to_read, std::chrono::milliseconds timeout = std::chrono::minutes(2));
    int                                 seek                            (int64_t offset, int whence);
    size_t                              read_ahead_size                 () const                        { return read_ahead_size_; }
    void                                read_ahead_size_set             (size_t read_ahead_size)        { read_ahead_size_ = read_ahead_size; }
    void                                request_pieces_from_offset      ();
    void                                remove_parent_torrent           ();
    void                                pause_parent_torrent            ();
    void                                close                           ();
    void                                meta_data_progress_callback_set (progress_callback_fn cb);
    void                                data_progress_callback_set      (progress_callback_fn cb);
    void                                cancel_current_io_operation     ();
    bool                                cancel_io_completed             () const;

    void                                use_blocking_seek_set           (bool bs) { use_blocking_seek_ = bs; }
    bool                                use_blocking_seek               () const { return use_blocking_seek_; }
    bool                                is_valid                        () const { return parent_torrent_ptr_ && file_index_ >= lt::file_index_t{0}; }
    bool                                are_streaming_pieces_in_cache   () const;
    bool                                streaming_mode_active           () const                        { return read_ahead_size_ > 0; }
    int64_t                             size                            () const;
    int64_t                             offset                          () const                        { return offsett_; }
    lt::file_index_t                    file_index                      () const { return file_index_; }

    bool                                is_open                         () const;
    std::string                         name                            () const;
    std::string                         path                            () const;

    lt::piece_index_t                   piece_index_start               () const;
    lt::peer_request                    file_offset_to_peer_request     (std::int64_t offset, int size) const;

    cache_pieces_t                      get_pieces_data                 (std::int64_t offset, size_t size, std::chrono::milliseconds timeout = std::chrono::minutes(2)) const;
    pieces_range_t                      get_pieces_range                (int64_t offset, size_t size) const;
    pieces_range_t                      get_pieces_read_ahead_range     (lt::piece_index_t from_piece, size_t read_ahead_size) const;
    pieces_range_t                      get_pieces_read_ahead_range     (lt::piece_index_t from_piece) const;
    pieces_range_t                      get_pieces_read_ahead_range     (int64_t offset) const;

private:
    const lt::file_storage&             files_storage                   () const;

    lt::file_index_t                    file_index_             = lt::file_index_t{-1};
    lt::torrent_handle                  handle_;
    torrent*                            parent_torrent_ptr_     = nullptr;
    int64_t                             offsett_                = 0;
    size_t                              read_ahead_size_        = 0;
    std::chrono::milliseconds           blocking_seek_timeout_  = std::chrono::minutes(2);
    bool                                use_blocking_seek_      = true;

//    std::string                         name_;

};

} // namespace capf::torrent

