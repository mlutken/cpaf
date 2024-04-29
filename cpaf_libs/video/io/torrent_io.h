#pragma once

#include <memory>
#include <filesystem>
#include <span>
#include <array>
#include <atomic>

#include "custom_io_base.h"

#include <cpaf_libs/torrent/file.h>

namespace cpaf::torrent {
class torrents;
class torrent;
}

namespace cpaf::video {

class torrent_io : public custom_io_base
{
public:
    explicit torrent_io(std::shared_ptr<torrent::torrents> torrents_instance);

    ~torrent_io() override;
private:

    void            torrent_cleanup         ();

    std::string     do_protocol_name        () const override { return "magnet"; }
    bool            do_open                 (const std::string& resource_path) override;
    void            do_close                () override;
    bool            do_is_open              () const override;
    int64_t         do_size                 () const noexcept override;
    void            do_cancel_current_io    () override;
    int             do_read_packet          (uint8_t* buf, int buf_size) override;
//    int       do_write_packet         (uint8_t* buf, int buf_size) override;
    int64_t         do_seek                 (int64_t offset, int whence) override;
    void            debug_append_to_file    (const std::filesystem::path& p, std::span<uint8_t> data);

    torrent::file                           tor_file_;
    std::shared_ptr<torrent::torrents>      torrents_instance_;
    std::shared_ptr<torrent::torrent>       torrent_;
    std::atomic<bool>                       io_cancel_requested_{false};
};

} //END namespace cpaf::video
