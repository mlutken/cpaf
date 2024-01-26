#include "compression_utils.h"

#include <iostream>
extern "C" {
#include <zlib.h>
}
#include <filesystem/cpaf_file_directory_functions.h>
#include <boost/algorithm/string.hpp>

namespace fs = std::filesystem;
namespace al = boost::algorithm;

namespace cpaf { namespace compression {



// --------------------------------------
// --- File type conversion functions ---
// --------------------------------------
std::string to_extension (type compression_type)
{
    switch (compression_type) {
        case type::gz: return ".gz"; break;
        case type::zip: return ".zip"; break;
        case type::bz2: return ".bz2"; break;
    default:
        ;
    }
    return "";
}

/** Detect file type from file-path's extension. */
type to_compression_type (const std::string& filePath)
{
    type ft = type::plain;
    if      ( al::ends_with(filePath, ".gz"  ))  ft = type::gz;
    else if ( al::ends_with(filePath, ".zip" ))  ft = type::zip;
    else if ( al::ends_with(filePath, ".bz2" ))  ft = type::bz2;
    return ft;
}


/** Detect file type from file-path's extension. */
type  to_compression_type (const std::filesystem::path& filePath)
{
    type ft = type::plain;
    if      ( filePath.extension() == ".gz" )   ft = type::gz;
    else if ( filePath.extension() == ".zip" )  ft = type::zip;
    else if ( filePath.extension() == ".bz2" )  ft = type::bz2;
    return ft;
}

/** Detect file type from data.
Looks for 'magic numbers'
For example for gzip the magic bytes the data must begin
with are: data[0] == 0x1F && data[1] == 0x8B .
@todo Only supports gzip detection currently. If not gzip we return normal_file.
*/
type to_compression_type(const char* data, size_t size)
{
    constexpr char gzip_magic_0 = static_cast<char>(0x1F);
    constexpr char gzip_magic_1 = static_cast<char>(0x8B);
    if (size > 1 && data[0] == gzip_magic_0 && data[1] == gzip_magic_1 ) {
        return type::gz;
    }
    return type::plain;
}

type to_compression_type(const unsigned char* data, size_t size)
{
    return to_compression_type(reinterpret_cast<const char*>(data), size);
}



}} //END namespace cpaf::compression


