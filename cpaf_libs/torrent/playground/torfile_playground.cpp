#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>

#include <fmt/format.h>

#include <cpaf_libs/torrent/torrents.h>
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
 * Magnet link example 3: "magnet:?xt=urn:btih:E4EC2A938F38AB62D4930436798F8ADD42F94ABE&dn=The%20Lost%20City%20(2022)%20%5B1080p%5D%20%5BBluRay%5D%20%5B5.1%5D&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337&tr=udp%3A%2F%2Fmovies.zsw.ca%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.dler.org%3A6969%2Fannounce&tr=udp%3A%2F%2Fopentracker.i2p.rocks%3A6969%2Fannounce&tr=udp%3A%2F%2Fopen.stealth.si%3A80%2Fannounce&tr=udp%3A%2F%2Ftracker.0x.tf%3A6969%2Fannounce"
 * Magnet link example 4: "magnet:?xt=urn:btih:EF947A280CE7AD1BC6752848F204A9F111F5C7D3&dn=The%20Lost%20City%20(2022)%20%5B1080p%5D%20%5BWEBRip%5D%20%5B5.1%5D&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337&tr=udp%3A%2F%2Fmovies.zsw.ca%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.dler.org%3A6969%2Fannounce&tr=udp%3A%2F%2Fopentracker.i2p.rocks%3A6969%2Fannounce&tr=udp%3A%2F%2Fopen.stealth.si%3A80%2Fannounce&tr=udp%3A%2F%2Ftracker.0x.tf%3A6969%2Fannounce"
 * Legal torrent sources: https://www.techworm.net/2020/07/best-legal-torrenting-sites.html
 *                        https://www.vuze.com/content/
 *
 * */


using namespace std;
using namespace std::filesystem;
using namespace std::chrono;
using namespace std::chrono_literals;
namespace tor = cpaf::torrent;

// /home/ml/.conan2/p/b/libto704fbe9751615/b/src/docs/streaming.rst
// /home/ml/.conan2/p/b/libto704fbe9751615/p/include/libtorrent/torrent.hpp::m_storage
// /home/ml/.conan2/p/b/libto704fbe9751615/p/include/libtorrent/disk_interface.hpp
// /home/ml/.conan2/p/b/libto704fbe9751615/p/include/libtorrent/torrent_info.hpp
// /home/ml/.conan2/p/b/libto704fbe9751615/p/include/libtorrent/file_storage.hpp
// /home/ml/.conan2/p/b/libto704fbe9751615/b/src/src/session.cpp
// /home/ml/.conan2/p/b/libto704fbe9751615/p/include/libtorrent/storage_defs.hpp
// /home/ml/.conan2/p/b/libto704fbe9751615/p/include/libtorrent/aux_/file_view_pool.hpp
// /home/ml/.conan2/p/b/libto704fbe9751615/b/src/test/test_read_piece.cpp
//
//


// https://www.libtorrent.org/reference-Custom_Storage.html#overview
// void lt::torrent_handle::set_piece_deadline(piece_index_t index, int deadline, deadline_flags_t flags = {}) const;
// void lt::torrent_handle::reset_piece_deadline(piece_index_t index) const;
//  Gnutella is a decentralized Peer to Peer information exchanging network: https://gtk-gnutella.sourceforge.io/?lang=en&page=faq#general0

// ".8567e2d162aba1bfd3b81cf88b257ce462eee761.parts"
// struct TORRENT_EXPORT read_piece_alert final : torrent_alert

// Notes: read_piece()

static const path base_torrents_path = "/tmp/torrents";

//fmt::println("usage: {} <magnet-url>", argv[0]);
//return 1;

#if 1
int main(int argc, char const* argv[])
{

//    string magnet_url = "magnet:?xt=urn:btih:8567e2d162aba1bfd3b81cf88b257ce462eee761&dn=Rambo.First.Blood.1982.REMASTERED.1080p.BluRay.x265-RARBG&tr=http%3A%2F%2Ftracker.trackerfix.com%3A80%2Fannounce&tr=udp%3A%2F%2F9.rarbg.me%3A2890&tr=udp%3A%2F%2F9.rarbg.to%3A2890";
    string magnet_url = "magnet:?xt=urn:btih:E4EC2A938F38AB62D4930436798F8ADD42F94ABE&dn=The%20Lost%20City%20(2022)%20%5B1080p%5D%20%5BBluRay%5D%20%5B5.1%5D&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337&tr=udp%3A%2F%2Fmovies.zsw.ca%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.dler.org%3A6969%2Fannounce&tr=udp%3A%2F%2Fopentracker.i2p.rocks%3A6969%2Fannounce&tr=udp%3A%2F%2Fopen.stealth.si%3A80%2Fannounce&tr=udp%3A%2F%2Ftracker.0x.tf%3A6969%2Fannounce";

    fmt::println ("--- torfile_playground  ---");
    if (argc >= 2) {
        magnet_url = argv[1];
    }




    fmt::println("torrent_name('gert'): '{}'", tor::torrent_name("gert"));
    fmt::println("torrent_name('{}'): '{}'", magnet_url, tor::torrent_name(magnet_url));


    tor::torrents my_torrents;
    my_torrents.base_torrents_path_set(base_torrents_path);
    my_torrents.debug_print_alerts_set(false);
    my_torrents.start();
    auto my_torrent_ptr = my_torrents.add_torrent(magnet_url);

    std::cerr << "Waiting for meta data for '" << my_torrent_ptr->name() << "' ... ";
    while (!my_torrent_ptr->has_meta_data()) {
        my_torrents.frame_update();
        this_thread::sleep_for(20ms);
    }
    std::cerr << " done!\n";
    my_torrent_ptr->prepare_streaming();

    for (const auto& file_name : my_torrent_ptr->all_file_names()) {
        cerr << "File in torrent: '" << file_name << "'\n";
    }


    cout << "XXXXXXXXXXXXXXXXXXXXXX\n";
    cout << "XXXXXXXXXXXXXXXXXXXXXX\n";
    cout << "XXXXXXXXXXXXXXXXXXXXXX\n";
    cout << "Largest file index       : '" << my_torrent_ptr->largest_file_index() << "'\n";
    cout << "Largest file             : '" << my_torrent_ptr->largest_file_local_file_path() << "'\n";
    cout << "Number of pieces         : " << my_torrent_ptr->num_pieces() << "\n";
    cout << "Piece len                : " << my_torrent_ptr->piece_length() << "\n";
    cout << "file_index_at_offset(2)  : " << my_torrent_ptr->file_index_at_offset(2) << "\n";
    cout << "file_index_at_offset(30) : " << my_torrent_ptr->file_index_at_offset(30) << "\n";
    cout << "file_index_at_offset(128): " << my_torrent_ptr->file_index_at_offset(128) << "\n";
    cout << "file_index_at_offset(129): " << my_torrent_ptr->file_index_at_offset(129) << "\n";
    cout << "file_index_at_offset(130): " << my_torrent_ptr->file_index_at_offset(130) << "\n";
    cerr << std::flush; cout << std::flush;

//    my_torrent_ptr->read_all_downloaded_pieces();
//    my_torrent_ptr->read_pieces(tor::pieces_range_t(0, 2));


    tor::file file_0 = my_torrent_ptr->open(0);
    cout << "* file_0  name  : '" << file_0.name() << "'\n";
    cout << "* file_0  path  : '" << file_0.path() << "'\n";
    cout << "* file_0  size  : '" << file_0.size() << "'\n";
    cout << "* file_0  offset: '" << file_0.offset() << "'\n";
    fmt::println ("* file_0  piece_index_start: '{}'", int(file_0.piece_index_start()));

    tor::file file_1 = my_torrent_ptr->open(1);
//    cout << "* file_1  name  : '" << file_1.name() << "'\n";
//    cout << "* file_1  path  : '" << file_1.path() << "'\n";
//    cout << "* file_1  size  : '" << file_1.size() << "'\n";
//    cout << "* file_1  offset: '" << file_1.offset() << "'\n";
//    fmt::println ("* file_1  piece_index_start: '{}'", int(file_1.piece_index_start()));

//    tor::file file_3 = my_torrent_ptr->open(3);
//    cout << "* file_3  name  : '" << file_3.name() << "'\n";
//    cout << "* file_3  path  : '" << file_3.path() << "'\n";
//    cout << "* file_3  size  : '" << file_3.size() << "'\n";
//    cout << "* file_3  offset: '" << file_3.offset() << "'\n";
//    fmt::println ("* file_3  piece_index_start: '{}'", int(file_3.piece_index_start()));

    tor::file largest_file = my_torrent_ptr->open(my_torrent_ptr->largest_file_index());
//    cout << "* largest_file  name                   : '" << largest_file.name() << "'\n";
//    cout << "* largest_file  path                   : '" << largest_file.path() << "'\n";
//    cout << "* largest_file  size                   : '" << largest_file.size() << "'\n";
//    fmt::println ("* largest_file  offset           : '{}'", largest_file.offset());
//    fmt::println ("* largest_file  piece_index_start: '{}'", int(largest_file.piece_index_start()));
//    cout << "* largest_file  piece_index_start  : '" << largest_file.piece_index_start() << "'\n";

    lt::peer_request pr1 = largest_file.file_offset_to_peer_request(0, 32);
    fmt::println ("* pr1 piece, start, len: {}, {}, {}", (int)pr1.piece, pr1.start, pr1.length);

    lt::peer_request pr0 = file_0.file_offset_to_peer_request(0, 10);
    fmt::println ("File 0 piece, start, len: {}, {}, {}", (int)pr0.piece, pr0.start, pr0.length);

    {
        tor::pieces_range_t range1  = file_0.get_pieces_range(0, 10);
        fmt::println ("File 0 begin: {}, end: {}, offset: {}, size: {}", (int)range1.piece_begin, (int)range1.piece_end, range1.piece_begin_start_offset, range1.data_size);
    }
//    {
//        tor::pieces_range_t range1  = file_3.get_pieces_range(0, 10);
//        fmt::println ("File 3 begin: {}, end: {}, offset: {}, size: {}", (int)range1.piece_begin, (int)range1.piece_end, range1.piece_begin_start_offset, range1.data_size);
//    }

    {
        tor::pieces_range_t range1  = largest_file.get_pieces_range(0, 10);
        fmt::println ("largest_file begin: {}, end: {}, offset: {}, size: {}", (int)range1.piece_begin, (int)range1.piece_end, range1.piece_begin_start_offset, range1.data_size);
    }
    {
        tor::pieces_range_t range1  = largest_file.get_pieces_range(0, 3'000'000);
        fmt::println ("largest_file begin: {}, end: {}, offset: {}, size: {}", (int)range1.piece_begin, (int)range1.piece_end, range1.piece_begin_start_offset, range1.data_size);
    }
    {
        tor::pieces_range_t range1  = largest_file.get_pieces_read_ahead_range(2, 6'000'000);
        fmt::println ("largest_file begin: {}, end: {}, offset: {}, size: {}", (int)range1.piece_begin, (int)range1.piece_end, range1.piece_begin_start_offset, range1.data_size);
    }
    cerr << std::flush; cout << std::flush;

//    this_thread::sleep_for(10s);
//    return 0; // FIXMENM


    for (int i  = 0;; ++i) {
        my_torrents.frame_update();
        this_thread::sleep_for(20ms);
        if (my_torrent_ptr->is_fully_downloaded()) {
            break;
        }

        if (i % 100 == 0) {
            cerr << "* file_0  name  : '" << file_0.name() << "'  i: " << i << "\n";
            std::array<char, 1024> text;
            text.fill('X');

            const auto bytes_read = file_0.read(text.data(), std::min((size_t)file_0.size(), text.size()));
            if (bytes_read > 0) {
                text[bytes_read] = 0;
                cerr << " >>>> file_0  content: '" << text.data() << "'\n";
            }


            cerr << std::flush; cout << std::flush;
        }
//        my_torrent_ptr->dbg_print_downloaded_indices();
//        my_torrent_ptr->dbg_print_cache_piece_indices();

    }

    fmt::println ("Torrent: {} downloaded!", my_torrent_ptr->name());
    cerr << std::flush; cout << std::flush;
    my_torrent_ptr->dbg_print_downloaded_indices();
    my_torrent_ptr->dbg_print_cache_piece_indices();

    return 0;
}




#else

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
                    std::cerr << "ALERT: " << a->message() << "\n";
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

#endif

