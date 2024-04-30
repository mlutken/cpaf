#pragma once

#include <filesystem>
#include <unordered_map>
#include <memory>
#include <thread>
#include <functional>
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
    using events_callback_fn = std::function<void (std::shared_ptr<torrent> tor_file)>;

    torrents();
    ~torrents();
    const std::filesystem::path&    base_torrents_dir           () const                                        { return base_torrents_dir_;  }
    void                            base_torrents_dir_set       (const std::filesystem::path& base_tor_path)    { base_torrents_dir_ = base_tor_path;  }
    void                            settings_pack_set           (const lt::settings_pack& settings_pack)        { settings_pack_ = settings_pack;       }

    void                            start                       ();
    void                            stop                        ();
    void                            frame_update                ();

    std::shared_ptr<torrent>        find_torrent                (const std::string& uri_or_name);
    std::shared_ptr<torrent>        add_torrent                 (const std::string& uri_or_name, const std::filesystem::path& base_dir = "");
    void                            remove_torrent              (const torrent& tor);
    void                            remove_torrent              (const std::string& uri_or_name);
    bool                            has_torrent                 (const std::string& uri_or_name) const;

    storage_mode_t                  storage_mode                () const                                        { return storage_mode_;         }
    void                            storage_mode_set            (storage_mode_t storage_mode)                   { storage_mode_ = storage_mode; }

    size_t                          default_read_ahead_size     () const                                        { return default_read_ahead_size_;  }
    void                            default_read_ahead_size_set (size_t bytes)                                  { default_read_ahead_size_ = bytes; }

    bool                            debug_print_alerts          () const                                        { return debug_print_alerts_; }
    void                            debug_print_alerts_set      (bool newDebug_print_alerts);

    void                            register_finished_event     (events_callback_fn callback)                   { finished_event_callback_ = callback; }

private:
    using torrents_map_t = std::unordered_map<std::size_t, std::shared_ptr<torrent>>;

    std::shared_ptr<torrent>        construct_torrent           (const std::string& uri_or_name, std::filesystem::path base_dir);
    void                            add_default_settings        (lt::settings_pack& settings) const;
    void                            background_process_fun      ();
    void                            handle_alerts               ();

    std::unique_ptr<lt::session>    session_ptr_;
    lt::settings_pack               settings_pack_;
    lt::session_params              session_params_;

    torrents_map_t                  torrents_map_;
    std::filesystem::path           base_torrents_dir_         = "/tmp/torrents";
    std::unique_ptr<std::jthread>   background_process_thread_  ;
    events_callback_fn              finished_event_callback_    ;
    size_t                          default_read_ahead_size_    = 10'000'000;
    storage_mode_t                  storage_mode_               = storage_mode_t::filesystem;
    std::atomic<bool>               is_running_                 = true;
    bool                            debug_print_alerts_         = false;


};


} //END namespace cpaf::torrent




