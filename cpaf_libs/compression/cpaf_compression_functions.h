#pragma once
 
#include <string>
#include <vector>
#include <variant>
#include <cstdio>
#include <filesystem>
#include <boost/filesystem/convenience.hpp>
extern "C" {
#include <zlib.h>
}

#include <containers/cpaf_container_utils.h>
#include <containers/cpaf_dynamic_array.hpp>
#include <containers/cpaf_string8.h>

//class gzFile;

namespace cpaf { namespace compression {

/** \unit cpaf_compression_functions.hpp
! Functions for primitive compression support. 
\author Martin Lutken 
*/

typedef std::vector<std::string>	string_vec_t;

// --------------------------------------
// --- Any compression type functions ---
// --------------------------------------

bool        compress    ( const std::filesystem::path& srcFilePath, const std::filesystem::path& dstFilePath  	);
bool        uncompress	( const std::filesystem::path& srcFilePath, const std::filesystem::path& dstFilePath 	);

void        append_to   ( const std::filesystem::path& filePath, const char* szString );
void        append_to   ( const std::filesystem::path& filePath, const std::string& sString );
void        writelines  ( const std::filesystem::path& filePath, const string_vec_t& vecLines, bool bAppendNewLine = true );
void        appendlines ( const std::filesystem::path& filePath, const string_vec_t& vecLines, bool bAppendNewLine = true );
void        readlines   ( string_vec_t& vecLines, const std::filesystem::path& filePath, std::size_t maxLineLen = 16384 );

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
bool        gz_compress 	( const std::filesystem::path& srcFilePath, const std::filesystem::path& dstFilePath  	);
bool        gz_uncompress	( const std::filesystem::path& srcFilePath, const std::filesystem::path& dstFilePath 	);

void        gz_append_to    ( const std::filesystem::path& filePath, const char* szString );
void        gz_append_to    ( const std::filesystem::path& filePath, const std::string& sString );
void        gz_writelines   ( const std::filesystem::path& filePath, const string_vec_t& vecLines, bool bAppendNewLine = true );
void        gz_appendlines  ( const std::filesystem::path& filePath, const string_vec_t& vecLines, bool bAppendNewLine = true );
void        gz_readlines    ( string_vec_t& vecLines, const std::filesystem::path& filePath, std::size_t maxLineLen = 16384 );


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

