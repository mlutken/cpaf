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

torrent_io::torrent_io(std::shared_ptr<torrent::torrents> torrents_instance) :
    torrents_instance_(torrents_instance)
{
    ofstream(tor_data_path_, ios_base::trunc);
    ofstream(file_data_path_, ios_base::trunc);
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
    debug_file_open(debug_file_path_);

    return true;
}

/// @note torrent::file::close() is not yet implemented!
void torrent_io::do_close()
{
    debug_file_close();

    if (!tor_file_.is_valid()) {
        tor_file_.close();
    }
}

bool torrent_io::do_is_open() const
{
//    return debug_file_is_open();
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
    std::cerr << "File [" << tor_file_.file_index() << "]" << "torrent_io::do_read_packet() offset: " << tor_file_.offset() << "\n";
//    return debug_file_read_packet(buf, buf_size);

    if (!tor_file_.is_valid()) {
        return AVERROR_EOF;
    }
    auto ret = tor_file_.read(buf, static_cast<size_t>(buf_size));

//    {
//        debug_append_to_file(tor_data_path_, span(buf, static_cast<size_t>(buf_size)) );
//        const auto bytes_read = fread(debug_file_buf_.data(), sizeof(uint8_t), static_cast<size_t>(buf_size), debug_file_);
//        debug_append_to_file(file_data_path_, span(debug_file_buf_.data(), static_cast<size_t>(buf_size)) );
//        auto ret = fread(buf, sizeof(uint8_t), static_cast<size_t>(buf_size), debug_file_);
//    }


    return ret;
}

int64_t torrent_io::do_seek(int64_t offset, int whence)
{
    const auto debug_file_new_pos = debug_file_seek(offset, whence);

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
    std::cerr << "torrent_io::do_seek (" << offset << ", " << whence << ") debug_file: " << debug_file_new_pos << ",  tor_file: " << new_pos << "\n";


    return new_pos;
//    return debug_file_new_pos;
}

bool torrent_io::debug_file_open(const std::string& resource_path)
{
    debug_file_path_ = resource_path;
    debug_file_ = fopen(debug_file_path_.c_str(), "rb");
    return debug_file_ != nullptr;
}

void torrent_io::debug_file_close()
{
    if (debug_file_) {
        fclose(debug_file_);
        debug_file_ = nullptr;
    }
}

bool torrent_io::debug_file_is_open() const
{
    return debug_file_ != nullptr;
}

int torrent_io::debug_file_read_packet(uint8_t* buf, int buf_size)
{
    const auto bytes_read = fread(buf, sizeof(uint8_t), static_cast<size_t>(buf_size), debug_file_);
    return bytes_read;
}

int64_t torrent_io::debug_file_seek(int64_t offset, int whence)
{
    if ( (AVSEEK_SIZE & whence) == AVSEEK_SIZE){
        const auto new_pos = static_cast<int64_t>(std::filesystem::file_size(debug_file_path_));
        return new_pos;
    }

    switch(whence) {
    case SEEK_SET:
        fseek(debug_file_, offset, whence);
        break;
    case SEEK_CUR:
        fseek(debug_file_, offset, whence);
        break;
    case SEEK_END:
        fseek(debug_file_, offset, whence);
        break;
    default:
        return AVERROR(EINVAL);
    }

    const auto new_pos = static_cast<int64_t>(ftell(debug_file_));
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
