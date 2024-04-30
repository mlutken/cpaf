#pragma once

#include <filesystem>
#include <optional>
#include <vector>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/alert_types.hpp>


namespace cpaf::torrent {

bool                                is_magnet_uri                   (const std::string& uri_or_name);
std::string                         torrent_name                    (const std::string& uri_or_name);
std::string                         largest_file_name               (const lt::torrent_handle& handle);
lt::file_index_t                    largest_file_index              (const lt::torrent_handle& handle);
std::int64_t                        file_size                       (const lt::torrent_handle& handle, lt::file_index_t file_index);
std::vector<std::string>            all_file_names                  (const libtorrent::torrent_handle& handle);
std::vector<std::string>            all_file_paths                  (const libtorrent::torrent_handle& handle);
std::filesystem::path               base_local_file_dir             (const libtorrent::torrent_handle& handle);
std::filesystem::path               largest_file_local_file_path    (const libtorrent::torrent_handle& handle);
std::filesystem::path               torrent_local_file_path         (const libtorrent::torrent_handle& handle);
bool                                has_meta_data                   (const libtorrent::torrent_handle& handle);
bool                                is_fully_downloaded             (const libtorrent::torrent_handle& handle);

void                                settings_set_default_bool       (libtorrent::settings_pack& settings, int name, bool value);
void                                settings_set_default_int        (libtorrent::settings_pack& settings, int name, int value);
void                                settings_set_default_str        (libtorrent::settings_pack& settings, int name, std::string value);

enum class cancel_io_state_t : uint8_t {
    not_requested,
    requested,
    completed
};

struct cache_piece_data_t {
    cache_piece_data_t                          ()                      = default;
    explicit cache_piece_data_t                 (const lt::read_piece_alert& rpa) : buffer(rpa.buffer), piece(rpa.piece), data_size(rpa.size) {}
    explicit cache_piece_data_t                 (const lt::read_piece_alert* rpa) : buffer(rpa->buffer), piece(rpa->piece), data_size(rpa->size) {}
    cache_piece_data_t                          (const cache_piece_data_t& other)  = default;

    cache_piece_data_t&       operator=         (const cache_piece_data_t& other) = default;
    bool                    is_valid            () const { return data_size != -1 && (buffer != nullptr); }
    size_t                  bytes_left          () const { return data_size < 0 ? 0 : static_cast<size_t>(data_size - data_start_offset); }
    char*                   buffer_begin        () const { return &buffer[data_start_offset]; }

    boost::shared_array<char>           buffer;
    lt::piece_index_t                   piece;
    int64_t                             data_size = -1;
    int64_t                             data_start_offset = 0;
};

struct cache_pieces_t {
    std::vector<cache_piece_data_t>     pieces;
    size_t                              data_size                   = 0;
    bool                                is_valid                        () const;
    lt::piece_index_t                   last_piece_index                () const;
};

struct pieces_range_t {

                pieces_range_t  () = default;
    explicit    pieces_range_t  (lt::piece_index_t begin) : piece_begin(begin), piece_end(begin + lt::piece_index_t(1)) {}
                pieces_range_t  (lt::piece_index_t begin, lt::piece_index_t end) : piece_begin(begin), piece_end(end) {}

    lt::piece_index_t                   piece_begin                 = -1;
    lt::piece_index_t                   piece_end                   = -1;

    int64_t                             piece_begin_start_offset    = -1;
    size_t                              data_size                   = 0;
    lt::file_index_t                    file_index                  = -1;

    int64_t                             piece_begin_data_start_offset   () const    { return piece_begin_start_offset; }
    bool                                is_valid                        () const    { return piece_begin != -1; }
    std::string                         dbg_string                      () const;
};

} // namespace cpaf::torrent

