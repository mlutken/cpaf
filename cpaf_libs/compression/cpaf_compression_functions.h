#ifndef CPAF_COMPRESSION_FUNCTIONS_H
#define CPAF_COMPRESSION_FUNCTIONS_H

 
#include <string>
#include <vector>
#include <variant>
#include <cstdio>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <zlib.h>
#include <bzlib.h>
#include <containers/cpaf_container_utils.h>
#include <containers/cpaf_dynamic_array.hpp>
#include <containers/cpaf_string8.h>

namespace cpaf { namespace compression {

/** \unit cpaf_compression_functions.hpp
! Functions for primitive compression support. 
\author Martin Lutken 
*/


// -----------------------------
// --- Types and Definitions ---
// -----------------------------
enum class type { plain, gz, zip, bz2 };
typedef std::vector<std::string>	string_vec_t;

// --------------------------------------
// --- File type conversion functions ---
// --------------------------------------
std::string to_extension(type compression_type);
type    to_compression_type (const std::string& filePath);
type    to_compression_type (const boost::filesystem::path& filePath);
type    to_compression_type (const char* data, size_t size);
type    to_compression_type (const unsigned char* data, size_t size);


// ---------------------------------------------------------
// --- Generic file class (handles any compression type) ---
// ---------------------------------------------------------
/** File class that can read normal as well as compressed
    files.
    The maximum line lenght to read is set with max_readline_size and
    by default this is set to DEFAULT_MAX_LINE_LENGHT = 8096.
    It can be changed both via constructor and member function
    max_readline_size().
 */
struct file
{
    static constexpr int64_t DEFAULT_MAX_LINE_LENGHT = 8096;

    file() = default;
    explicit file(const std::string& file_path);
    explicit file(const boost::filesystem::path& file_path);
    file(const std::string& file_path, const std::string& mode);
    file(const boost::filesystem::path&file_path, const std::string& mode);
    file(const std::string& file_path, const std::string& mode, std::size_t max_readline_size);
    file(const boost::filesystem::path& file_path, const std::string& mode, std::size_t max_readline_size);
    ~file() noexcept;

    // ------------------------------------------------
    // --- file: Info functions, setter and getters ---
    // ------------------------------------------------
    type                file_type           () const { return file_type_; }
    void                max_readline_size   (std::size_t max_readline_size) noexcept;
    std::size_t         max_readline_size   () const noexcept { return max_readline_size_; }
    std::string_view    file_path           () const noexcept { return file_path_; }
    std::string_view    mode                () const noexcept { return mode_; }

    // -----------------------------
    // --- file: state functions ---
    // -----------------------------
    bool                open                (const std::string& mode) noexcept;
    bool                open                (const std::string& file_path, const std::string& mode) noexcept;
    bool                open                (const boost::filesystem::path& file_path, const std::string& mode) noexcept;
    bool                is_open             () const noexcept {  return is_open_; }
    bool                close               () noexcept;
    bool                eof                 () const noexcept;


    // ----------------------------
    // --- file: Read functions ---
    // ----------------------------
    char*               gets                (char* buf) const noexcept;
    char*               gets                () const noexcept;
    std::string_view    readline            () const noexcept;
    std::size_t         read                (void* buffer, std::size_t size, std::size_t count);

    // -----------------------------
    // --- file: Write functions ---
    // -----------------------------
    bool                puts                (const char* buf) const noexcept;
    bool                puts                (const std::string& str) const noexcept;
    std::size_t         write               (const char* buf, std::size_t bytes_to_write) const noexcept;
    std::size_t         write               (const std::string& str, std::size_t start_pos, std::size_t bytes_to_write) const noexcept;

private:
    void                allocate_read_buffer();
    void                deallocate_read_buffer();


    using handle_type = std::variant<std::FILE*, gzFile>;
    std::string     file_path_;
    std::string     mode_;
    std::size_t     max_readline_size_ = DEFAULT_MAX_LINE_LENGHT;
    handle_type     handle_;
    type        file_type_;
    char*           read_buffer_ = nullptr;
    bool            is_open_ = false;
};



// --------------------------------------
// --- Any compression type functions ---
// --------------------------------------

bool        compress    ( const boost::filesystem::path& srcFilePath, const boost::filesystem::path& dstFilePath  	);
bool        uncompress	( const boost::filesystem::path& srcFilePath, const boost::filesystem::path& dstFilePath 	);

void        append_to   ( const boost::filesystem::path& filePath, const char* szString );
void        append_to   ( const boost::filesystem::path& filePath, const std::string& sString );
void        writelines  ( const boost::filesystem::path& filePath, const string_vec_t& vecLines, bool bAppendNewLine = true );
void        appendlines ( const boost::filesystem::path& filePath, const string_vec_t& vecLines, bool bAppendNewLine = true );
void        readlines   ( string_vec_t& vecLines, const boost::filesystem::path& filePath, std::size_t maxLineLen = 16384 );

// ------------------------------------------------
// --- gz (zlib) in-memory uncompress functions ---
// ------------------------------------------------
cpaf::containers::dynamic_uchar_array   gz_uncompress_uchar  (const unsigned char* source_buffer, size_t size);
cpaf::containers::dynamic_uchar_array   gz_uncompress_uchar  (const cpaf::containers::dynamic_uchar_array& source_buffer);
cpaf::containers::dynamic_char_array    gz_uncompress_char   (const char* source_buffer, size_t size);
cpaf::containers::dynamic_char_array    gz_uncompress_char   (const cpaf::containers::dynamic_char_array& source_buffer);
cpaf::containers::string8               gz_uncompress_string8(const char* source_buffer, size_t size);
cpaf::containers::string8               gz_uncompress_string8(const cpaf::containers::dynamic_uchar_array& source_buffer);
cpaf::containers::string8               gz_uncompress_string8(const cpaf::containers::dynamic_char_array& source_buffer);
cpaf::containers::string8               gz_uncompress_string8(const cpaf::containers::string8& source_buffer);

// --------------------------------
// --- gz (zlib) file functions ---
// --------------------------------
bool        gz_compress 	( const boost::filesystem::path& srcFilePath, const boost::filesystem::path& dstFilePath  	);
bool        gz_uncompress	( const boost::filesystem::path& srcFilePath, const boost::filesystem::path& dstFilePath 	);

void        gz_append_to    ( const boost::filesystem::path& filePath, const char* szString );
void        gz_append_to    ( const boost::filesystem::path& filePath, const std::string& sString );
void        gz_writelines   ( const boost::filesystem::path& filePath, const string_vec_t& vecLines, bool bAppendNewLine = true );
void        gz_appendlines  ( const boost::filesystem::path& filePath, const string_vec_t& vecLines, bool bAppendNewLine = true );
void        gz_readlines    ( string_vec_t& vecLines, const boost::filesystem::path& filePath, std::size_t maxLineLen = 16384 );


// -------------------------------------------------
// --- Wrapping zlib functions in cpaf namespace ---
// -------------------------------------------------
bool        gz_gets         ( gzFile file, std::string& sLine, std::size_t maxLineLen );
std::string gz_readline     ( gzFile file, std::size_t maxLineLen );
char*       gz_gets         ( gzFile file, char *buf, int maxLineLen );
gzFile      gz_open         ( const std::string& sFileName, const std::string& sMode );
int         gz_close        ( gzFile file );
int         gz_eof          ( gzFile file );

// ------------------------------------
// --- gz (zlib) template functions ---
// ------------------------------------

/**
  In memory gz uncompression

  @return a dynamic_array with the uncompressed data.
*/
template <typename CHAR_TYPE, std::size_t EXTRA_CAPACITY = 0>
cpaf::containers::dynamic_array<CHAR_TYPE, EXTRA_CAPACITY>
gz_uncompress(const CHAR_TYPE* source_buffer, size_t size)
{
    namespace con = cpaf::containers;
    constexpr auto ZLIB_WINDOW_BITS = 15;
    constexpr auto ENABLE_ZLIB_GZIP = 32;
    constexpr auto zlib_chunk_size  = 0x4000; // Size of the memory chunk used by the zlib routines

    size_t dst_capacity = 8*size; // Initially reserve for 8 times the size of the compressed data
    con::dynamic_array<CHAR_TYPE, EXTRA_CAPACITY> dst_buffer(dst_capacity);

    CHAR_TYPE* current_source_buffer_pos = const_cast<CHAR_TYPE*>(source_buffer);
    const unsigned int source_buffer_size = static_cast<unsigned int>(size);

    z_stream strm   = {};
    strm.zalloc     = Z_NULL;
    strm.zfree      = Z_NULL;
    strm.opaque     = Z_NULL;
    strm.next_in    = reinterpret_cast<unsigned char*>(current_source_buffer_pos);
    strm.avail_in   = source_buffer_size;
    inflateInit2 (&strm, ZLIB_WINDOW_BITS | ENABLE_ZLIB_GZIP);

    while (1) {
        unsigned int bytes_read = static_cast<unsigned int>( con::buffer_read_forward(current_source_buffer_pos, source_buffer, size, zlib_chunk_size) );
        if (bytes_read == 0) {
            break;
        }
        strm.avail_in = bytes_read;
        do {
            dst_buffer.reserve_for_append(zlib_chunk_size);
            unsigned bytes_decompressed;
            strm.avail_out = zlib_chunk_size;
            strm.next_out = reinterpret_cast<unsigned char*>(dst_buffer.data_append_ptr());
            auto zlib_status = inflate (& strm, Z_NO_FLUSH);
            switch (zlib_status) {
            case Z_OK:
            case Z_STREAM_END:
            case Z_BUF_ERROR:
                break;
            default:
                inflateEnd (& strm);
                fprintf (stderr, "ERROR: Gzip error %d in 'input data'.\n", zlib_status);
                dst_buffer.clear();
                return dst_buffer;
            }
            bytes_decompressed = zlib_chunk_size - strm.avail_out;
            dst_buffer.size_force(dst_buffer.size()+bytes_decompressed);
        }
        while (strm.avail_out == 0);
        strm.next_in = reinterpret_cast<unsigned char*>(current_source_buffer_pos);
    }
    return dst_buffer;
}

/**
  In memory gz uncompression

  @return a dynamic_array with the uncompressed data.
*/
template <typename CHAR_TYPE, std::size_t EXTRA_CAPACITY = 0>
cpaf::containers::dynamic_array<CHAR_TYPE, EXTRA_CAPACITY>
gz_uncompress(const cpaf::containers::dynamic_array<CHAR_TYPE, EXTRA_CAPACITY>& source_buffer)
{
    return gz_uncompress<CHAR_TYPE, EXTRA_CAPACITY>(source_buffer.data(), source_buffer.size());
}


}} //END namespace cpaf::compression


#endif //CPAF_COMPRESSION_FUNCTIONS_H


