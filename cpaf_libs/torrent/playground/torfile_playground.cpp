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


void dbg_read_from_file(std::shared_ptr<tor::torrent> torrent_ptr, lt::file_index_t file_index)
{
    std::array<char, 1024*16> text;
    text.fill('X');

    tor::file file = torrent_ptr->open(file_index);
    fmt::println("*******************************************");
    fmt::println("*** DEBUG read from file index : '{}' ***", (int)file_index);
    fmt::println("*******************************************");
    cout << "* file  name  : '" << file.name() << "'\n";
    cout << "* file  path  : '" << file.path() << "'\n";
    cout << "* file  size  : '" << file.size() << "'\n";
    cout << "* file  offset: '" << file.offset() << "'\n";
    fmt::println ("* file piece_index_start: '{}'", int(file.piece_index_start()));

    const auto bytes_read = file.read(text.data(), std::min(static_cast<size_t>(file.size()), text.size()));
    if (bytes_read > 0) {
        text[bytes_read] = 0;
        cerr << "  *** Blocking read of file '" << file.name() << "' content ***\n'" << text.data() << "'\n";
        cerr << "***********************************************************************\n";
    }
    cerr << std::flush;
}

#if 1
int main(int argc, char const* argv[])
{
    std::array<char, 1024*1024> text;
    text.fill('X');

    string magnet_url = "magnet:?xt=urn:btih:8567e2d162aba1bfd3b81cf88b257ce462eee761&dn=Rambo.First.Blood.1982.REMASTERED.1080p.BluRay.x265-RARBG&tr=http%3A%2F%2Ftracker.trackerfix.com%3A80%2Fannounce&tr=udp%3A%2F%2F9.rarbg.me%3A2890&tr=udp%3A%2F%2F9.rarbg.to%3A2890";
//    string magnet_url = "magnet:?xt=urn:btih:E4EC2A938F38AB62D4930436798F8ADD42F94ABE&dn=The%20Lost%20City%20(2022)%20%5B1080p%5D%20%5BBluRay%5D%20%5B5.1%5D&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337&tr=udp%3A%2F%2Fmovies.zsw.ca%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.dler.org%3A6969%2Fannounce&tr=udp%3A%2F%2Fopentracker.i2p.rocks%3A6969%2Fannounce&tr=udp%3A%2F%2Fopen.stealth.si%3A80%2Fannounce&tr=udp%3A%2F%2Ftracker.0x.tf%3A6969%2Fannounce";

    fmt::println ("--- torfile_playground  ---");
    if (argc >= 2) {
        magnet_url = argv[1];
    }

    fmt::println("torrent_name('{}'): '{}'", magnet_url, tor::torrent_name(magnet_url));


    tor::torrents my_torrents;
    my_torrents.base_torrents_path_set(base_torrents_path);
    my_torrents.debug_print_alerts_set(false);
    my_torrents.start();
    auto my_torrent_ptr = my_torrents.add_torrent(magnet_url);
    const auto initital_state = my_torrent_ptr->state();
    std::cerr << "initital_state: " << initital_state << "\n";



    std::cerr << "Waiting for meta data for '" << my_torrent_ptr->name() << "' ... ";
    while (!my_torrent_ptr->has_meta_data()) {
        //// my_torrents.frame_update();
//        std::cerr << "State: " << my_torrent_ptr->state() << "\n";
        this_thread::sleep_for(0ms);
    }
    std::cerr << " done!\n";
    my_torrent_ptr->prepare_streaming();

    for (auto index : my_torrent_ptr->all_file_indices()) {
        auto file = my_torrent_ptr->open(index);
        fmt::println ("[{} => {}] File in torrent: '{}'", (int)index, (int)file.piece_index_start(), file.name());
//        fmt::println ("[{} == {}] File in torrent: '{}'", (int)index, (int)my_torrent_ptr->file_path_to_index(file.path()), file.path());
//        fmt::println ("[{} == {}] File in torrent: '{}'", (int)index, (int)my_torrent_ptr->file_path_to_index(file.name()), file.name());
    }

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

    dbg_read_from_file(my_torrent_ptr, 1);


    tor::file file_0 = my_torrent_ptr->open(lt::file_index_t(0));
    fmt::println ("* file_0  piece_index_start: '{}'", int(file_0.piece_index_start()));
    tor::file largest_file = my_torrent_ptr->open_largest_file_streaming(10'000'000);
    fmt::println ("* largest_file  index            : {}", (int)largest_file.file_index());
    fmt::println ("* largest_file  name             : {}", largest_file.name());
    fmt::println ("* largest_file  path             : {}", largest_file.path());
    fmt::println ("* largest_file  size             : {:.4} Gb", largest_file.size()/1E9f);
    fmt::println ("* largest_file  offset           : {}", largest_file.offset());
    fmt::println ("* largest_file  piece_index_start: {}", int(largest_file.piece_index_start()));
//    cout << "* largest_file  piece_index_start  : '" << largest_file.piece_index_start() << "'\n";

    lt::peer_request pr1 = largest_file.file_offset_to_peer_request(0, 32);
    fmt::println ("* pr1 piece, start, len: {}, {}, {}", (int)pr1.piece, pr1.start, pr1.length);

    lt::peer_request pr0 = file_0.file_offset_to_peer_request(0, 10);
    fmt::println ("File 0 piece, start, len: {}, {}, {}", (int)pr0.piece, pr0.start, pr0.length);

    {
        tor::pieces_range_t range1  = file_0.get_pieces_range(0, 10);
        fmt::println ("File 0 begin: {}, end: {}, offset: {}, size: {}", (int)range1.piece_begin, (int)range1.piece_end, range1.piece_begin_data_start_offset(), range1.data_size);
    }
    {
        tor::file file = my_torrent_ptr->open(1);
        tor::pieces_range_t range  = file.get_pieces_range(0, 10);
        fmt::println ("File 1 begin: {}, end: {}, offset: {}, size: {}", (int)range.piece_begin, (int)range.piece_end, range.piece_begin_data_start_offset(), range.data_size);
    }

    {
        tor::pieces_range_t range1  = largest_file.get_pieces_range(0, 10);
        fmt::println ("get_pieces_range            begin: {}, end: {}, offset: {}, size: {}", (int)range1.piece_begin, (int)range1.piece_end, range1.piece_begin_data_start_offset(), range1.data_size);
    }
    {
        tor::pieces_range_t range1  = largest_file.get_pieces_range(2, 3'000'000);
        fmt::println ("get_pieces_range            begin: {}, end: {}, offset: {}, size: {}", (int)range1.piece_begin, (int)range1.piece_end, range1.piece_begin_data_start_offset(), range1.data_size);
    }
    {
        tor::pieces_range_t range1  = largest_file.get_pieces_read_ahead_range(2, 3'000'000);
        fmt::println ("get_pieces_read_ahead_range begin: {}, end: {}, offset: {}, size: {}", (int)range1.piece_begin, (int)range1.piece_end, range1.piece_begin_data_start_offset(), range1.data_size);
    }
    cerr << std::endl << std::flush; cout << std::endl << std::flush;

    for (int i  = 0;; ++i) {
        const auto bytes_read = largest_file.read(text.data(), std::min(16'000ul, text.size()));
    //        fmt::println("Bytes read from largest file: {}, current offset: {}", bytes_read, largest_file.offset());
        cerr << "Bytes read from largest file: " << bytes_read << ",  current offset: " << largest_file.offset() << "\n";
        this_thread::sleep_for(2s);
        if (my_torrent_ptr->is_fully_downloaded()) {
            break;
        }
    }

    my_torrents.stop();
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

