#ifndef CPAF_TOKEN_PARSER_H
#define CPAF_TOKEN_PARSER_H

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
#include <semantic/tokens/token.h>

namespace cpaf::semantic
{

class token_parser
{
public:
    using size_type                 = std::vector<token>::size_type;
    using const_iterator            = std::vector<token>::const_iterator;
    using const_reverse_iterator    = std::vector<token>::const_reverse_iterator;

    explicit                    token_parser  () = default;
    explicit                    token_parser  (std::u16string&& str);
    explicit                    token_parser  (const std::u16string& str);
    explicit                    token_parser  (const std::u16string::const_iterator& begin, const std::u16string::const_iterator& end);

    void                        parse   ();
    void                        parse   (std::vector<token>& tokens) const;

    const_iterator              begin   () const  { return tokens_.begin(); }
    const_iterator              end     () const  { return tokens_.end();   }
    const_reverse_iterator      rbegin  () const  { return tokens_.rbegin();}
    const_reverse_iterator      rend    () const  { return tokens_.rend();  }
    size_type                   size    () const  { return tokens_.size();  }
    bool                        empty   () const  { return tokens_.empty(); }

    const std::vector<token>&   tokens  () const  { return tokens_; }
    const std::u16string*       source  () const;

    void                        dbg_mode(bool dbg_mode) { dbg_mode_ = dbg_mode;}
    bool                        dbg_mode() const        { return dbg_mode_;}
private:
    char16_t                    get    (std::u16string::const_iterator pos) const;

    void                            parse_simple            (std::vector<token>& tokens) const;
    std::u16string::const_iterator  next_token              (token& tk, std::u16string::const_iterator pos) const;
    std::u16string::const_iterator  next_whitespace_token   (token& tk, std::u16string::const_iterator pos) const;
    std::u16string::const_iterator  next_special_token      (token& tk, std::u16string::const_iterator pos) const;
    std::u16string::const_iterator  next_separator_token    (token& tk, std::u16string::const_iterator pos) const;
    std::u16string::const_iterator  next_string_token       (token& tk, std::u16string::const_iterator pos) const;
    std::u16string::const_iterator  next_number_token       (token& tk, std::u16string::const_iterator pos) const;
    bool                            is_special_start        (std::u16string::const_iterator& pos) const;
    bool                            is_special_end          (std::u16string::const_iterator& pos) const;

    std::vector<token>                      tokens_;
    std::unique_ptr<std::u16string>         str_ptr_;       // In case we get initilized with a temporary rvalue reference (&& constructor). Here we handle the source string memory/lifetime.
    const std::u16string*                   raw_str_ptr_ = nullptr;   // In case we get initialized with const std::u16string (lvalue) we use this. Here we assume the caller will handle the source string life time and we only store a pointer
    const std::u16string::const_iterator    string_begin_;
    const std::u16string::const_iterator    string_end_;

//    std::vector<token>::size_type           cur_token_index_;
    bool                                    dbg_mode_ = false;
};


namespace osops {
    std::ostream& operator<< (std::ostream& os, const cpaf::semantic::token& token);
} // END namespace osops

} //end namespace cpaf::semantic


#endif //CPAF_TOKEN_PARSER_H

