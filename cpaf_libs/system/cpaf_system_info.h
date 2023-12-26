#pragma once
 
#include <bit>
#include <config/cpaf_platform_definitions.h>

namespace cpaf { namespace system {

enum class platform_type_t {
    unknown         = 0,
    desktop         = CPAF_PLATFORM_TYPE_ID_DESKTOP,
    embedded        = CPAF_PLATFORM_TYPE_ID_EMBEDDED,
    desktop_web     = CPAF_PLATFORM_TYPE_ID_DESKTOP_WEB,
    mobile          = CPAF_PLATFORM_TYPE_ID_MOBILE,
    tablet          = CPAF_PLATFORM_TYPE_ID_TABLET,
    tablet_web      = CPAF_PLATFORM_TYPE_ID_TABLET_WEB,
    mobile_web      = CPAF_PLATFORM_TYPE_ID_MOBILE_WEB,
    COUNT
};

consteval bool is_little_endian () { return std::endian::native == std::endian::little; }
consteval bool is_big_endian    () { return !is_little_endian(); }

inline constexpr platform_type_t platform_type()
{
    return static_cast<platform_type_t>(CPAF_PLATFORM_TYPE_ID);
}



}} //END namespace cpaf::system


