#include "cpaf_compression_functions.h"
#include <iostream>
#include <zlib.h>
#include <filesystem/cpaf_file_directory_functions.h>
#include <boost/algorithm/string.hpp>

#include "compression_utils.h"

namespace fs = std::filesystem;
namespace al = boost::algorithm;

namespace cpaf { namespace compression {


// --------------------------------------
// --- Any compression type functions ---
// --------------------------------------

bool compress ( const std::filesystem::path& srcFilePath, const std::filesystem::path& dstFilePath  	)
{
    type ft = to_compression_type ( dstFilePath );
    switch ( ft )
    {
    case type::gz :  return gz_compress( srcFilePath, dstFilePath );
    default:
        cpaf::filesystem::force_copy_file( srcFilePath, dstFilePath );
        return false;
    }
}


bool uncompress	( const std::filesystem::path& srcFilePath, const std::filesystem::path& dstFilePath 	)
{
    type ft = to_compression_type ( srcFilePath );
    switch ( ft )
    {
    case type::gz :  return gz_uncompress( srcFilePath, dstFilePath );
    default:
        return false;
    }
}


void append_to ( const std::filesystem::path& filePath, const char* szString )
{
    type ft = to_compression_type ( filePath );
    switch ( ft )
    {
    case type::gz :  gz_append_to( filePath, szString ); break;
    default:
        cpaf::filesystem::append_to( filePath, szString );
    }
}

void append_to    ( const std::filesystem::path& filePath, const std::string& sString )
{
    append_to( filePath, sString.c_str() );
}

void writelines   ( const std::filesystem::path& filePath, const string_vec_t& vecLines, bool bAppendNewLine )
{
    type ft = to_compression_type ( filePath );
    switch ( ft )
    {
    case type::gz :  gz_writelines( filePath, vecLines, bAppendNewLine ); break;
    default:
        cpaf::filesystem::writelines( filePath, vecLines, bAppendNewLine );
    }
}

void appendlines  ( const std::filesystem::path& filePath, const string_vec_t& vecLines, bool bAppendNewLine )
{
    type ft = to_compression_type ( filePath );
    switch ( ft )
    {
    case type::gz :  gz_appendlines( filePath, vecLines, bAppendNewLine ); break;
    default:
        cpaf::filesystem::appendlines( filePath, vecLines, bAppendNewLine );
    }
}

void readlines    ( string_vec_t& vecLines, const std::filesystem::path& filePath, std::size_t maxLineLen )
{
    type ft = to_compression_type ( filePath );
    switch ( ft )
    {
    case type::gz :  gz_readlines( vecLines, filePath, maxLineLen ); break;
    default:
        cpaf::filesystem::readlines( vecLines, filePath, maxLineLen );
    }
}

// ------------------------------------------------
// --- gz (zlib) in-memory uncompress functions ---
// ------------------------------------------------
cpaf::containers::dynamic_uchar_array gz_uncompress_uchar(const unsigned char* source_buffer, size_t size)
{
    return gz_uncompress<unsigned char, 0>(source_buffer, size);
}
containers::dynamic_uchar_array gz_uncompress_uchar(const containers::dynamic_uchar_array& source_buffer)
{
    return gz_uncompress_uchar(source_buffer.data(), source_buffer.size());
}

cpaf::containers::dynamic_char_array gz_uncompress_char(const char* source_buffer, size_t size)
{
    return gz_uncompress<char, 0>(source_buffer, size);
}

containers::dynamic_char_array gz_uncompress_char(const containers::dynamic_char_array& source_buffer)
{
    return gz_uncompress_char(source_buffer.data(), source_buffer.size());
}

containers::string8 gz_uncompress_string8(const char* source_buffer, size_t size)
{
    namespace con = cpaf::containers;
    constexpr auto ZLIB_WINDOW_BITS = 15;
    constexpr auto ENABLE_ZLIB_GZIP = 32;
    constexpr auto zlib_chunk_size  = 0x4000; // Size of the memory chunk used by the zlib routines

    size_t dst_capacity = 8*size; // Initially reserve for 8 times the size of the compressed data
    con::string8 dst_buffer(dst_capacity);

    char* current_source_buffer_pos = const_cast<char*>(source_buffer);
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

containers::string8 gz_uncompress_string8(const containers::dynamic_uchar_array& source_buffer)
{
    return gz_uncompress_string8(reinterpret_cast<const char*>(source_buffer.data()), source_buffer.size());
}

containers::string8 gz_uncompress_string8(const containers::dynamic_char_array& source_buffer)
{
    return gz_uncompress_string8(source_buffer.data(), source_buffer.size());
}

containers::string8 gz_uncompress_string8(const containers::string8& source_buffer)
{
    return gz_uncompress_string8(source_buffer.data(), source_buffer.size());
}



// ---------------------------
// --- gz (zlib) functions ---
// ---------------------------

/**
Compress a single file to gz format using zlib. 
If \a dstFilePath is an existing directory the file is compressed to here with 
'.gz' appended to the filename.
*/
bool gz_compress(
	const std::filesystem::path& srcFilePath, 	///< [in] Path to source file
	const std::filesystem::path& dstFilePath  	///< [in] Path to destination file. Can be a directory name
) 
{
    if ( srcFilePath.empty() )  return false;
	bool bOk = true;
    namespace fs = std::filesystem;
    fs::path dstFile(dstFilePath);

    if ( fs::is_directory(dstFile) ) {
        dstFile /= srcFilePath.filename().string() + ".gz";
	}
	
	const unsigned iBlockSize = 8192;
	char buffer[iBlockSize];
    std::FILE*  fp 	= fopen		( srcFilePath.string().c_str(), "r"		);
	if ( !fp )	return false;
	gzFile zp 	= gzopen	( dstFile.string().c_str(), 	"w9" 	);
	if ( !zp )	return false;
	
	while (!feof(fp)) {
		unsigned iBytesRead = fread ( buffer,1, iBlockSize, fp );
		gzwrite  ( zp  , buffer, iBytesRead );		
	}
	fclose(fp);
	gzclose(zp);
	return bOk;
}


/**
Uncompress a single file from gz format using zlib. 
If \a dstFilePath is an existing directory the file is uncompressed to here with 
'.gz' removed from the filename.
*/
bool gz_uncompress(
	const std::filesystem::path& srcFilePath, 	///< [in] Path to source file
	const std::filesystem::path& dstFilePath  	///< [in] Path to destination file. Can be a directory name
) 
{
    if ( srcFilePath.empty() )  return false;
    bool bOk = true;
	namespace fs = std::filesystem;
	fs::path dstFile(dstFilePath);
	
	if ( fs::is_directory(dstFile) ) {
        dstFile /= srcFilePath.filename();
		dstFile.replace_extension("");
	}
	
	const unsigned iBlockSize = 8192;
	char buffer[iBlockSize];
	gzFile zp 	= gzopen	( srcFilePath.string().c_str(), "r" );
	if ( !zp )	return false;
    std::FILE*  fp 	= fopen		( dstFile.string().c_str(), 	"w"	);
	if ( !fp )	return false;
	
	while (!gzeof(zp)) {
// 								gzFile file, voidp buf, unsigned len
        int iBytesRead = gzread ( zp, buffer, iBlockSize );
        fwrite  ( buffer, 1, static_cast<std::size_t>(iBytesRead), fp );
	}
	fclose(fp);
	gzclose(zp);
	return bOk;
}

void gz_append_to  (const std::filesystem::path& filePath, const char* szString)
{
    gzFile file = gzopen( filePath.string().c_str(), "a" );
    gzputs (file, szString);
    gzclose(file);
}

void gz_append_to  (const std::filesystem::path& filePath, const std::string& sString)
{
    gz_append_to( filePath, sString.c_str() );
}

void gz_writelines (const std::filesystem::path& filePath, const string_vec_t& vecLines, bool bAppendNewLine)
{
    gzFile file = gzopen( filePath.string().c_str(), "w" );
    string_vec_t::const_iterator        it    = vecLines.begin();
    const string_vec_t::const_iterator  itEND = vecLines.end();
    if ( bAppendNewLine ) {
        for ( ; it != itEND; ++it ) gzputs ( file, (*it + "\n").c_str() );
    }
    else {
        for ( ; it != itEND; ++it ) gzputs ( file, it->c_str() );
    }
    gzclose(file);
}

void    gz_appendlines   ( const std::filesystem::path& filePath, const string_vec_t& vecLines, bool bAppendNewLine )
{
    gzFile file = gzopen( filePath.string().c_str(), "a" );
    string_vec_t::const_iterator        it    = vecLines.begin();
    const string_vec_t::const_iterator  itEND = vecLines.end();
    if ( bAppendNewLine ) {
        for ( ; it != itEND; ++it ) gzputs ( file, (*it + "\n").c_str() );
    }
    else {
        for ( ; it != itEND; ++it ) gzputs ( file, it->c_str() );
    }
    gzclose(file);
}


void  gz_readlines ( string_vec_t& vecLines, const std::filesystem::path& filePath, std::size_t maxLineLen )
{
    char* line = new char[maxLineLen];
    gzFile file = gzopen( filePath.string().c_str(), "r" );
    while ( !gzeof(file) && gzgets( file, line, static_cast<int>(maxLineLen) ) ) {
        vecLines.push_back( line );
    }
	delete[] line;
    gzclose(file);
}

// -------------------------------------------------
// --- Wrapping zlib functions in cpaf namespace ---
// -------------------------------------------------
// -- To avoid linker errors when other projects has it's own zlib like Mozilla.

bool gz_gets ( gzFile file, std::string& sLine, std::size_t maxLineLen)
{
    char* line = new char[maxLineLen];
    char* ret = gzgets( file, line, static_cast<int>(maxLineLen) );
    sLine = line;
	delete[] line;
    return Z_NULL != ret;
}

std::string gz_readline ( gzFile file, std::size_t maxLineLen)
{
    std::string sLine;
    gz_gets( file, sLine, maxLineLen );
    return sLine;
}


char* gz_gets ( gzFile file, char *buf, int maxLineLen)
{
    return gzgets( file, buf, maxLineLen );
}



gzFile gz_open (const std::string& sFileName, const std::string& sMode )
{
    return gzopen(sFileName.c_str(), sMode.c_str() );
}


int gz_close (gzFile file)
{
    return gzclose(file);
}

int gz_eof (gzFile file)
{
    return gzeof(file);
}






}} //END namespace cpaf::compression


