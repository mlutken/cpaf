#pragma once

#include <filesystem>
#include <unordered_map>
#include <memory>
#include <thread>
#include <libtorrent/session.hpp>
#include <cpaf_libs/torrent/torrent.h>


namespace cpaf::torrent {

enum class storage_mode_t { filesystem, memory };

/**
 * @brief The torrents class
 *
 *
 *
 *
 *  @see https://en.wikipedia.org/wiki/Standard_(warez)#Naming
 *  @see https://torrent.wonderhowto.com/how-to/guide-torrent-acronyms-choose-right-download-for-your-damaged-missing-media-0134623/
*/

class torrents
{
public:
    torrents();
    const std::filesystem::path&    base_torrents_path      () const                                        { return base_torrents_path_;  }
    void                            base_torrents_path_set  (const std::filesystem::path& base_tor_path)    { base_torrents_path_ = base_tor_path;  }
    void                            settings_pack_set       (const lt::settings_pack& settings_pack)        { settings_pack_ = settings_pack;       }

    void                            start                   ();
    void                            stop                    ();
    void                            frame_update            ();

    std::shared_ptr<torrent>        add_torrent             (const std::string& uri_or_name);
    void                            delete_torrent          (const std::string& uri_or_name);
    bool                            has_torrent             (const std::string& uri_or_name) const;

    storage_mode_t                  storage_mode            () const                                        { return storage_mode_;         }
    void                            storage_mode_set        (storage_mode_t storage_mode)                   { storage_mode_ = storage_mode; }

    bool                            debug_print_alerts      () const                                        { return debug_print_alerts_; }
    void                            debug_print_alerts_set  (bool newDebug_print_alerts);

private:
    using torrents_map_t = std::unordered_map<std::size_t, std::shared_ptr<torrent>>;

    std::shared_ptr<torrent>        create                  (const std::string& uri_or_name);
    void                            background_process_fun  ();
    void                            handle_alerts           ();

    std::unique_ptr<lt::session>    session_ptr_;
    lt::settings_pack               settings_pack_;
    lt::session_params              session_params_;

    torrents_map_t                  torrents_map_;
    std::filesystem::path           base_torrents_path_     = "/tmp/torrents";
    std::unique_ptr<std::jthread>   background_process_thread_;
    storage_mode_t                  storage_mode_           = storage_mode_t::filesystem;
    std::atomic<bool>               is_running_             = true;
    bool                            debug_print_alerts_     = false;

};


} //END namespace cpaf::torrent




