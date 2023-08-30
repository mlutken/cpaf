#pragma once

#include <filesystem>
#include <optional>
#include <vector>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/alert_types.hpp>


namespace cpaf::torrent {

bool                                string_is_uri                   (const std::string& uri_or_name);
std::string                         torrent_name                    (const std::string& uri_or_name);
std::string                         largest_file_name               (const lt::torrent_handle& handle);
//std::optional<lt::file_index_t>     largest_file_index              (const lt::torrent_handle& handle);
lt::file_index_t                    largest_file_index              (const lt::torrent_handle& handle);
std::vector<std::string>            all_file_names                  (const libtorrent::torrent_handle& handle);
std::vector<std::string>            all_file_paths                  (const libtorrent::torrent_handle& handle);
std::filesystem::path               largest_file_local_file_path    (const libtorrent::torrent_handle& handle, const std::filesystem::path& base_torrents_path);
bool                                has_meta_data                   (const libtorrent::torrent_handle& handle);
bool                                is_fully_downloaded             (const libtorrent::torrent_handle& handle);


struct cache_piece_data {
    cache_piece_data                          ()                      = default;
    explicit cache_piece_data                 (const lt::read_piece_alert& rpa) : buffer(rpa.buffer), piece(rpa.piece), size(rpa.size) {}
    explicit cache_piece_data                 (const lt::read_piece_alert* rpa) : buffer(rpa->buffer), piece(rpa->piece), size(rpa->size) {}
    cache_piece_data                          (const cache_piece_data& other)  = default;

    cache_piece_data&       operator=         (const cache_piece_data& other) = default;
    bool                    is_valid          () const { return size != -1; }


    boost::shared_array<char>           buffer;
    lt::piece_index_t                   piece;
    int64_t                             size = -1;
};


} // namespace cpaf::torrent

