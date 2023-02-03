#pragma once
#include <string>
#include <vector>
#include <boost/range.hpp>

namespace cpaf::unicode {
using const_u8_iter_pair    = std::pair<std::string::const_iterator, std::string::const_iterator>;
using u8_const_range_vec    = std::vector<const_u8_iter_pair>;
using u8_range              = boost::iterator_range<std::string::iterator>;
using u8_const_range        = boost::iterator_range<std::string::const_iterator>;
using u8_range_vec          = std::vector<u8_range>;
using u8_vec                = std::vector<std::string>;
using u8_index_pair         = std::pair<std::string::size_type, std::string::size_type>;

using const_u16_iter_pair   = std::pair<std::u16string::const_iterator, std::u16string::const_iterator>;
using u16_const_range_vec   = std::vector<const_u16_iter_pair>;
using u16_range             = boost::iterator_range<std::u16string::iterator>;
using u16_range_vec         = std::vector<u16_range>;
using u16_vec               = std::vector<std::u16string>;
using u16_index_pair        = std::pair<std::u16string::size_type, std::u16string::size_type>;

enum class do_trim { no, yes };
enum class post_op { none, trim, simplify_ws, simplify_title, simplify_title_full  };



} // END namespace cpaf::unicode

