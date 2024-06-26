#include "torrents.h"

#include <iostream>
#include <fmt/format.h>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/alert_types.hpp>
#include <cpaf_libs/torrent/torrent_utils.h>
#include <cpaf_libs/torrent/temp_storage.h>
#include <libtorrent/posix_disk_io.hpp>
#include <libtorrent/mmap_disk_io.hpp>


using namespace std;
using namespace std::filesystem;
using namespace std::chrono;
using namespace std::chrono_literals;

namespace cpaf::torrent {

torrents::torrents()
{
    settings_pack_.set_int(lt::settings_pack::alert_mask,
                           lt::alert_category::status |
                           lt::alert_category::error |
                           lt::alert_category::piece_progress
                           );
}

torrents::~torrents()
{
    stop();
}

void torrents::start()
{
    if (!base_torrents_dir_.empty()) {
        create_directories(base_torrents_dir_);
    }

    add_default_settings(settings_pack_);
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
    background_process_thread_ = make_unique<jthread>( [this] () {this->background_process_fun(); });
}

void torrents::stop()
{
    is_running_ = false;
    background_process_thread_->request_stop();
    background_process_thread_->join();
    background_process_thread_.reset();
}

void torrents::frame_update()
{
    //    handle_alerts();
}

std::shared_ptr<torrent> torrents::find_torrent(const std::string& uri_or_name)
{
    for (auto [hash, tor_ptr]: torrents_map_) {
        const auto& name_to_find = torrent_name(uri_or_name);
        const auto& name_test = tor_ptr->name();

        if (name_test == name_to_find) {
//            if (tor_ptr->name() == torrent_name(uri_or_name)) {
            return tor_ptr;
        }
    }
    return nullptr;
}

std::shared_ptr<torrent> torrents::add_torrent(const std::string& uri_or_name, const std::filesystem::path& base_dir)
{
     if (!has_torrent(uri_or_name)) {
        return construct_torrent(uri_or_name, base_dir);
    }
    else {
         for (auto [hash, tor_ptr]: torrents_map_) {
             if (tor_ptr->name() == torrent_name(uri_or_name)) {
                 return tor_ptr;
             }
         }
    }
    return nullptr;
}

void torrents::remove_torrent(const torrent& tor)
{
    std::cerr << fmt::format( "FIXMENM Remove torrent: {}\n", tor.local_file_path().string() );
    std::cerr << fmt::format( "FIXMENM Remove base_local_file_dir: {}\n", tor.base_local_file_dir().string() );
    const auto thash = lt::hash_value(tor.handle_);
    torrents_map_.erase(thash);
    session_ptr_->remove_torrent(tor.handle_);
}

/// @todo Implement torrents::delete_torrent()
void torrents::remove_torrent(const std::string& uri_or_name)
{
    const auto name = torrent_name(uri_or_name);
    // TODO:
}

bool torrents::has_torrent(const std::string& uri_or_name) const
{
    for (auto [hash, tor_ptr]: torrents_map_) {
        if (tor_ptr->name() == torrent_name(uri_or_name)) {
            return true;
        }
    }
    return false;
}



void torrents::debug_print_alerts_set(bool newDebug_print_alerts)
{
    debug_print_alerts_ = newDebug_print_alerts;
}

// ---------------------------------
// --- PRIVATE: Helper functions ---
// ---------------------------------

std::shared_ptr<torrent> torrents::construct_torrent(const std::string& uri_or_name, path base_dir)
{
    lt::error_code ec;
    lt::add_torrent_params add_torrent_params;
    if (base_dir.empty()) {
        base_dir = base_torrents_dir_;
    }

    lt::parse_magnet_uri(uri_or_name, add_torrent_params, ec);
    add_torrent_params.save_path = base_dir.string();
    add_torrent_params.storage_mode = lt::storage_mode_t::storage_mode_sparse; // storage_mode_sparse, storage_mode_allocate
    lt::torrent_handle handle = session_ptr_->add_torrent(std::move(add_torrent_params));
    ///    handle.set_flags(lt::torrent_flags::sequential_download);

    const auto tor_ptr = std::shared_ptr<torrent>( new torrent(uri_or_name, handle, this));
    auto tor_name = tor_ptr->name();
    const auto thash = lt::hash_value(handle);
    cerr << fmt::format("FIXMENM construct_torrent(), dht: {} torrent_name: '{}'  thash: '{}'\n", session_ptr_->is_dht_running(), tor_name, thash);
    torrents_map_[thash] = tor_ptr;
    return tor_ptr;
}

void torrents::add_default_settings(libtorrent::settings_pack& settings) const
{
    settings_set_default_bool(settings, libtorrent::settings_pack::enable_dht, true); // Enable DHT
    settings_set_default_bool(settings, libtorrent::settings_pack::enable_lsd, true); // Enable local service discovery
    settings_set_default_bool(settings, libtorrent::settings_pack::enable_upnp, true); // Enable UPnP
    settings_set_default_bool(settings, libtorrent::settings_pack::enable_natpmp, true); // Enable NAT-PMP
}

void torrents::background_process_fun()
{
    while (is_running_) {
        handle_alerts();
        this_thread::sleep_for(0ms);
    }
}

void torrents::handle_alerts()
{
    std::vector<lt::alert*> alerts;
    session_ptr_->pop_alerts(&alerts);

    for (lt::alert const* a : alerts) {
        if (debug_print_alerts()) {
            std::cerr << a->message() << "\n";
        }

#if 0
        if (auto pfa = lt::alert_cast<lt::piece_finished_alert>(a)) {
            const auto thash = lt::hash_value(pfa->handle);
            if (pfa->piece_index < 3) {
                std::cerr << "### piece_finished_alert! piece index: " << pfa->piece_index << "\n";
            }
            auto tor_ptr = torrents_map_[thash];
//            auto tor_ptr = torrents_map_[pfa->torrent_name()];
            if (tor_ptr) {
                tor_ptr->set_piece_downloaded(pfa->piece_index);
                if (tor_ptr->has_meta_data()) {
//                    std::cerr << "    >>>> Request read piece\n";
//                    pfa->handle.read_piece(pfa->piece_index);
                }
                else {
//                    std::cerr << "    >>>> Torrent has no meta_data!\n";
                }

            }
            else {
                std::cerr << "ERROR piece finished. Torrent not found!  index: " << pfa->piece_index << ", torrent name: '" << pfa->torrent_name() << "'\n";
                std::cerr << "              Torrent not found!  index: " << pfa->piece_index << ", torrent hash : '" << lt::hash_value(pfa->handle) << "'\n";
            }
//            std::cerr << "piece finished index: " << pfa->piece_index << ", torrent name: " << pfa->torrent_name() << "\n";
        }
        if (auto rpa = lt::alert_cast<lt::read_piece_alert>(a)) {
            const auto thash = lt::hash_value(rpa->handle);
            auto tor_ptr = torrents_map_[thash];
            if (tor_ptr) {
                if (rpa->piece < 3) {
                    std::cerr << "!!! read_piece_alert! piece index: " << rpa->piece << "\n";
                }

                tor_ptr->insert_piece_data_in_cache(rpa);
            }
//            rpa->handle.read_piece(rpa->piece);
        }

#endif

        if (auto pfa = lt::alert_cast<lt::piece_finished_alert>(a)) {
            auto tor_ptr = torrents_map_[lt::hash_value(pfa->handle)];
            if (tor_ptr) {
                tor_ptr->handle_piece_finished(pfa);
            }
        }
        if (auto rpa = lt::alert_cast<lt::read_piece_alert>(a)) {
            auto tor_ptr = torrents_map_[lt::hash_value(rpa->handle)];
            if (tor_ptr) {
                tor_ptr->handle_piece_read(rpa);
            }
        }
        if (auto tfa = lt::alert_cast<lt::torrent_finished_alert>(a)) {
            auto tor_ptr = torrents_map_[lt::hash_value(tfa->handle)];
            if (tor_ptr) {
                tor_ptr->handle_torrent_finished(tfa);
                if (finished_event_callback_) {
                    finished_event_callback_(tor_ptr);
                }
            }
        }
        if (auto tea = lt::alert_cast<lt::torrent_error_alert>(a)) {
            auto tor_ptr = torrents_map_[lt::hash_value(tea->handle)];
            if (tor_ptr) {
                tor_ptr->handle_torrent_error(tea);
            }
        }
    }
}




} //END namespace cpaf::torrent


