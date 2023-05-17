#include <iostream>
#include <thread>
#include <chrono>

#include <libtorrent/session.hpp>
#include <libtorrent/session_params.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/magnet_uri.hpp>

// https://stackoverflow.com/questions/52327290/linking-openssl-with-webassembly
// https://github.com/arvidn/libtorrent
// https://libtorrent.org/
// https://libtorrent.org/tutorial-ref.html

// https://snapcraft.io/orion-desktop
// TEST Sintel: "magnet:?xt=urn:btih:19288989FD99A41AD24F30C23B0271E0A825AC5F&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2F9.rarbg.to%3A2920%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337&tr=udp%3A%2F%2Ftracker.internetwarriors.net%3A1337%2Fannounce&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.pirateparty.gr%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.cyberia.is%3A6969%2Fannounce"
using namespace std;


int main(int argc, char const* argv[]) try
{
    cerr << "--- cpaf torrent1 playground ---\n";
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <magnet-url>" << std::endl;
        return 1;
    }
    lt::settings_pack p;
    p.set_int(lt::settings_pack::alert_mask, lt::alert_category::status
              | lt::alert_category::error);
    lt::session ses(p);

    lt::add_torrent_params atp = lt::parse_magnet_uri(argv[1]);
    atp.save_path = "."; // save in current dir
    lt::torrent_handle h = ses.add_torrent(std::move(atp));

    for (;;) {
        std::vector<lt::alert*> alerts;
        ses.pop_alerts(&alerts);

        for (lt::alert const* a : alerts) {
            std::cout << a->message() << std::endl;
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
    std::cout << "done, shutting down" << std::endl;
}
catch (std::exception& e)
{
    std::cerr << "Error: " << e.what() << std::endl;
}
