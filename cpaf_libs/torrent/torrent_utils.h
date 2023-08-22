#pragma once

#include <filesystem>
#include <optional>
#include <libtorrent/torrent_handle.hpp>


namespace cpaf::torrent {

bool                                string_is_uri                   (const std::string& uri_or_name);
std::string                         torrent_name                    (const std::string& uri_or_name);
std::string                         largest_file_name               (const lt::torrent_handle& handle);
std::optional<lt::file_index_t>     largest_file_index              (const lt::torrent_handle& handle);
std::filesystem::path               largest_file_local_file_path    (const libtorrent::torrent_handle& handle, const std::filesystem::path& base_torrents_path);
bool                                has_meta_data                   (const libtorrent::torrent_handle& handle);
bool                                is_fully_downloaded             (const libtorrent::torrent_handle& handle);


} // namespace cpaf::torrent

