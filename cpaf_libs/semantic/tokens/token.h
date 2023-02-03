#ifndef CPAF_TOKEN_H
#define CPAF_TOKEN_H

#include <string>
#include <string_view>
#include <memory>
#include <array>
//#include <variant>
#include <map>
#include <bitset>
//#include <unordered_map>

//#include <boost/algorithm/string.hpp>
//#include <sparsehash/dense_hash_map>

#include <containers/cpaf_string8.h>
#include <unicode/cpaf_u16string_utils.h>
#include <unicode/cpaf_unicode_chars.hpp>
#include <semantic/tokens/token_attributes.h>
#include <semantic/primitives/char_match.h>

namespace cpaf::semantic
{

struct token
{
    using vector                    = std::vector<token>;
    using size_type                 = std::vector<token>::size_type;
    using const_iterator            = std::vector<token>::const_iterator;
    using const_reverse_iterator    = std::vector<token>::const_reverse_iterator;
    using iter_pair                 = std::pair<const_iterator, const_iterator>;
    using iter_pair_vec             = std::vector<iter_pair>;

    static  std::string         debug_str           (const const_iterator& begin, const const_iterator& end);
    static  std::string         debug_str           (const vector& tokens);
    static  std::string         debug_str           (const iter_pair& token_range);

    static bool                 is_white_space      (char16_t c) { return cpaf::unicode::chars16::is_white_space(c); }
    static bool                 is_separator        (char16_t c);

    bool                        is_separator        (const std::u16string::const_iterator& it) const;
    bool                        is_white_space      () const;
    bool                        is_separator        () const;
    bool                        is_separator_or_ws  () const;

    bool                        valid               () const    { return begin_ < end_; }
    bool                        position_valid      (const std::u16string::const_iterator& it) const  { return (begin_ <= it) && (it < end_); }
    token_attributes&           attributes          ()          { return attributes_; }
    const token_attributes&     attributes          () const    { return attributes_; }
    std::u16string_view         u16view             () const { return std::u16string_view(&*begin_, static_cast<vector::size_type>(end_ - begin_));}
    std::u16string_view         u16view             (std::u16string::const_iterator it_end) const;
    void                        begin               (const std::u16string::const_iterator& it) { begin_ = it; }
    void                        end                 (const std::u16string::const_iterator& it) { end_ = it;   }
    float                       float_value         (const char16_t decimal_point = unicode::chars16::dot,
                                                     const char16_t thousands_separator = unicode::chars16::comma) const;
    double                      double_value        (const char16_t decimal_point = unicode::chars16::dot,
                                                     const char16_t thousands_separator = unicode::chars16::comma) const;
    float                       number_value        (const char16_t decimal_point = unicode::chars16::dot,
                                                     const char16_t thousands_separator = unicode::chars16::comma) const
                                                          { return float_value(decimal_point, thousands_separator); }
    std::u16string              u16str              () const { return std::u16string(begin_, end_);}
    std::string                 str                 () const { return cpaf::unicode::to_string(u16str());}
    std::string                 dbg_string          (bool print_attributes_info = true) const;

    size_t                      len                 () const    { const auto l = std::distance(begin(), end()); return l >= 0 ? static_cast<size_t>(l) : 0;  }
    const std::u16string::const_iterator&   begin               () const { return begin_;   }
    const std::u16string::const_iterator&   end                 () const { return end_;     }

    std::u16string::const_iterator  begin_;
    std::u16string::const_iterator  end_;
    token_attributes                attributes_;
    std::u16string                  dbg_u16str_;
};

// ----------------------
// --- Free functions ---
// ----------------------

std::string         dbg_string      (const token::vector& tokens);
std::string         dbg_string      (const token::vector::const_iterator& begin,
                                     const token::vector::const_iterator& end);
std::u16string_view u16_view        (const token& tk);
std::u16string_view u16_view        (const token::vector::const_iterator& begin,
                                     const token::vector::const_iterator& end);
std::u16string_view u16_view        (const token::iter_pair& tk_range);

unicode::const_u16_iter_pair
mk_u16_iterator_pair                (const token::vector::const_iterator& begin,
                                     const token::vector::const_iterator& end);


token::vector::const_iterator
find_next_white_space               (token::vector::const_iterator begin,
                                     const token::vector::const_iterator& end);

//std::string to_string (const std::vector<token>::const_iterator& begin,
//                       const std::vector<token>::const_iterator& end);


namespace osops {
    std::ostream& operator<< (std::ostream& os, const cpaf::semantic::token& token);
} // END namespace osops

} //end namespace cpaf::semantic


#endif //CPAF_TOKEN_H

