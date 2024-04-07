#pragma once

#include <memory>
#include <filesystem>
#include <span>
#include <array>

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
    explicit torrent_io(std::atomic<stream_state_t>& stream_state,
                        std::shared_ptr<torrent::torrents> torrents_instance);

private:

    std::string     do_protocol_name        () const override { return "magnet"; }
    bool            do_open                 (const std::string& resource_path) override;
    void            do_close                () override;
    bool            do_is_open              () const override;
    int64_t         do_size                 () const noexcept override;
    int             do_read_packet          (uint8_t* buf, int buf_size) override;
//    int       do_write_packet         (uint8_t* buf, int buf_size) override;
    int64_t         do_seek                 (int64_t offset, int whence) override;
    void            debug_append_to_file    (const std::filesystem::path& p, std::span<uint8_t> data);

    torrent::file                           tor_file_;
    std::shared_ptr<torrent::torrents>      torrents_instance_;
    std::shared_ptr<torrent::torrent>       torrent_;
};

} //END namespace cpaf::video
