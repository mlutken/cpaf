#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>

#include <fmt/format.h>

#include <cpaf_libs/torrent/files.h>
#include <cpaf_libs/torrent/torrent_utils.h>
#include <cpaf_libs/torrent/temp_storage.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"

#include <libtorrent/session.hpp>
#include <libtorrent/session_params.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/disk_interface.hpp>
#include <libtorrent/posix_disk_io.hpp>

#pragma GCC diagnostic pop

/**
 * NASA torrents: https://www.vuze.com/content/channel.php?id=48&name=NASACast%20Video
 * Magnet link example 1: "magnet:?xt=urn:btih:a2aeda9a399ab8dbbf1edb46ba74841256567f6f&dn=This%5FWeek%5F%40NASA,%5FJuly%5F27,%5F2018%5BV004850366%5D.mp4&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A80%2Fannounce&tr=udp%3A%2F%2Ftracker.publicbt.com%3A80%2Fannounce&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969&tr=udp%3A%2F%2Fopen.demonii.com%3A1337&ws=http%3A%2F%2Fwww.nasa.gov%2Fsites%2Fdefault%2Ffiles%2Fatoms%2Fvideo%2Fnhq%5F20180727%5Fthis%5Fweek%5Fnasa%5F1.mp4"
 * Magnet link example 2: "magnet:?xt=urn:btih:8567e2d162aba1bfd3b81cf88b257ce462eee761&dn=Rambo.First.Blood.1982.REMASTERED.1080p.BluRay.x265-RARBG&tr=http%3A%2F%2Ftracker.trackerfix.com%3A80%2Fannounce&tr=udp%3A%2F%2F9.rarbg.me%3A2890&tr=udp%3A%2F%2F9.rarbg.to%3A2890"
 * Legal torrent sources: https://www.techworm.net/2020/07/best-legal-torrenting-sites.html
 *                        https://www.vuze.com/content/
 *
 * */


using namespace std;
using namespace std::filesystem;
namespace tor = cpaf::torrent;

// /home/ml/.conan2/p/b/libto704fbe9751615/b/src/docs/streaming.rst
// /home/ml/.conan2/p/b/libto704fbe9751615/p/include/libtorrent/torrent.hpp::m_storage
// /home/ml/.conan2/p/b/libto704fbe9751615/p/include/libtorrent/disk_interface.hpp
// https://www.libtorrent.org/reference-Custom_Storage.html#overview
// void lt::torrent_handle::set_piece_deadline(piece_index_t index, int deadline, deadline_flags_t flags = {}) const;
// void lt::torrent_handle::reset_piece_deadline(piece_index_t index) const;
//  Gnutella is a decentralized Peer to Peer information exchanging network: https://gtk-gnutella.sourceforge.io/?lang=en&page=faq#general0

int main(int argc, char const* argv[])
{

    const path base_torrents_path = "/tmp/torrents";

    fmt::println ("--- torfile_playground  ---");
    if (argc != 2) {
        fmt::println("usage: {} <magnet-url>", argv[0]);
        return 1;
    }



    const string magnet_url = argv[1];

    fmt::println("torrent_name('gert'): '{}'", tor::torrent_name("gert"));
    fmt::println("torrent_name('{}'): '{}'", magnet_url, tor::torrent_name(magnet_url));


    tor::files torrent_files;
    tor::file tor_file = torrent_files.open(magnet_url);

    lt::settings_pack settings_pack;
    lt::add_torrent_params torrent_params;
    lt::error_code ec;

    settings_pack.set_int(lt::settings_pack::alert_mask, lt::alert_category::status | lt::alert_category::error | lt::alert_category::piece_progress);
    lt::session_params ses_params(settings_pack);
//    ses_params.disk_io_constructor = lt::posix_disk_io_constructor;
    ses_params.disk_io_constructor = tor::temp_disk_constructor;
//    lt::session ses(settings_pack);
    lt::session ses(ses_params);

    lt::parse_magnet_uri(argv[1], torrent_params, ec);
    std::filesystem::create_directories(base_torrents_path);
    torrent_params.save_path = base_torrents_path;
    lt::torrent_handle handle = ses.add_torrent(std::move(torrent_params));


//    while (!handle.has_metadata()) {
//        cerr << "handle.status().state: " << handle.status().state << "\n";
//        std::this_thread::sleep_for(std::chrono::milliseconds(200));
//    }

    while ( !tor::has_meta_data(handle) ) {
        cerr << "XX handle.status().state: " << handle.status().state << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }


    auto torinfo = handle.torrent_file();

    cerr << "Largest file       : '" << tor::largest_file_local_file_path(handle, base_torrents_path) << "'\n";
    cerr << "Number of pieces   : " << torinfo->num_pieces() << "\n";
    cerr << "Piece len          : " << torinfo->piece_length() << "\n";
    cerr << "Largest file: '" << tor::largest_file_local_file_path(handle, base_torrents_path) << "'\n";

//    int piece_length() const { return m_files.piece_length(); }
//    int num_pieces() const { return m_files.num_pieces(); }

    for (lt::aux::file_entry p : torinfo->files()) {
        cerr << "File in torrent: '" << p.filename() << "'\n";
    }

    for (;;) {
        std::vector<lt::alert*> alerts;
        ses.pop_alerts(&alerts);

        for (lt::alert const* a : alerts) {
            if (auto pfinished_alert = lt::alert_cast<lt::piece_finished_alert>(a)) {
//                if (pfinished_alert->piece_index < 30) {
                    std::cerr << a->message() << "\n";
                    if (pfinished_alert->piece_index < 30) {
                        std::cerr << "pfinished_alert->piece_index: " << pfinished_alert->piece_index << "\n";
                    }
            }
            // if we receive the finished alert or an error, we're done
            if (lt::alert_cast<lt::torrent_finished_alert>(a)) {
                goto done;
            }
            if (lt::alert_cast<lt::torrent_error_alert>(a)) {
                goto done;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
done:
    std::cerr << "done, shutting down" << std::endl;

    return 0;
}


