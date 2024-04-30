#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <optional>
#include <chrono>
#include <libtorrent/torrent_handle.hpp>

#include <cpaf_libs/torrent/file.h>
#include <cpaf_libs/torrent/streaming_cache.h>


namespace cpaf::torrent {

class torrents;

class torrent
{
    friend class torrents;
private:
    /// Use torrents::open() to create instances of torrent!
    explicit                            torrent                         (const std::string& uri_or_name, lt::torrent_handle handle, torrents* parent_torrents_ptr);
public:
    torrent    ()                       = delete;
    torrent    (const torrent& other)   = delete;

    torrent&                            operator=                       (const torrent& other) = delete;

    file                                open                            (lt::file_index_t file_index);
    file                                open                            (std::string_view file_path);
    file                                open_streaming                  (lt::file_index_t file_index, size_t read_ahead_size);
    file                                open_streaming                  (std::string_view file_path, size_t read_ahead_size);
    file                                open_largest_file_streaming     (size_t read_ahead_size);
    bool                                wait_for_meta_data              (std::chrono::milliseconds timeout = std::chrono::minutes(1));
    void                                cancel_current_io_operation     ();
    bool                                cancel_io_completed             () const;
    void                                remove                          ();
    void                                pause                           ();

    const std::string&                  uri                             () const                                    { return uri_; }
    std::string                         name                            () const;
    std::size_t                         hash_value                      () const                                    { return lt::hash_value(handle_); }
    lt::torrent_status::state_t         state                           () const                                    { return handle_.status().state;   }


    std::string                         largest_file_name               () const;
    lt::file_index_t                    largest_file_index              () const;
    std::int64_t                        file_size                       (lt::file_index_t file_index) const;
    std::int64_t                        largest_file_size               () const                                    { return file_size(largest_file_index()); };
    std::filesystem::path               base_local_file_dir             () const;
    std::filesystem::path               largest_file_local_file_path    () const;
    std::filesystem::path               local_file_path                 () const;
    lt::index_range<lt::file_index_t>   all_file_indices                () const;
    std::vector<std::string>            all_file_names                  () const;
    std::vector<std::string>            all_file_paths                  () const;
    int                                 number_of_files                 () const;
    lt::file_index_t                    file_path_to_index              (std::string_view file_path) const;

    lt::file_index_t                    file_index_at_offset            (std::int64_t offset) const;
    lt::file_index_t                    file_index_at_piece             (lt::piece_index_t piece) const;

    std::vector<lt::file_slice>         map_block                       (lt::piece_index_t piece, std::int64_t offset, std::int64_t size) const;


    int                                 piece_length                    () const;
    int                                 num_pieces                      () const;
    lt::piece_index_t                   piece_index_begin               () const    { return 0; }
    lt::piece_index_t                   piece_index_end                 () const;
    bool                                piece_index_valid               (lt::piece_index_t piece) const    { return (piece_index_begin() <= piece) && (piece < piece_index_end()); }
    void                                ensure_piece_range_valid        (pieces_range_t& range) const;

    bool                                is_valid                        () const                                    { return parent_torrents_ptr_ != nullptr; }
    bool                                has_meta_data                   () const;
    bool                                is_fully_downloaded             () const;
    bool                                prepare_streaming               ();
    void                                request_pieces                  (const pieces_range_t& range, int32_t deadline_in_ms = 0) const;


    lt::piece_index_t                   file_offset_to_piece_index      (lt::file_index_t file_index, std::int64_t offset) const;
    lt::peer_request                    file_offset_to_peer_request     (lt::file_index_t file_index, std::int64_t offset, size_t size) const;
    cache_pieces_t                      get_pieces_data                 (lt::file_index_t file_index, int64_t offset, size_t size, std::chrono::milliseconds timeout = std::chrono::minutes(2)) const;


    pieces_range_t                      get_pieces_range                (lt::file_index_t file_index, std::int64_t offset, size_t size) const;
    pieces_range_t                      get_pieces_read_ahead_range     (lt::file_index_t file_index, lt::piece_index_t from_piece, size_t read_ahead_size) const;
    pieces_range_t                      get_pieces_read_ahead_range     (lt::file_index_t file_index, std::int64_t offset, size_t read_ahead_size) const;
    bool                                are_pieces_in_cache             (const pieces_range_t& range) const;

    void                                handle_piece_finished           (const lt::piece_finished_alert* pfa);
    void                                handle_piece_read               (const lt::read_piece_alert* rpa);
    void                                handle_torrent_finished         (const lt::torrent_finished_alert* tfa);
    void                                handle_torrent_error            (const lt::torrent_error_alert* tea);

    void                                dbg_print_downloaded_indices    () const;
    void                                dbg_print_cache_piece_indices   () const;


private:
    using pieces_downloaded_set_t = std::unordered_set<lt::piece_index_t>;
    const lt::file_storage&             files_storage                   () const;

    lt::torrent_handle                      handle_                     {};
    streaming_cache                         piece_data_cache_;
    std::string                             uri_                        {};
    std::string                             name_                       {};
    torrents*                               parent_torrents_ptr_        = nullptr;
    size_t                                  default_read_ahead_size_    = 10'000'000; // 10 Mb
    mutable std::atomic<cancel_io_state_t>  cancel_io_state_            {cancel_io_state_t::not_requested};
    std::chrono::microseconds               io_yield_time_              = std::chrono::milliseconds(10);
};

} // namespace cpaf::torrent

