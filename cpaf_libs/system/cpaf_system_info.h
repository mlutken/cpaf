#pragma once

#include <limits.h>
#include <stdint.h>
#include <bit>
#include <string_view>
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
    END
};

consteval bool is_little_endian () { return std::endian::native == std::endian::little; }
consteval bool is_big_endian    () { return !is_little_endian(); }

inline consteval std::string_view endianess_str()
{
    if (is_little_endian()) {
        return "little endian";
    }
    return "big endian";
}

inline consteval platform_type_t platform_type()
{
    return static_cast<platform_type_t>(CPAF_PLATFORM_TYPE_ID);
}


#if CHAR_BIT != 8
#error "unsupported char size"
#endif

enum
{
    O32_LITTLE_ENDIAN = 0x03020100ul,
    O32_BIG_ENDIAN = 0x00010203ul,
    O32_PDP_ENDIAN = 0x01000302ul,      /* DEC PDP-11 (aka ENDIAN_LITTLE_WORD) */
    O32_HONEYWELL_ENDIAN = 0x02030001ul /* Honeywell 316 (aka ENDIAN_BIG_WORD) */
};

static const union { unsigned char bytes[4]; uint32_t value; } o32_host_order =
    { { 0, 1, 2, 3 } };

#define O32_HOST_ORDER (o32_host_order.value)


}} //END namespace cpaf::system


