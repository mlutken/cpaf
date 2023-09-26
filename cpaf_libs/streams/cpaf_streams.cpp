#include <fstream>
#include <iomanip>
#include <memory>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

#include "cpaf_streams.h"

namespace fs = std::filesystem;
namespace al = boost::algorithm;
namespace io = boost::iostreams;
namespace con = cpaf::containers;
namespace com = cpaf::compression;

namespace cpaf::streams {

// ---------------------------------------------
// --- Get streams from resource indentifies ---
// ---------------------------------------------

cistream create_istream(const std::string& resource_path, std::ios_base::openmode extra_flags)
{
    auto compression_type = compression::to_compression_type(resource_path);
    switch (compression_type)
    {
        case compression::type::gz: {
                const fs::path file_path (resource_path);
                auto file = std::make_unique<std::ifstream>(resource_path, std::ios_base::in | std::ios::binary | extra_flags);
                auto is = std::make_unique<io::filtering_istream>();
                is->push(io::gzip_decompressor());
                is->push(*file);
                return cistream(std::move(file), std::move(is), file_path);
            }
        case compression::type::zip: {
                const fs::path file_path (resource_path);
                auto file = std::make_unique<std::ifstream>(resource_path, std::ios_base::in | std::ios::binary | extra_flags);
                auto is = std::make_unique<io::filtering_istream>();
                is->push(io::zlib_decompressor());
                is->push(*file);
                return cistream(std::move(file), std::move(is), file_path);
            }
        case compression::type::bz2: {
                const fs::path file_path (resource_path);
                auto file = std::make_unique<std::ifstream>(resource_path, std::ios_base::in | std::ios::binary | extra_flags);
                auto is = std::make_unique<io::filtering_istream>();
                is->push(io::bzip2_decompressor());
                is->push(*file);
                return cistream(std::move(file), std::move(is), file_path);
            }
        default:
            ;
    }

    // Default case is a normal "raw" std::ifstream
    const fs::path file_path (resource_path);
    auto is = std::make_unique<std::ifstream>(resource_path, std::ios_base::in | extra_flags);
    return cistream(std::make_unique<std::ifstream>(), std::move(is), file_path);
}

cistream create_istream(const std::filesystem::path& file_path, std::ios_base::openmode extra_flags)
{
    return create_istream(file_path.string(), extra_flags);
}

cistream create_istream_raw(const std::string& resource_path, std::ios_base::openmode extra_flags)
{
    const fs::path file_path (resource_path);
    auto is = std::make_unique<std::ifstream>(resource_path, std::ios_base::in | extra_flags);
    return cistream(std::make_unique<std::ifstream>(), std::move(is), file_path);
}

cistream create_istream_raw(const std::filesystem::path& file_path, std::ios_base::openmode extra_flags)
{
    return create_istream_raw(file_path.string(), extra_flags);
}

costream create_ostream(const std::string& resource_path, std::ios_base::openmode extra_flags)
{
    auto compression_type = compression::to_compression_type(resource_path);
    switch (compression_type)
    {
        case compression::type::gz: {
                const fs::path file_path (resource_path);
                auto file = std::make_unique<std::ofstream>(resource_path, std::ios_base::out | std::ios::binary | extra_flags);
                auto os = std::make_unique<io::filtering_ostream>();
                os->push(io::gzip_compressor());
                os->push(*file);
                break;
            }
        case compression::type::zip: {
                const fs::path file_path (resource_path);
                auto file = std::make_unique<std::ofstream>(resource_path, std::ios_base::out | std::ios::binary | extra_flags);
                auto os = std::make_unique<io::filtering_ostream>();
                os->push(io::zlib_compressor());
                os->push(*file);
                return costream(std::move(file), std::move(os), file_path);
            }
        case compression::type::bz2: {
                const fs::path file_path (resource_path);
                auto file = std::make_unique<std::ofstream>(resource_path, std::ios_base::out | std::ios::binary | extra_flags);
                auto os = std::make_unique<io::filtering_ostream>();
                os->push(io::bzip2_compressor());
                os->push(*file);
                return costream(std::move(file), std::move(os), file_path);
            }
        default:
            ;
    }

    // Default case is a normal "raw" std::ofstream
    const fs::path file_path (resource_path);
    auto os = std::make_unique<std::ofstream>(resource_path, std::ios_base::out | extra_flags);
    return costream(std::make_unique<std::ofstream>(), std::move(os), file_path);

}

costream create_ostream(const std::filesystem::path& file_path, std::ios_base::openmode extra_flags)
{
    return create_ostream(file_path.string(), extra_flags);
}

costream create_ostream_raw(const std::string& resource_path, std::ios_base::openmode extra_flags)
{
    const fs::path file_path (resource_path);
    auto os = std::make_unique<std::ofstream>(resource_path, std::ios_base::out | extra_flags);
    return costream(std::make_unique<std::ofstream>(), std::move(os), file_path);
}

costream create_ostream_raw(const std::filesystem::path& file_path, std::ios_base::openmode extra_flags)
{
    return create_ostream_raw(file_path.string(), extra_flags);
}

// ---------------------------------------------
// --- Get streams from resource indentifies ---
// ---------------------------------------------

std::size_t cistream::size() const
{
    if (file_path_.empty()) return 0;
    return fs::file_size(file_path_);
}

// ---------------------------------------------
// --- Read/write file convenience functions ---
// ---------------------------------------------

std::size_t calc_reserve_size(const cistream& is)
{
    if( is.compression_type() == compression::type::plain) {
        return is.size() +2;
    }
    else {
        return is.size()*8;
    }
}

cpaf::containers::string8
to_string8(const std::string& resource_path, std::size_t reserve_size)
{
    auto is = create_istream(resource_path);
    if (reserve_size == 0) {
        reserve_size = calc_reserve_size(is);
    }
    con::string8 arr(reserve_size);
    std::streamsize read_size = static_cast<std::streamsize>(reserve_size) - 1;
    while (!is.is().eof()) {
        char* write_pos = &arr[arr.size()];
        is.is().read(write_pos, read_size);
        const std::size_t bytes_read = static_cast<std::size_t>(is.is().gcount());
        arr.size_force(arr.size() + bytes_read);
        if (!is.is().eof()) {
            const size_t unsigned_read_size = static_cast<size_t>(read_size);
            if (arr.capacity() - arr.size() < unsigned_read_size) {
                arr.reserve(arr.capacity()*2);
            }
        }
    }

    arr[arr.size()] = '\0';
    return arr;
}

containers::string8 to_string8(const std::filesystem::path& file_path, std::size_t reserve_size)
{
    return to_string8(file_path.string(), reserve_size);
}

/** Read contents of a stream into a string */
std::string to_string(const std::string& resource_path, std::size_t reserve_size)
{
    auto is = create_istream(resource_path);
    if (reserve_size == 0) {
        reserve_size = calc_reserve_size(is);
    }
    if (!is.is())
    {
        return "";
    }
    std::string contents;
    contents.reserve(reserve_size);

    is.is().unsetf(std::ios::skipws); // No white space skipping!
    std::copy(
        std::istream_iterator<char>(is.is()),
        std::istream_iterator<char>(),
        std::back_inserter(contents));
    return contents;
}

std::string to_string(const std::filesystem::path& file_path, std::size_t reserve_size)
{
    return to_string(file_path.string(), reserve_size);
}

// ----------------------------------------
// --- Write file convenience functions ---
// ----------------------------------------

void to_file(const std::string& resource_path, std::span<char> str)
{
    auto os = create_ostream(resource_path);
    os.os().write(str.data(), static_cast<std::streamsize>(str.size()));
}

void to_file(const std::string& resource_path, containers::string8 str)
{
    auto os = create_ostream(resource_path);
    os.os().write(str.c_str(), static_cast<std::streamsize>(str.size()));
}

void to_file(const std::string& resource_path, const char* sz_str, std::size_t size)
{
    auto os = create_ostream(resource_path);
    os.os().write(sz_str, static_cast<std::streamsize>(size));
}


void to_file(const std::string& resource_path, const char* sz_str)
{
    auto os = create_ostream(resource_path);
    os.os() << sz_str;
}


void to_file(const std::string& resource_path, const std::string& str)
{
    auto os = create_ostream(resource_path);
    os.os().write(str.c_str(), static_cast<std::streamsize>(str.size()));
}

void to_file(const std::string& resource_path, const std::string_view& str)
{
    auto os = create_ostream(resource_path);
    os.os().write(str.data(), static_cast<std::streamsize>(str.size()));
}

void to_file(const std::string& resource_path, const std::vector<std::string>& lines, bool append_newline)
{
    auto os = create_ostream(resource_path);
    for (const auto& line: lines) {
        os.os().write(line.c_str(), static_cast<std::streamsize>(line.size()));
        if (append_newline) {
            os.os() << "\n";
        }
    }
}

void to_file(const std::filesystem::path& file_path, std::span<char> str)
{
    auto os = create_ostream(file_path);
    os.os().write(str.data(), static_cast<std::streamsize>(str.size()));
}

void to_file(const std::filesystem::path& file_path, containers::string8 str)
{
    to_file(file_path.string(), str);
}

void to_file(const std::filesystem::path& file_path, const char* sz_str)
{
    to_file(file_path.string(), sz_str);
}

void to_file(const std::filesystem::path& file_path, const char* sz_str, std::size_t size)
{
    to_file(file_path.string(), sz_str, size);
}

void to_file(const std::filesystem::path& file_path, const std::string& str)
{
    to_file(file_path.string(), str);
}

void to_file(const std::filesystem::path& file_path, const std::string_view& str)
{
    to_file(file_path.string(), str);
}

void to_file(const std::filesystem::path& file_path, const std::vector<std::string>& lines, bool append_newline)
{
    to_file(file_path.string(), lines, append_newline);
}

// --------------------------------------------
// --- Append to file convenience functions ---
// --------------------------------------------
void append_to(const std::string& resource_path, std::span<char> str)
{
    auto os = create_ostream(resource_path, std::ios_base::app);
    os.os().write(str.data(), static_cast<std::streamsize>(str.size()));
}

void append_to(const std::string& resource_path, containers::string8 str)
{
    auto os = create_ostream(resource_path, std::ios_base::app);
    os.os().write(str.c_str(), static_cast<std::streamsize>(str.size()));
}

void append_to(const std::string& resource_path, const char* sz_str)
{
    auto os = create_ostream(resource_path, std::ios_base::app);
    os.os() << sz_str;
}

void append_to(const std::string& resource_path, const std::string& str)
{
    auto os = create_ostream(resource_path, std::ios_base::app);
    os.os().write(str.c_str(), static_cast<std::streamsize>(str.size()));
}

void append_to(const std::string& resource_path, const std::string_view& str)
{
    auto os = create_ostream(resource_path, std::ios_base::app);
    os.os().write(str.data(), static_cast<std::streamsize>(str.size()));
}

void append_to(const std::string& resource_path, const std::vector<std::string>& lines, bool append_newline)
{
    auto os = create_ostream(resource_path, std::ios_base::app);
    for (const auto& line: lines) {
        os.os().write(line.c_str(), static_cast<std::streamsize>(line.size()));
        if (append_newline) {
            os.os() << "\n";
        }
    }
}

void append_to(const std::filesystem::path& file_path, std::span<char> str)
{
    append_to(file_path.string(), str);
}

void append_to(const std::filesystem::path& file_path, containers::string8 str)
{
    append_to(file_path.string(), str);
}

void append_to(const std::filesystem::path& file_path, const char* sz_str)
{
    append_to(file_path.string(), sz_str);
}

void append_to(const std::filesystem::path& file_path, const std::string& str)
{
    append_to(file_path.string(), str);
}

void append_to(const std::filesystem::path& file_path, const std::string_view& str)
{
    append_to(file_path.string(), str);
}

void append_to(const std::filesystem::path& file_path, const std::vector<std::string>& lines, bool append_newline)
{
    append_to(file_path.string(), lines, append_newline);
}

} //END namespace cpaf::streams


/**
*/

