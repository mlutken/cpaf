#include "torrents.h"

#include <iostream>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/alert_types.hpp>
#include <cpaf_libs/torrent/torrent_utils.h>
#include <cpaf_libs/torrent/temp_storage.h>
#include <libtorrent/posix_disk_io.hpp>
#include <libtorrent/mmap_disk_io.hpp>


using namespace std;
using namespace std::filesystem;

namespace cpaf::torrent {

torrents::torrents()
{
    settings_pack_.set_int(lt::settings_pack::alert_mask,
                           lt::alert_category::status |
                           lt::alert_category::error |
                           lt::alert_category::piece_progress
                           );
}

void torrents::start()
{
    if (!base_torrents_path_.empty()) {
        create_directories(base_torrents_path_);
    }

    session_params_ = lt::session_params(settings_pack_);
    lt::session_params ses_params(settings_pack_);
    if (storage_mode() == storage_mode_t::memory) {
        ses_params.disk_io_constructor = temp_disk_constructor;
    }
    else {
//        ses_params.disk_io_constructor = lt::posix_disk_io_constructor;
        ses_params.disk_io_constructor = lt::mmap_disk_io_constructor;
    }
    session_ptr_ = make_unique<lt::session>(ses_params);
}

void torrents::stop()
{

}

void torrents::frame_update()
{
    handle_alerts();
}

torrent torrents::add_torrent(const std::string& uri_or_name)
{
    if (!has_torrent(uri_or_name)) {
        return create(uri_or_name);
    }
    else {
        return torrents_map_[uri_or_name];
    }
}


void torrents::delete_torrent(const std::string& uri_or_name)
{
    const auto name = torrent_name(uri_or_name);
    // TODO:
}

bool torrents::has_torrent(const std::string& uri_or_name) const
{
    return torrents_map_.contains(torrent_name(uri_or_name));
}



void torrents::debug_print_alerts_set(bool newDebug_print_alerts)
{
    debug_print_alerts_ = newDebug_print_alerts;
}

// ---------------------------------
// --- PRIVATE: Helper functions ---
// ---------------------------------

torrent torrents::create(const std::string& uri_or_name)
{
    lt::error_code ec;
    lt::add_torrent_params add_torrent_params;

    lt::parse_magnet_uri(uri_or_name, add_torrent_params, ec);
    add_torrent_params.save_path = base_torrents_path_.string();
    add_torrent_params.storage_mode = lt::storage_mode_t::storage_mode_sparse; // storage_mode_sparse, storage_mode_allocate
    lt::torrent_handle handle = session_ptr_->add_torrent(std::move(add_torrent_params));
//    handle.set_sequential_download(true);
    handle.set_flags(lt::torrent_flags::sequential_download);
    const auto f = torrent(uri_or_name, handle, this);
    torrents_map_[f.name()] = f;
    return f;
}

void torrents::handle_alerts()
{
    std::vector<lt::alert*> alerts;
    session_ptr_->pop_alerts(&alerts);

    for (lt::alert const* a : alerts) {
        if (debug_print_alerts()) {
            std::cerr << a->message() << "\n";
        }

//        set_piece_downloaded()

        if (auto pfa = lt::alert_cast<lt::piece_finished_alert>(a)) {
//            std::cerr << "piece finished index: " << pfa->piece_index << ", torrent name: " << pfa->torrent_name() << "\n";
            torrents_map_[pfa->torrent_name()].set_piece_downloaded(pfa->piece_index);
//                if (pfinished_alert->piece_index < 30) {
//            pfa->handle.read_piece(pfa->piece_index);
                if (pfa->piece_index < 30) {
//                    std::cerr << "pfinished_alert->piece_index: " << pfinished_alert->piece_index << "\n";
                }
        }
        else if (auto read_piece_alert = lt::alert_cast<lt::read_piece_alert>(a)) {
//                if (pfinished_alert->piece_index < 30) {
                if (pfa->piece_index < 30) {
                    std::cerr << "!!! read_piece_alert->piece_index: " << pfa->piece_index << "\n";
                }
        }


//        if (lt::alert_cast<lt::torrent_finished_alert>(a)) {
//        }
//        if (lt::alert_cast<lt::torrent_error_alert>(a)) {
//        }
    }
}




} //END namespace cpaf::torrent


