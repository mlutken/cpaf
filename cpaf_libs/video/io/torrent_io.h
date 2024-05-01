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
    torrent_io() = delete;
    torrent_io(const torrent_io& other) = delete;
    torrent_io& operator=(const torrent_io& other) = delete;
    ~torrent_io() override;
private:

    bool            is_valid                () const { return torrents_instance_ && torrent_ && tor_file_.is_valid(); }
    void            torrent_cleanup         ();

    std::string     do_protocol_name        () const override { return "magnet"; }
    bool            do_open                 (const std::string& resource_path) override;
    void            do_close                () override;
    bool            do_is_open              () const override;
    int64_t         do_size                 () const noexcept override;
    void            do_cancel_current_io    () override;
    void            do_open_progress_cb_set (progress_callback_fn cb) override;
    void            do_data_progress_cb_set (progress_callback_fn cb) override;
    int             do_read_packet          (uint8_t* buf, int buf_size) override;
//    int       do_write_packet         (uint8_t* buf, int buf_size) override;
    int64_t         do_seek                 (int64_t offset, int whence) override;
    void            debug_append_to_file    (const std::filesystem::path& p, std::span<uint8_t> data);

    torrent::file                           tor_file_;
    std::shared_ptr<torrent::torrents>      torrents_instance_;
    std::shared_ptr<torrent::torrent>       torrent_;
    std::atomic<bool>                       io_cancel_requested_{false};
    progress_callback_fn                    open_progress_callback_ {};
    progress_callback_fn                    data_progress_callback_ {};
};

} //END namespace cpaf::video
