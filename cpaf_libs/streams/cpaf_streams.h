#ifndef CPAF_STREAMS_H
#define CPAF_STREAMS_H

#include <string>
#include <istream>
#include <fstream>
#include <ostream>
#include <memory>
#include <boost/filesystem/path.hpp>
#include <containers/cpaf_string8.h>
#include <compression/cpaf_compression_functions.h>

namespace cpaf::streams {

/** \unit cpaf_streams.h
! Functions for getting streams of different kinds.
Compression, network etc.
\author Martin Lutken
*/

// ---------------------------------------------
// --- Get streams from resource indentifies ---
// ---------------------------------------------
struct cistream {
    cistream() = default;
    std::istream&       is() { return *is_;}

    std::size_t         size () const;
    compression::type   compression_type() const { return compression_type_;}

private:
    explicit cistream(std::unique_ptr<std::istream> file,
                      std::unique_ptr<std::istream> is,
                      const boost::filesystem::path& file_path)
        : file_(std::move(file)), is_(std::move(is)), file_path_(file_path)
    {
        compression_type_ = compression::to_compression_type(file_path);
    }

    std::unique_ptr<std::istream>   file_;
    std::unique_ptr<std::istream>   is_;
    boost::filesystem::path         file_path_; // if this is non-empty the resource is a file (as opposed to a network resource for example)
    compression::type               compression_type_;

    void file_path_set(const boost::filesystem::path& file_path) { file_path_ = file_path;}
    friend cistream create_istream      (const std::string& resource_path, std::ios_base::openmode extra_flags);
    friend cistream create_istream      (const boost::filesystem::path& file_path, std::ios_base::openmode extra_flags);
    friend cistream create_istream_raw  (const std::string& resource_path, std::ios_base::openmode extra_flags);
    friend cistream create_istream_raw  (const boost::filesystem::path& file_path, std::ios_base::openmode extra_flags);
};

struct costream {

    costream() = default;
    std::ostream& os() { return *os_;}
    compression::type  compression_type() const { return compression_type_;}

private:
    explicit costream(std::unique_ptr<std::ostream> file,
                      std::unique_ptr<std::ostream> os,
                      const boost::filesystem::path& file_path)
        : file_(std::move(file)), os_(std::move(os)), file_path_(file_path)
    {
        compression_type_ = compression::to_compression_type(file_path);
    }

    std::unique_ptr<std::ostream>   file_;
    std::unique_ptr<std::ostream>   os_;
    boost::filesystem::path         file_path_; // if this is non-empty the resource is a file (as opposed to a network resource for example)
    compression::type               compression_type_;

    friend costream create_ostream      (const std::string& resource_path, std::ios_base::openmode extra_flags);
    friend costream create_ostream      (const boost::filesystem::path& file_path, std::ios_base::openmode extra_flags);
    friend costream create_ostream_raw  (const std::string& resource_path, std::ios_base::openmode extra_flags);
    friend costream create_ostream_raw  (const boost::filesystem::path& file_path, std::ios_base::openmode extra_flags);
};


cistream create_istream     (const std::string& resource_path, std::ios_base::openmode extra_flags = std::ios_base::openmode());
cistream create_istream     (const boost::filesystem::path& file_path, std::ios_base::openmode extra_flags = std::ios_base::openmode());
cistream create_istream_raw (const std::string& resource_path, std::ios_base::openmode extra_flags = std::ios_base::openmode());
cistream create_istream_raw (const boost::filesystem::path& file_path, std::ios_base::openmode extra_flags = std::ios_base::openmode());

costream create_ostream     (const std::string& resource_path, std::ios_base::openmode extra_flags = std::ios_base::openmode());
costream create_ostream     (const boost::filesystem::path& file_path, std::ios_base::openmode extra_flags = std::ios_base::openmode());
costream create_ostream_raw (const std::string& resource_path, std::ios_base::openmode extra_flags = std::ios_base::openmode());
costream create_ostream_raw (const boost::filesystem::path& file_path, std::ios_base::openmode extra_flags = std::ios_base::openmode());

// ---------------------------------------
// --- Read file convenience functions ---
// ---------------------------------------
std::size_t calc_reserve_size(const cistream& is);

cpaf::containers::string8   to_string8  (const std::string& resource_path, std::size_t reserve_size = 0);
cpaf::containers::string8   to_string8  (const boost::filesystem::path& file_path, std::size_t reserve_size = 0);
std::string                 to_string   (const std::string& resource_path, std::size_t reserve_size = 0);
std::string                 to_string   (const boost::filesystem::path& file_path, std::size_t reserve_size = 0);

template<typename T, std::size_t EXTRA_CAPACITY = 0>
inline containers::dynamic_array<T, EXTRA_CAPACITY>
to_dynamic_array_raw(const std::string& resource_path, std::size_t reserve_size)
{
    auto is = create_istream_raw(resource_path, std::ios_base::binary);
    if (reserve_size == 0) {
        reserve_size = calc_reserve_size(is);
    }
    containers::dynamic_array<T, EXTRA_CAPACITY> arr(reserve_size);
    std::streamsize read_size = static_cast<std::streamsize>(reserve_size) - 1;
    while (!is.is().eof()) {
        char* write_pos = reinterpret_cast<char*>(&arr[arr.size()]);
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
    return arr;
}

template<typename T, std::size_t EXTRA_CAPACITY = 0>
inline containers::dynamic_array<T, EXTRA_CAPACITY>
to_dynamic_array_raw(const boost::filesystem::path& file_path, std::size_t reserve_size)
{
    return to_dynamic_array_raw<T>(file_path.string(), reserve_size);
}

template<typename T, std::size_t EXTRA_CAPACITY = 0>
inline containers::dynamic_array<T, EXTRA_CAPACITY>
to_dynamic_array(const std::string& resource_path, std::size_t reserve_size)
{
    auto is = create_istream(resource_path, std::ios_base::binary);
    if (reserve_size == 0) {
        reserve_size = calc_reserve_size(is);
    }
    containers::dynamic_array<T> arr(reserve_size);
    std::streamsize read_size = static_cast<std::streamsize>(reserve_size) - 1;
    while (!is.is().eof()) {
        char* write_pos = reinterpret_cast<char*>(&arr[arr.size()]);
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
    return arr;
}

template<typename T, std::size_t EXTRA_CAPACITY = 0>
inline containers::dynamic_array<T, EXTRA_CAPACITY>
to_dynamic_array(const boost::filesystem::path& file_path, std::size_t reserve_size)
{
    return to_dynamic_array<T>(file_path.string(), reserve_size);
}

// ----------------------------------------
// --- Write file convenience functions ---
// ----------------------------------------
template<typename T, std::size_t EXTRA_CAPACITY = 0>
inline void to_file(const std::string& resource_path, const containers::dynamic_array<T, EXTRA_CAPACITY>& dyn_array)
{
    auto os = create_ostream(resource_path);
    os.os().write(reinterpret_cast<const char*>(dyn_array.data()), static_cast<std::streamsize>(dyn_array.size()));
}

void            to_file     (const std::string& resource_path, cpaf::containers::string8 str);
void            to_file     (const std::string& resource_path, const char* sz_str);
void            to_file     (const std::string& resource_path, const char* sz_str, std::size_t size);
void            to_file     (const std::string& resource_path, const std::string& str);
void            to_file     (const std::string& resource_path, const std::string_view& str);
void            to_file     (const std::string& resource_path, const std::vector<std::string>& lines, bool append_newline = true);

template<typename T, std::size_t EXTRA_CAPACITY = 0>
inline void to_file(const boost::filesystem::path& file_path, const containers::dynamic_array<T, EXTRA_CAPACITY>& dyn_array)
{
    to_file<T, EXTRA_CAPACITY>(file_path.string(), dyn_array);
}

void            to_file     (const boost::filesystem::path& file_path, cpaf::containers::string8 str);
void            to_file     (const boost::filesystem::path& file_path, const char* sz_str);
void            to_file     (const boost::filesystem::path& file_path, const char* sz_str, std::size_t size);
void            to_file     (const boost::filesystem::path& file_path, const std::string& str);
void            to_file     (const boost::filesystem::path& file_path, const std::string_view& str);
void            to_file     (const boost::filesystem::path& file_path, const std::vector<std::string>& lines, bool append_newline = true);

// --------------------------------------------
// --- Append to file convenience functions ---
// --------------------------------------------
void            append_to   (const std::string& resource_path, cpaf::containers::string8 str);
void            append_to   (const std::string& resource_path, const char* sz_str);
void            append_to   (const std::string& resource_path, const std::string& str);
void            append_to   (const std::string& resource_path, const std::string_view& str);
void            append_to   (const std::string& resource_path, const std::vector<std::string>& lines, bool append_newline = true);

void            append_to   (const boost::filesystem::path& file_path, cpaf::containers::string8 str);
void            append_to   (const boost::filesystem::path& file_path, const char* sz_str);
void            append_to   (const boost::filesystem::path& file_path, const std::string& str);
void            append_to   (const boost::filesystem::path& file_path, const std::string_view& str);
void            append_to   (const boost::filesystem::path& file_path, const std::vector<std::string>& lines, bool append_newline = true);


} //END namespace cpaf::streams


#endif //CPAF_STREAMS_H


