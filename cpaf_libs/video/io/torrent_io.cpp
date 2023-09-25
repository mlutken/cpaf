#include "torrent_io.h"
#include <cpaf_libs/torrent/torrents.h>
//#include <cpaf_libs/torrent/>

namespace cpaf::video {

torrent_io::torrent_io(std::shared_ptr<torrent::torrents> torrents_instance) :
    torrents_instance_(torrents_instance)
{

}


bool torrent_io::do_open(const std::string& resource_path)
{
    torrent_ = torrents_instance_->add_torrent(resource_path);

    if (!torrent_) {
        return false;
    }
    std::cerr << "Waiting for meta data for '" << torrent_->name() << "' ... ";
    torrent_->wait_for_meta_data();
    std::cerr << " done!\n";

    tor_file_ = torrent_->open_largest_file_streaming(torrents_instance_->default_read_ahead_size());
    if (!tor_file_.is_valid()) {
        return false;
    }

    return true;
}

/// @note torrent::file::close() is not yet implemented!
void torrent_io::do_close()
{
    if (!tor_file_.is_valid()) {
        tor_file_.close();
    }
}

bool torrent_io::do_is_open() const
{
    return tor_file_.is_valid();
}

int torrent_io::do_read_packet(uint8_t* buf, int buf_size)
{
    if (!tor_file_.is_valid()) {
        return AVERROR_EOF;
    }
    return tor_file_.read(buf, static_cast<size_t>(buf_size));
}

int64_t torrent_io::do_seek(int64_t offset, int whence)
{
    if (!tor_file_.is_valid()) {
        return -1; // ERROR
    }
    return tor_file_.seek(offset, whence);
}

} //END namespace cpaf::video
