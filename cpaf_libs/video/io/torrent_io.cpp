#include "torrent_io.h"



bool torrent_io::do_open(const std::string& /*resource_path*/)
{
    return false;
}

void torrent_io::do_close()
{

}

bool torrent_io::do_is_open() const
{
    return false;
}

int torrent_io::do_read_packet(uint8_t* /*buf*/, int /*buf_size*/)
{
    return AVERROR_EOF;
}

int64_t torrent_io::do_seek(int64_t /*offset*/, int /*whence*/)
{
    return -1; // ERROR
}
