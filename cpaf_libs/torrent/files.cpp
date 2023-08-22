#include "files.h"

#include <iostream>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/alert_types.hpp>
#include <cpaf_libs/torrent/torrent_utils.h>
#include <cpaf_libs/torrent/temp_storage.h>
#include <libtorrent/posix_disk_io.hpp>


using namespace std;
using namespace std::filesystem;

namespace cpaf::torrent {

files::files()
{
    settings_pack_.set_int(lt::settings_pack::alert_mask, lt::alert_category::status | lt::alert_category::error | lt::alert_category::piece_progress);
}

void files::start()
{
    if (!base_torrents_path_.empty()) {
        create_directories(base_torrents_path_);
    }

    session_params_ = lt::session_params(settings_pack_);
    lt::session_params ses_params(settings_pack_);
    if (storage_mode() == storage_mode_t::memory) {
        ses_params.disk_io_constructor = temp_disk_constructor;
    }
    ////    ses_params.disk_io_constructor = lt::posix_disk_io_constructor;
    session_ptr_ = make_unique<lt::session>(ses_params);
}

void files::stop()
{

}

void files::frame_update()
{
    handle_alerts();
}

file files::open(const std::string& uri_or_name)
{
    if (!has_file(uri_or_name)) {
        return create(uri_or_name);
    }
    else {
        return files_map_[uri_or_name];
    }
}


void files::delete_file(const std::string& uri_or_name)
{
    const auto name = torrent_name(uri_or_name);
    // TODO:
}

bool files::has_file(const std::string& uri_or_name) const
{
    return files_map_.contains(torrent_name(uri_or_name));
}



void files::debug_print_alerts_set(bool newDebug_print_alerts)
{
    debug_print_alerts_ = newDebug_print_alerts;
}

// ---------------------------------
// --- PRIVATE: Helper functions ---
// ---------------------------------

file files::create(const std::string& uri_or_name)
{
    lt::error_code ec;
    lt::add_torrent_params add_torrent_params;

    lt::parse_magnet_uri(uri_or_name, add_torrent_params, ec);
    add_torrent_params.save_path = base_torrents_path_.string();
    lt::torrent_handle handle = session_ptr_->add_torrent(std::move(add_torrent_params));
    const auto f = file(uri_or_name, handle, this);
    files_map_[f.name()] = f;
    return f;
}

void files::handle_alerts()
{
    std::vector<lt::alert*> alerts;
    session_ptr_->pop_alerts(&alerts);

    for (lt::alert const* a : alerts) {
        if (debug_print_alerts()) {
            std::cerr << a->message() << "\n";
        }
        if (auto pfinished_alert = lt::alert_cast<lt::piece_finished_alert>(a)) {
//                if (pfinished_alert->piece_index < 30) {
                if (pfinished_alert->piece_index < 30) {
                    std::cerr << "pfinished_alert->piece_index: " << pfinished_alert->piece_index << "\n";
                }
        }

//        if (lt::alert_cast<lt::torrent_finished_alert>(a)) {
//        }
//        if (lt::alert_cast<lt::torrent_error_alert>(a)) {
//        }
    }
}




} //END namespace cpaf::torrent


