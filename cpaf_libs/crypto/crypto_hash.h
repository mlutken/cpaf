#pragma once
#include <string>
#include <array>

namespace cpaf::crypto {

/** \unit crypto_hash.h
! Common cryptographic hashing functions.
\author Martin Lutken
*/

std::string sha1    (const std::string& text);
std::string md5     (const std::string_view& text);

} //END namespace cpaf::crypto


