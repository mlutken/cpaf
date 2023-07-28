#include "cpaf_stream_utils.h"

#include <cpaf_libs/unicode/cpaf_u8string_utils.h>

namespace cu = cpaf::unicode;

std::string protocol_from_uri(const std::string& uri)
{
    return cu::substring_between(uri, "", ":");
}
