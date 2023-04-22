#pragma once
 
#include <bit>

namespace cpaf { namespace system {

consteval bool is_little_endian () { return std::endian::native == std::endian::little; }
consteval bool is_big_endian    () { return !is_little_endian(); }



}} //END namespace cpaf::system


