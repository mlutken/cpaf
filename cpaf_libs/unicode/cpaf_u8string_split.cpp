#include "cpaf_u8string_split.h"

#include <locale>


namespace cpaf::unicode {

template <class CONTAINER = std::vector<std::string>>
CONTAINER split_raw(const std::string& to_split, const std::string& split_at_string)
{
    CONTAINER container;
    boost::split(container, to_split, boost::is_any_of(split_at_string));
    return container;
}


u8_vec splitv(const std::string& to_split, const std::string& split_at_string)
{
    return cpaf::unicode::split<u8_vec>(to_split, split_at_string);
}

u8_const_range_vec splitrv(const std::string& to_split, const std::string& split_at_string)
{
    return cpaf::unicode::split_raw<u8_const_range_vec>(to_split, split_at_string);
}




} // END namespace cpaf::unicode

