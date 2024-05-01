#pragma once

extern "C"
{
#include <libavformat/avformat.h>
}

#include <memory>
#include <chrono>
#include <cpaf_libs/video/av_util.h>

namespace cpaf::torrent {
class torrents;
}


namespace cpaf::video {

/**
 * @brief The custom_io_base class
 *
 * @note Factory self registration dropped due to challenges with static libs and static var initialization.
 * So until a solution for this is found we do it the simple way :)
 * @see Inspiration to factory pattern: https://dzone.com/articles/factory-with-self-registering-types
 * @see https://www.cppstories.com/2018/02/static-vars-static-lib/
 */
class custom_io_base
{
public:
    static std::unique_ptr<custom_io_base>  create (const std::string& resource_path,
                                                    get_torrents_fn get_torrents_function);

    explicit custom_io_base();
    virtual ~custom_io_base();
    std::string         protocol_name               () const { return do_protocol_name(); }
    bool                open                        (const std::string& resource_path);
    void                close                       ();
    bool                is_open                     () { return do_is_open(); }
    int64_t             size                        () const noexcept   { return do_size(); }
    void                cancel_current_io           ()                  { do_cancel_current_io(); }
    size_t              buffer_size                 () const noexcept   { return do_buffer_size(); };
    void                open_progress_callback_set  (progress_callback_fn cb) { do_open_progress_cb_set(std::move(cb)); }
    void                data_progress_callback_set  (progress_callback_fn cb) { do_data_progress_cb_set(std::move(cb)); }
    const std::string&  resource_path               () const { return resource_path_; }
    bool                init                        (AVFormatContext*  ff_format_context);

    std::chrono::microseconds
                        current_io_operation_duration() const;
//    AVIOContext*        ff_avio_context         () const { return ff_avio_context_; }

protected:
    std::chrono::milliseconds   io_timeout_     = std::chrono::minutes(1);

    bool                keep_downloaded_      = false;
    //    bool                use_blocking_seek_      = true;

private:
    using time_point_t = std::chrono::steady_clock::time_point;

    uint8_t*                ff_avio_context_buffer_ = nullptr;
    AVIOContext*            ff_avio_context_        = nullptr;
    std::string             resource_path_;
    mutable time_point_t    last_io_op_started_time_;
    void                    cleanup                 ();
    void                    mark_io_begin           () const { last_io_op_started_time_ = std::chrono::steady_clock::now();}
    void                    mark_io_done            () const { last_io_op_started_time_ = time_point_t();}

    int                     read_packet             (uint8_t* buf, int buf_size);
    int                     write_packet            (uint8_t* buf, int buf_size);
    int64_t                 seek                    (int64_t offset, int whence);


    virtual std::string     do_protocol_name        () const { return ""; }
    virtual bool            do_open                 (const std::string& resource_path) = 0;
    virtual void            do_close                () = 0;
    virtual bool            do_is_open              () const = 0;
    virtual int64_t         do_size                 () const noexcept = 0;
    virtual void            do_cancel_current_io    () = 0;
    virtual size_t          do_buffer_size          () const noexcept { return 4096; };
    virtual void            do_open_progress_cb_set (progress_callback_fn cb) = 0;
    virtual void            do_data_progress_cb_set (progress_callback_fn cb) = 0;

    virtual int             do_read_packet          (uint8_t* buf, int buf_size) = 0;
    virtual int             do_write_packet         (uint8_t* buf, int buf_size);
    virtual int64_t         do_seek                 (int64_t offset, int whence) = 0;

    static int              read_packet_wrapper     (void* opaque, uint8_t* buf, int buf_size);
    static int              write_packet_wrapper    (void* opaque, uint8_t* buf, int buf_size);
    static int64_t          seek_wrapper            (void* opaque, int64_t offset, int whence);

};

} //END namespace cpaf::video
