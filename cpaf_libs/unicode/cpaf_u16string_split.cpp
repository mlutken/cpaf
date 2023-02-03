#include "cpaf_u16string_split.h"

#include <locale>


namespace cpaf::unicode {

u16_vec splitv(const std::u16string& to_split, const std::u16string& split_at_string)
{
    return split<u16_vec>(to_split, split_at_string);
}

u16_const_range_vec splitrv(const std::u16string& to_split, const std::u16string& split_at_string)
{
    return split<u16_const_range_vec>(to_split, split_at_string);
}




} // END namespace cpaf::unicode

