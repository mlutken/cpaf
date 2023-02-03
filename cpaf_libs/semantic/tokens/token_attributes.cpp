#include <unicode/cpaf_u16string_utils.h>
#include "token_attributes.h"

namespace cpaf::semantic
{

std::string token_attributes::dbg_string() const
{
    std::string s;
    s += space_before   () ?  "_" : " ";
    s += is_separator   () ?  "-" : "";
    s += is_string      () ?  "s" : "";
    s += is_number      () ?  "n" : "";
    s += space_after    () ?  "_" : " ";
    return s;
}



} //end namespace cpaf::semantic


