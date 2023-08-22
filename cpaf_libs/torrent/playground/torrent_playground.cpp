// https://webtorrentplayer.github.io/
// https://www.libtorrent.org/tutorial-ref.html
// http://openbittorrent.com/
// https://cooltechzone.com/torrent-sites/torrent-search-engine
// https://cooltechzone.com/torrent-sites/torrent-search-engine#More%20torrent%20search%20engines%20(+10)
// https://snowfl.com/
// https://www.linuxhelp.com/how-to-use-popcorn-time-in-linux-desktop
// WARNINGS DISABLE:
// https://stackoverflow.com/questions/6321839/how-to-disable-warnings-for-particular-include-files

// torrent_flags.hpp
// /home/ml/.conan2/p/b/libto704fbe9751615/p/include/libtorrent/torrent_handle.hpp::set_piece_deadline()
//

#include <iostream>
#include <thread>
#include <chrono>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"

#include <libtorrent/session.hpp>
//#include <libtorrent/session_params.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/magnet_uri.hpp>

#pragma GCC diagnostic pop

#include <filesystem>

int main3()
{
    std::cerr << "------------------------\n";
    return 0;
}

#if 1

/**
 * NASA torrents: https://www.vuze.com/content/channel.php?id=48&name=NASACast%20Video
 * Magnet link example 1: "magnet:?xt=urn:btih:a2aeda9a399ab8dbbf1edb46ba74841256567f6f&dn=This%5FWeek%5F%40NASA,%5FJuly%5F27,%5F2018%5BV004850366%5D.mp4&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A80%2Fannounce&tr=udp%3A%2F%2Ftracker.publicbt.com%3A80%2Fannounce&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969&tr=udp%3A%2F%2Fopen.demonii.com%3A1337&ws=http%3A%2F%2Fwww.nasa.gov%2Fsites%2Fdefault%2Ffiles%2Fatoms%2Fvideo%2Fnhq%5F20180727%5Fthis%5Fweek%5Fnasa%5F1.mp4"
 * Magnet link example 2: "magnet:?xt=urn:btih:8567e2d162aba1bfd3b81cf88b257ce462eee761&dn=Rambo.First.Blood.1982.REMASTERED.1080p.BluRay.x265-RARBG&tr=http%3A%2F%2Ftracker.trackerfix.com%3A80%2Fannounce&tr=udp%3A%2F%2F9.rarbg.me%3A2890&tr=udp%3A%2F%2F9.rarbg.to%3A2890"
 * */
int main2(int argc, char const* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <magnet-url>\n", argv[0]);
        return 1;
    }
    lt::session ses;
    lt::add_torrent_params atp;
    lt::error_code ec;
    std::filesystem::create_directories("/tmp/torrents");

    lt::parse_magnet_uri(argv[1], atp, ec);
    atp.save_path = "/tmp/torrents"; // save in current dir
    lt::torrent_handle h = ses.add_torrent(atp);

    // ...
    return 0;
}

int main(int argc, char const* argv[])
{
    std::cerr << "-------- torrent_playground --- \n";
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <magnet-url>" << std::endl;
        return 1;
    }
    lt::settings_pack p;
    lt::add_torrent_params atp;
    lt::error_code ec;

    std::cerr << "Hest\n" << std::endl;
    p.set_int(lt::settings_pack::alert_mask, lt::alert_category::status
              | lt::alert_category::error);
    lt::session ses(p);

    lt::parse_magnet_uri(argv[1], atp, ec);
    std::filesystem::create_directories("/tmp/torrents");
    atp.save_path = "/tmp/torrents";
    lt::torrent_handle h = ses.add_torrent(std::move(atp));

    for (;;) {
        std::vector<lt::alert*> alerts;
        ses.pop_alerts(&alerts);

        for (lt::alert const* a : alerts) {
            std::cerr << a->message() << std::endl;
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

