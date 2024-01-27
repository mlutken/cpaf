#pragma once
 
#include <string>
#include <vector>
#include <variant>
#include <cstdio>
#include <filesystem>
#include <boost/filesystem/convenience.hpp>

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

// --------------------------------------
// --- File type conversion functions ---
// --------------------------------------
std::string     to_extension        (type compression_type);
type            to_compression_type (const std::string& filePath);
type            to_compression_type (const std::filesystem::path& filePath);
type            to_compression_type (const char* data, size_t size);
type            to_compression_type (const unsigned char* data, size_t size);
bool            detect_is_zip_file  (const std::filesystem::path& file_path);




}} //END namespace cpaf::compression

