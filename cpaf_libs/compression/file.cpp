#include "file.h"

#include <iostream>

extern "C" {
#include <zlib.h>
}


#include <filesystem/cpaf_file_directory_functions.h>
#include <boost/algorithm/string.hpp>

namespace fs = std::filesystem;
namespace al = boost::algorithm;

namespace cpaf { namespace compression {

struct file_handle_t {
    std::variant<std::FILE*, gzFile> h;
};


// -------------------------------------
// --- Generic file functions handle ---
// -------------------------------------

file::file()
    : handle_(std::make_unique<file_handle_t>())
{

}

file::file(const std::string& file_path)
    : handle_(std::make_unique<file_handle_t>())
    , file_path_(file_path)
{
    allocate_read_buffer();
}

file::file(const std::filesystem::path& file_path)
    : file(file_path.string())
{
}

file::file(const std::string& file_path, const std::string& mode)
    : handle_(std::make_unique<file_handle_t>())
    , file_path_(file_path)
{
    if (!open(file_path, mode)) {
        throw std::ios_base::failure("Could not open file: " + file_path_);
    }
    allocate_read_buffer();
}

file::file(const std::filesystem::path& file_path, const std::string& mode)
    : file(file_path.string(), mode)
{
}

file::file(const std::string& file_path, const std::string& mode, std::size_t max_readline_size)
    : file(file_path, mode)
{
    max_readline_size_ = max_readline_size;
}

file::file(const std::filesystem::path& file_path, const std::string& mode, std::size_t max_readline_size)
    : file(file_path.string(), mode, max_readline_size)
{
}


file::~file() noexcept
{
    close();
    deallocate_read_buffer();
}

void file::max_readline_size(std::size_t max_readline_size) noexcept
{
    max_readline_size_ = max_readline_size;
    allocate_read_buffer();
}

// -----------------------------
// --- file: state functions ---
// -----------------------------

bool file::open (const std::string& mode) noexcept
{
    return open(file_path_, mode);
}

bool file::open (const std::string& file_path, const std::string& mode) noexcept
{
    file_path_ = file_path;
    mode_ = mode;

    if (file_path_.empty()) return false;
    if (is_open()) return true;

    file_type_ = to_compression_type (file_path);
    switch ( file_type_ )
    {
        case type::gz: {
            handle_->h = gzopen(file_path.c_str(), mode.c_str());
            is_open_ = std::get< gzFile>(handle_->h) != NULL;
            break;
        }
        default: {
            handle_->h = std::fopen(file_path.c_str(), mode.c_str());
            is_open_ = std::get<std::FILE*>(handle_->h) != nullptr;
        }
    }
    return is_open_;
}

bool file::open(const std::filesystem::path& file_path, const std::string& mode) noexcept
{
    return open(file_path.string(), mode);
}

bool file::close() noexcept
{
    if (!is_open()) return true;
    switch ( file_type_ )
    {
        case type::gz: {
            gzFile fh = std::get<gzFile>(handle_->h);
            is_open_ = !(gzclose(fh) == Z_OK);
            break;
        }
        default: {
            std::FILE* fh = std::get<std::FILE*>(handle_->h);
            is_open_ = !(std::fclose(fh) == 0);
        }
    }
    return !is_open_;
}

bool file::eof () const noexcept
{
    switch ( file_type_ )
    {
        case type::gz: {
            const gzFile fh = std::get<gzFile>(handle_->h);
            return gzeof(fh) != 0;
            break;
        }
        default:
            ;
    }
    return std::feof(std::get<std::FILE*>(handle_->h)) != 0;
}

// ----------------------------
// --- file: Read functions ---
// ----------------------------
char* file::gets (char *buf) const noexcept
{
    switch ( file_type_ )
    {
        case type::gz: {
            gzFile fh = std::get<gzFile>(handle_->h);
            return gzgets(fh, buf, max_readline_size());
            break;
        }
        default:
            ;
    }
    std::FILE* fh = std::get<std::FILE*>(handle_->h);
    char* ret = std::fgets(buf, max_readline_size(), fh);
    return ret;
}

/** Read a line and return as a string.
    Probably you should use gets(char* buf) functions if you are
    concerned with performance. */
char* file::gets() const noexcept
{
    gets(read_buffer_);
    return read_buffer_;
}

std::string_view file::readline() const noexcept
{
    if(!gets()) return "";
    return read_buffer_;
}

// -----------------------------
// --- file: Write functions ---
// -----------------------------
bool file::puts (const char* buf) const noexcept
{
    switch ( file_type_ )
    {
        case type::gz: {
            gzFile fh = std::get<gzFile>(handle_->h);
            return gzputs(fh, buf) != -1;
            break;
        }
        default:
            ;
    }
    std::FILE* fh = std::get<std::FILE*>(handle_->h);
    return std::fputs(buf, fh) > 0;
}

bool file::puts (const std::string& str) const noexcept
{
    return puts(str.c_str());
}

std::size_t file::write(const char* buf, std::size_t bytes_to_write) const noexcept
{
    switch ( file_type_ )
    {
        case type::gz: {
            gzFile fh = std::get<gzFile>(handle_->h);
            return static_cast<std::size_t>(gzwrite(fh, buf, bytes_to_write)); // gzwrite() never returns negative numbers!
            break;
        }
        default:
            ;
    }
    std::FILE* fh = std::get<std::FILE*>(handle_->h);
    return std::fwrite (buf, sizeof (char), bytes_to_write, fh);
}

std::size_t file::write(const std::string& str, std::size_t start_pos, std::size_t bytes_to_write) const noexcept
{
    return write(&str[start_pos], bytes_to_write);
}

void file::allocate_read_buffer()
{
    deallocate_read_buffer();
    read_buffer_ = new char[max_readline_size()];
}

void file::deallocate_read_buffer()
{
    if (read_buffer_) {
        delete[] read_buffer_;
        read_buffer_ = nullptr;
    }
}




}} //END namespace cpaf::compression


