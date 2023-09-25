#pragma once

#include <memory>
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

private:

    std::string     do_protocol_name        () const override { return "magnet"; }
    bool            do_open                 (const std::string& resource_path) override;
    void            do_close                () override;
    bool            do_is_open              () const override;
    int             do_read_packet          (uint8_t* buf, int buf_size) override;
//    int       do_write_packet         (uint8_t* buf, int buf_size) override;
    int64_t         do_seek                 (int64_t offset, int whence) override;

    torrent::file                           tor_file_;
    std::shared_ptr<torrent::torrents>      torrents_instance_;
    std::shared_ptr<torrent::torrent>       torrent_;

    std::string            debug_file_path_ = "/home/ml/temp/Rambo.First.Blood.1982.REMASTERED.1080p.BluRay.x265-RARBG/Rambo.First.Blood.1982.REMASTERED.1080p.BluRay.x265-RARBG.mp4";
    FILE*                  debug_file_ = nullptr;

};

} //END namespace cpaf::video
