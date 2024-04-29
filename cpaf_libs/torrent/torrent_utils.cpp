#include "torrent_utils.h"

#include <algorithm>
#include <fmt/format.h>
#include <boost/algorithm/string.hpp>

#include <libtorrent/torrent_status.hpp>
#include <libtorrent/magnet_uri.hpp>

using namespace std;
using namespace std::filesystem;
namespace ba = boost::algorithm;

namespace cpaf::torrent {


bool is_magnet_uri (const std::string& uri_or_name)
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

    if (largest != -1) {
        const auto& storage = handle.torrent_file()->files();
        return string(storage.file_name(largest));
    }

    return "";

}

std::vector<string> all_file_names(const libtorrent::torrent_handle& handle)
{
    std::vector<string> names;

    auto torinfo_ptr = handle.torrent_file();
    if (!torinfo_ptr) {
        return names;
    }

    const auto& storage = torinfo_ptr->files();
    const auto iter_range = storage.file_range();

    for (auto file_index: iter_range) {
        names.push_back(string(storage.file_name(file_index)));
    }

    return names;
}

std::vector<string> all_file_paths(const libtorrent::torrent_handle& handle)
{
    std::vector<string> paths;

    auto torinfo_ptr = handle.torrent_file();
    if (!torinfo_ptr) {
        return paths;
    }

    const auto& storage = torinfo_ptr->files();
    const auto iter_range = storage.file_range();

    for (auto file_index: iter_range) {
        paths.push_back(string(storage.file_path(file_index)));
    }

    return paths;

}


std::filesystem::path largest_file_local_file_path(const libtorrent::torrent_handle& handle, const std::filesystem::path& base_torrents_path)
{
    const auto largest = largest_file_index(handle);

    if (largest != -1) {
        const auto& storage = handle.torrent_file()->files();
        return base_torrents_path / path(storage.file_path(largest));
    }

    return "";
}

libtorrent::file_index_t largest_file_index(const libtorrent::torrent_handle& handle)
{
    auto torinfo_ptr = handle.torrent_file();
    if (!torinfo_ptr) {
        return libtorrent::file_index_t(-1);
    }

    const auto& storage = torinfo_ptr->files();
    const auto iter_range = storage.file_range();
    auto largest_file_it = std::max_element(iter_range.begin(), iter_range.end(), [&storage](const auto& index_lhs, const auto& index_rhs)
    {
        return storage.file_size(index_lhs) < storage.file_size(index_rhs);
    });

    if (largest_file_it != iter_range.end()) {
        return *largest_file_it;
    }

    return libtorrent::file_index_t(-1);
}

int64_t file_size(const libtorrent::torrent_handle& handle, libtorrent::file_index_t file_index)
{
    auto torinfo_ptr = handle.torrent_file();
    if (!torinfo_ptr) {
        return -1;
    }

    const auto& storage = torinfo_ptr->files();
    return storage.file_size(file_index);
}


//std::optional<libtorrent::file_index_t> largest_file_index(const libtorrent::torrent_handle& handle)
//{
//    auto torinfo_ptr = handle.torrent_file();
//    if (!torinfo_ptr) {
//        return std::nullopt;
//    }

//    const auto& storage = torinfo_ptr->files();
//    const auto iter_range = storage.file_range();
//    auto largest_file_it = std::max_element(iter_range.begin(), iter_range.end(), [&storage](const auto& index_lhs, const auto& index_rhs)
//    {
//        return storage.file_size(index_lhs) < storage.file_size(index_rhs);
//    });

//    if (largest_file_it != iter_range.end()) {
//        return std::optional<libtorrent::file_index_t>(*largest_file_it);
//    }

//    return std::nullopt;
//}

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

void settings_set_default_bool(libtorrent::settings_pack& settings, int name, bool value)
{
    if (!settings.has_val(name)) {
        settings.set_bool(name, value);
    }
}

void settings_set_default_int(libtorrent::settings_pack& settings, int name, int value)
{
    if (!settings.has_val(name)) {
        settings.set_int(name, value);
    }
}

void settings_set_default_str(libtorrent::settings_pack& settings, int name, std::string value)
{
    if (!settings.has_val(name)) {
        settings.set_str(name, value);
    }
}

// ----------------------
// --- cache_pieces_t ---
// ----------------------

bool cache_pieces_t::is_valid() const
{
    if (pieces.empty()) { return false; }
    for (const auto& cache_piece_data : pieces) {
        if (!cache_piece_data.is_valid()) {
            return false;
        }
    }
    return true;
}

libtorrent::piece_index_t cache_pieces_t::last_piece_index() const
{
    if (pieces.empty()) { return -1; }
    return pieces.back().piece;
}

// ----------------------
// --- pieces_range_t ---
// ----------------------

string pieces_range_t::dbg_string() const
{
    return fmt::format("Piece begin;offset: {};{}, end: {}, file: {}, size: {:.4} Mb", (int)piece_begin, piece_begin_start_offset, (int)piece_end, (int)file_index, data_size/1e6 );
}


} // namespace cpaf::torrent
