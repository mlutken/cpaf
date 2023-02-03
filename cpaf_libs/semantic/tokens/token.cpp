#include <iostream>
#include <unicode/cpaf_u16string_utils.h>
#include <unicode/cpaf_u16string_parse_numbers.h>
#include <semantic/primitives/pattern_matcher.h>
#include "token.h"

using namespace cpaf::unicode::osops;
using namespace cpaf::semantic::osops;
using namespace cpaf::unicode;
using namespace std;

namespace cpaf::semantic
{

string token::debug_str(const token::const_iterator& begin, const token::const_iterator& end)
{
    std::string s;
    for (auto it = begin; it != end; ++it) {
        const token& tk = *it;
        s.append(tk.str());
    }
    return s;
}

string token::debug_str(const vector& tokens)
{
    return debug_str(tokens.begin(), tokens.end());
}

string token::debug_str(const iter_pair& token_range)
{
    return debug_str(token_range.first, token_range.second);
}

bool token::is_separator(char16_t c)
{
    if (pattern_classifier_node::is_pattern_match_char(c)) {
        return false;
    }
    return chars16::is_separator(c);
}

bool token::is_separator(const std::u16string::const_iterator& it) const
{
    if (!position_valid(it)) return false;
    return token::is_separator(*it);
}

bool token::is_white_space() const
{
    if (len() == 1) {
        return is_white_space(*begin());
    }
    return false;
}

bool token::is_separator() const
{
    if (len() == 1) {
        return is_separator(begin());
    }
    return false;
}

bool token::is_separator_or_ws() const
{
    return is_white_space() || is_separator();
}

u16string_view token::u16view(std::u16string::const_iterator it_end) const
{
    if (it_end < begin() || it_end > end()) return mk_u16_view(end(), end());
    return mk_u16_view(begin(), it_end);
}

float token::float_value(const char16_t decimal_point, const char16_t thousands_separator) const
{
    constexpr float default_value = 0;
    return unicode::parse_float(begin(), end(), default_value, decimal_point, thousands_separator);
}

double token::double_value(const char16_t decimal_point, const char16_t thousands_separator) const
{
    constexpr double default_value = 0;
    return unicode::parse_double(begin(), end(), default_value, decimal_point, thousands_separator);
}


std::string token::dbg_string(bool print_attributes_info) const
{
    string s;
    if (print_attributes_info) {
        s += attributes_.dbg_string() + " : '";
    }
    else {
        s += "'";
    }
    s += cpaf::unicode::to_string(std::u16string(begin_, end_));
    s += print_attributes_info && attributes_.space_after() ? " '" : "'";
    return s;
}

// ----------------------
// --- Free functions ---
// ----------------------

std::string dbg_string(const token::vector& tokens)
{
    return dbg_string(tokens.begin(), tokens.end());
}

std::string dbg_string(const token::vector::const_iterator& begin,
                       const token::vector::const_iterator& end)
{
    string s;
    if (begin >= end) {
        return "";
    }
    auto it = begin;
    s.append(it->dbg_string(false));
    ++it;
    while (it != end) {
        s.append(", ");
        s.append(it->dbg_string(false));
        ++it;
    }
    return s;
}

std::u16string_view u16_view (const token::vector::const_iterator& begin,
                              const token::vector::const_iterator& end)
{
    if (begin == end) {
        return std::u16string_view(&*(begin->begin()), 0);
    }
    //// const auto len = static_cast<token::vector::size_type>(end->end() - begin->begin());
    const auto len = static_cast<token::vector::size_type>((end-1)->end() - begin->begin());
    return std::u16string_view(&*(begin->begin()), len);
}

u16string_view u16_view(const token::iter_pair& tk_range)
{
    return u16_view(tk_range.first, tk_range.second);
}

u16string_view u16_view(const token& tk)
{
    return tk.u16view();
}


const_u16_iter_pair mk_u16_iterator_pair(const token::vector::const_iterator& begin,
                                         const token::vector::const_iterator& end)
{
    if (begin == end) {
        return {begin->begin(), begin->begin()}; // Empty range!
    }
    return {begin->begin(), (end-1)->end()};
}

/** Return first token iterator from begin that is a whitespace.
 *
 * Search forward to get the first token that is a whitespace. In case no such is found
 * we return the end iterator.
*/
token::vector::const_iterator
find_next_white_space (token::vector::const_iterator begin,
                       const token::vector::const_iterator& end)
{
    if (begin < end && begin->attributes().is_whitespace()) {
        return begin;
    }
    ++begin;
    while (begin < end && !(begin->attributes().is_whitespace())){
        ++begin;
    }

    return begin;
}


//std::string to_string(const std::vector<token>::const_iterator& begin, const std::vector<token>::const_iterator& end)
//{
//    return unicode::to_string(to_u16string(begin, end));
//}


} //end namespace cpaf::semantic
