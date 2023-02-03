#pragma once

#include <boost/algorithm/string.hpp>
#include <unicode/cpaf_unicode_common.h>


namespace cpaf::unicode {

template <class CONTAINER = std::vector<std::u16string>>
CONTAINER split(const std::u16string& to_split, const std::u16string& split_at_string)
{
    CONTAINER container;
    boost::split(container, to_split, boost::is_any_of(split_at_string));
    return container;
}

template <class CONTAINER = std::vector<std::u16string>>
CONTAINER split_trim(std::u16string_view to_split, std::u16string_view split_at_string)
{
    CONTAINER container;
    boost::split(container, to_split, boost::is_any_of(split_at_string));
    for (std::u16string& part : container) {
        boost::trim (part);
    }

    return container;
}

/** Split to standard std::vector<std::u16string> */
u16_vec splitv(const std::u16string& to_split, const std::u16string& split_at_string);

/** Split to a boost range vector: std::vector< boost::iterator_range<std::u16string::const_iterator> > */
u16_const_range_vec splitrv(const std::u16string& to_split, const std::u16string& split_at_string);

inline std::u16string join(const u16_vec& to_join, const std::u16string& join_with)
{
    return boost::join(to_join, join_with);
}

template<class CONTAINER>
std::u16string	join  (const CONTAINER& to_join, const std::u16string& join_with)
{
    const size_t container_size = to_join.size();
    if (container_size == 0) {
        return u"";
    }
    auto it = to_join.begin();
    std::u16string s = std::u16string(it->begin(), it->end());
    ++it;
    const auto it_end = to_join.end();
    for ( ; it != it_end; ++it ) {
        s.append(join_with);
        s.append(std::u16string(it->begin(), it->end()));
    }

    return s;
}



} // END namespace cpaf::unicode


