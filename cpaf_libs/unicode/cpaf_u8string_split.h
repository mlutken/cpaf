#pragma once

#include <boost/algorithm/string.hpp>
#include <unicode/cpaf_unicode_common.h>


namespace cpaf::unicode {


template <class CONTAINER = std::vector<std::string_view>>
CONTAINER split(std::string_view to_split, std::string_view split_at_string)
{
    CONTAINER container;
    boost::split(container, to_split, boost::is_any_of(split_at_string));
    return container;
}

template <class CONTAINER = std::vector<std::string>>
CONTAINER split_trim(std::string_view to_split, std::string_view split_at_string)
{
    CONTAINER container;
    boost::split(container, to_split, boost::is_any_of(split_at_string));
    for (std::string& part : container) {
        boost::trim (part);
    }

    return container;
}

/** Split to standard std::vector<std::string> */
u8_vec splitv(const std::string& to_split, const std::string& split_at_string);

/** Split to a boost range vector: std::vector< boost::iterator_range<std::string::const_iterator> > */
u8_const_range_vec splitrv(const std::string& to_split, const std::string& split_at_string);

inline std::string join(const u8_vec& to_join, const std::string& join_with)
{
    return boost::join(to_join, join_with);
}

template<class CONTAINER>
std::string	join  (const CONTAINER& to_join, const std::string& join_with)
{
    const size_t container_size = to_join.size();
    if (container_size == 0) {
        return "";
    }
    auto it = to_join.begin();
    std::string s = std::string(it->begin(), it->end());
    ++it;
    const auto it_end = to_join.end();
    for ( ; it != it_end; ++it ) {
        s.append(join_with);
        s.append(std::string(it->begin(), it->end()));
    }

    return s;
}



} // END namespace cpaf::unicode

