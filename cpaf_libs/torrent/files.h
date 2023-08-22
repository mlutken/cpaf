#pragma once

#include <filesystem>
#include <unordered_map>
#include <memory>
#include <libtorrent/session.hpp>
#include <cpaf_libs/torrent/file.h>

namespace cpaf::torrent {

enum class storage_mode_t { filesystem, memory };

/**
 * @brief The files class
 *
 *
 *
 *
 *  @see https://en.wikipedia.org/wiki/Standard_(warez)#Naming
 *  @see https://torrent.wonderhowto.com/how-to/guide-torrent-acronyms-choose-right-download-for-your-damaged-missing-media-0134623/
*/

class files
{
public:
    files();
    const std::filesystem::path&    base_torrents_path      () const                                        { return base_torrents_path_;  }
    void                            base_torrents_path_set  (const std::filesystem::path& base_tor_path)    { base_torrents_path_ = base_tor_path;  }
    void                            settings_pack_set       (const lt::settings_pack& settings_pack)        { settings_pack_ = settings_pack;       }

    void                            start                   ();
    void                            stop                    ();
    void                            frame_update            ();

    file                            open                    (const std::string& uri_or_name);
    void                            delete_file             (const std::string& uri_or_name);
    bool                            has_file                (const std::string& uri_or_name) const;

    storage_mode_t                  storage_mode            () const                                        { return storage_mode_;         }
    void                            storage_mode_set        (storage_mode_t storage_mode)                   { storage_mode_ = storage_mode; }

    bool                            debug_print_alerts      () const                                        { return debug_print_alerts_; }
    void                            debug_print_alerts_set  (bool newDebug_print_alerts);

private:
    using files_map_t = std::unordered_map<std::string, file>;

    file                            create                  (const std::string& uri_or_name);
    void                            handle_alerts           ();

    std::unique_ptr<lt::session>    session_ptr_;
    lt::settings_pack               settings_pack_;
    lt::session_params              session_params_;

    files_map_t                     files_map_;
    std::filesystem::path           base_torrents_path_     = "/tmp/torrents";
    storage_mode_t                  storage_mode_           = storage_mode_t::filesystem;
    bool                            debug_print_alerts_     = false;

};


} //END namespace cpaf::torrent




