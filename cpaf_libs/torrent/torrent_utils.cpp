#include "torrent_utils.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>

#include <libtorrent/torrent_status.hpp>
#include <libtorrent/magnet_uri.hpp>

using namespace std;
using namespace std::filesystem;
namespace ba = boost::algorithm;

namespace cpaf::torrent {


bool string_is_uri (const std::string& uri_or_name)
{
    return ba::istarts_with(uri_or_name, "magnet:?");
}

string torrent_name(const std::string& uri_or_name)
{
    lt::error_code ec;
    lt::add_torrent_params torrent_params;

    lt::parse_magnet_uri(uri_or_name, torrent_params, ec);
    if (ec.failed()) {
        return uri_or_name;
    }

    return torrent_params.name;
}

std::string largest_file_name(const libtorrent::torrent_handle& handle)
{
    const auto largest = largest_file_index(handle);

    if (largest) {
        const auto& storage = handle.torrent_file()->files();
        return string(storage.file_name(*largest));
    }

    return "";

}

std::filesystem::path largest_file_local_file_path(const libtorrent::torrent_handle& handle, const std::filesystem::path& base_torrents_path)
{
    const auto largest = largest_file_index(handle);

    if (largest) {
        const auto& storage = handle.torrent_file()->files();
        return base_torrents_path / path(storage.file_path(*largest));
    }

    return "";
}

std::optional<libtorrent::file_index_t> largest_file_index(const libtorrent::torrent_handle& handle)
{
    auto torinfo_ptr = handle.torrent_file();
    if (!torinfo_ptr) {
        return std::nullopt;
    }

    const auto& storage = torinfo_ptr->files();
    const auto iter_range = storage.file_range();
    auto largest_file_it = std::max_element(iter_range.begin(), iter_range.end(), [&storage](const auto& index_lhs, const auto& index_rhs)
    {
        return storage.file_size(index_lhs) < storage.file_size(index_rhs);
    });

    if (largest_file_it != iter_range.end()) {
        return std::optional<libtorrent::file_index_t>(*largest_file_it);
    }

    return std::nullopt;
}

bool has_meta_data(const libtorrent::torrent_handle& handle)
{
    switch (handle.status().state) {
        case lt::torrent_status::state_t::queued_for_checking:
        case lt::torrent_status::state_t::checking_files:
        case lt::torrent_status::state_t::downloading_metadata:
            return false;
        break;
    default:
        return true;
    }
}

bool is_fully_downloaded(const libtorrent::torrent_handle& handle)
{
    switch (handle.status().state) {
        case lt::torrent_status::state_t::finished:
        case lt::torrent_status::state_t::seeding:
            return true;
        break;
    default:
        return false;
    }
}


} // namespace cpaf::torrent
