#pragma once

#include "custom_io_base.h"

//#include <fstream>
#include <stdio.h>

namespace cpaf::video {

class my_file_io : public custom_io_base
{
public:
    explicit my_file_io();

private:
    std::string             resource_file_path_;
//    std::fstream           stream_;
    FILE*                  file_ = nullptr;

    std::string     do_protocol_name        () const override { return "myfile"; }
    bool            do_open                 (const std::string& resource_path) override;
    void            do_close                () override;
    bool            do_is_open              () const override;
    int64_t         do_size                 () const noexcept override;
    void            do_cancel_current_io    () override;
    void            do_open_progress_cb_set (progress_callback_fn /*cb*/) override {}
    void            do_data_progress_cb_set (progress_callback_fn /*cb*/) override {}
    int             do_read_packet          (uint8_t* buf, int buf_size) override;
//    int     do_write_packet         (uint8_t* buf, int buf_size) override;
    int64_t         do_seek                 (int64_t offset, int whence) override;
};

} //END namespace cpaf::video
