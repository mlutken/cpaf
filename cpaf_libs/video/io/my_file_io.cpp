#include "my_file_io.h"

#include <cpaf_libs/unicode/cpaf_u8string_utils.h>
#include <filesystem>
#include <thread>

namespace cu = cpaf::unicode;
using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

namespace cpaf::video {

#if 1
bool my_file_io::do_open(const std::string& resource_path)
{
    resource_file_path_ = cu::substring_between(resource_path, ":", "");
    file_ = fopen(resource_file_path_.c_str(), "rb");
    return file_ != nullptr;
}

void my_file_io::do_close()
{
    if (file_) {
        fclose(file_);
        file_ = nullptr;
    }
}

bool my_file_io::do_is_open() const
{
    return file_ != nullptr;
}

int64_t my_file_io::do_size() const noexcept
{
    return static_cast<int64_t>(std::filesystem::file_size(resource_file_path_));
}

int my_file_io::do_read_packet(uint8_t* buf, int buf_size)
{
    const auto bytes_read = fread(buf, sizeof(uint8_t), static_cast<size_t>(buf_size), file_);
    return bytes_read;
}

/**
 * @brief my_file_io::do_seek
 * @param offset
 * @param whence
 * @return
 *
 * @see https://cdry.wordpress.com/2009/09/09/using-custom-io-callbacks-with-ffmpeg/
 * @see https://ffmpeg.org/doxygen/trunk/avio_read_callback_8c-example.html
 * @see https://ffmpeg.org/doxygen/trunk/structAVIOContext.html
 */

int64_t my_file_io::do_seek(int64_t offset, int whence)
{
    switch(whence) {
    case AVSEEK_SIZE:
    {
        const auto new_pos = static_cast<int64_t>(std::filesystem::file_size(resource_file_path_));
        return new_pos;
    }
        //        return static_cast<int64_t>(std::filesystem::file_size(resource_file_path_));
        break;
    case SEEK_SET:
        fseek(file_, offset, whence);
        break;
    case SEEK_CUR:
        fseek(file_, offset, whence);
        break;
    case SEEK_END:
        fseek(file_, offset, whence);
        break;
    default:
        return AVERROR(EINVAL);
    }

    const auto new_pos = static_cast<int64_t>(ftell(file_));
    return new_pos;
//    return static_cast<int64_t>(stream_.tellg());
}
#else

bool my_file_io::do_open(const std::string& resource_path)
{
    resource_file_path_ = cu::substring_between(resource_path, ":", "");
    stream_.open(resource_file_path_, ios::binary | ios_base::in | ios_base::out );
    const bool is_open = stream_.is_open();
    return is_open;
}

void my_file_io::do_close()
{
    stream_.close();
}

bool my_file_io::do_is_open() const
{
    return stream_.is_open();
}

int my_file_io::do_read_packet(uint8_t* buf, int buf_size)
{
    stream_.read(reinterpret_cast<fstream::char_type*>(buf), buf_size);
    const auto bytes_read = stream_.gcount();
    return bytes_read;
}

/**
 * @brief my_file_io::do_seek
 * @param offset
 * @param whence
 * @return
 *
 * @see https://cdry.wordpress.com/2009/09/09/using-custom-io-callbacks-with-ffmpeg/
 * @see https://ffmpeg.org/doxygen/trunk/avio_read_callback_8c-example.html
 * @see https://ffmpeg.org/doxygen/trunk/structAVIOContext.html
 */
int64_t my_file_io::do_seek(int64_t offset, int whence)
{
    // Seeking does NOT work properly if file is in a fail state....
    if (stream_.fail()) {
        stream_.clear();
    }
    const auto off = static_cast<fstream::off_type>(offset);
    switch(whence) {
    case AVSEEK_SIZE:
    {
        const auto new_pos = static_cast<int64_t>(std::filesystem::file_size(resource_file_path_));
        return new_pos;
    }
        //        return static_cast<int64_t>(std::filesystem::file_size(resource_file_path_));
        break;
    case SEEK_SET:
        stream_.seekg(off);
        break;
    case SEEK_CUR:
        stream_.seekg(off, std::ios_base::cur);
        break;
    case SEEK_END:
        stream_.seekg(off, std::ios_base::end);
        break;
    default:
        return AVERROR(EINVAL);
    }

    const auto new_pos = static_cast<int64_t>(stream_.tellg());
    return new_pos;
//    return static_cast<int64_t>(stream_.tellg());
}
#endif

} //END namespace cpaf::video
