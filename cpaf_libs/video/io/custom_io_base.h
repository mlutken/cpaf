#pragma once

extern "C"
{
#include <libavformat/avformat.h>
}

#include <memory>

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
    static std::unique_ptr<custom_io_base>  create              (const std::string& protocol_name);
    static std::unique_ptr<custom_io_base>  create_and_open     (const std::string& name);

    custom_io_base() = default;
    virtual ~custom_io_base();
    size_t              buffer_size             () const noexcept { return do_buffer_size(); };
    bool                open                    (const std::string& resource_path);
    void                close                   () { do_close(); }
    bool                is_open                 () { do_is_open(); }
    const std::string&  resource_path           () const { return resource_path_; }
    bool                init                    (AVFormatContext*  ff_format_context);
//    AVIOContext*        ff_avio_context         () const { return ff_avio_context_; }

private:
    uint8_t*            ff_avio_context_buffer_ = nullptr;
    AVIOContext*        ff_avio_context_        = nullptr;
    std::string         resource_path_;


    virtual std::string do_protocol_name        () const { return ""; }
    virtual bool        do_open                 (const std::string& resource_path) = 0;
    virtual void        do_close                () = 0;
    virtual bool        do_is_open              () const = 0;
    virtual size_t      do_buffer_size          () const noexcept { return 4096; };
    virtual int         do_read_packet          (uint8_t* buf, int buf_size) = 0;
    virtual int         do_write_packet         (uint8_t* buf, int buf_size);
    virtual int64_t     do_seek                 (int64_t offset, int whence) = 0;

    static int          read_packet_wrapper     (void* opaque, uint8_t* buf, int buf_size);
    static int          write_packet_wrapper    (void* opaque, uint8_t* buf, int buf_size);
    static int64_t      seek_wrapper            (void* opaque, int64_t offset, int whence);

};

