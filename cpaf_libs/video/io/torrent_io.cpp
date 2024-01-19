#include "torrent_io.h"

#include <fstream>
#include <cpaf_libs/torrent/torrents.h>
#include <cpaf_libs/unicode/cpaf_u8string_utils.h>
//#include <cpaf_libs/torrent/>

namespace cu = cpaf::unicode;
using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

namespace cpaf::video {

torrent_io::torrent_io(std::atomic<stream_state_t>& stream_state,
                       std::shared_ptr<torrent::torrents> torrents_instance)
    : custom_io_base(stream_state)
    , torrents_instance_(torrents_instance)
{
}


bool torrent_io::do_open(const std::string& resource_path)
{
    torrent_ = torrents_instance_->add_torrent(resource_path);

    if (!torrent_) {
        return false;
    }

    std::cerr << "!! Waiting for meta data for '" << torrent_->name() << "' ...\n";
    torrent_->wait_for_meta_data();
    std::cerr << " !! Waiting for meta data done!\n";

    cerr << "Largest file index         : '" << torrent_->largest_file_index() << "'\n";
    cerr << "Largest file               : '" << torrent_->largest_file_local_file_path() << "'\n";
    cerr << "Number of pieces           : " << torrent_->num_pieces() << "\n";
    cerr << "End piece                  : " << torrent_->piece_index_end() << "\n";
    cerr << "Piece len                  : " << torrent_->piece_length() << "\n";

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

int64_t torrent_io::do_size() const noexcept
{
    if (!tor_file_.is_valid()) {
        return 0;
    }
    return tor_file_.size();
}

int torrent_io::do_read_packet(uint8_t* buf, int buf_size)
{
//    std::cerr << "File [" << tor_file_.file_index() << "]" << "torrent_io::do_read_packet() offset: " << tor_file_.offset() << "\n";

    if (!tor_file_.is_valid()) {
        return AVERROR_EOF;
    }
    const auto ret = tor_file_.read(buf, static_cast<size_t>(buf_size));
    return ret;
}

int64_t torrent_io::do_seek(int64_t offset, int whence)
{
    if (!tor_file_.is_valid()) {
        return AVERROR(ENOENT); // ERROR
    }

    int64_t new_pos = AVERROR(EINVAL);
    if ( (AVSEEK_SIZE & whence) == AVSEEK_SIZE){
        new_pos = size();
    }
    else if (whence == SEEK_SET || whence == SEEK_CUR || whence == SEEK_END) {
        if ( tor_file_.seek(offset, whence) == 0) {
            new_pos = tor_file_.offset();
        }
    }

//    std::cerr << "torrent_io::do_seek (" << offset << ", " << whence << ") tor_file new pos: " << new_pos << "\n";
    return new_pos;
}

void torrent_io::debug_append_to_file(const std::filesystem::path& p, std::span<uint8_t> data)
{
    ofstream os(p, ios_base::app);
    os << hex;
    for (auto c : data) {
        os << static_cast<int>(c) << '\n';
    }
}

} //END namespace cpaf::video
