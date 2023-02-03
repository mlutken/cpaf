#include <iostream>
#include <unicode/cpaf_u16string_utils.h>
#include <unicode/cpaf_u16string_parse_numbers.h>
#include "token_parser.h"

using namespace cpaf::unicode::osops;
using namespace cpaf::semantic::osops;
using namespace cpaf::unicode;
using namespace std;

namespace cpaf::semantic
{

token_parser::token_parser(std::u16string&& str)
    : str_ptr_(std::make_unique<std::u16string>(std::move(str)))
    , string_begin_(str_ptr_->begin())
    , string_end_(str_ptr_->end())
{
}

token_parser::token_parser(const std::u16string& str)
    : raw_str_ptr_(&str),
      string_begin_(str.begin()),
      string_end_(str.end())
{
}


token_parser::token_parser(const std::u16string::const_iterator& begin, const std::u16string::const_iterator& end)
    : string_begin_(begin)
    , string_end_(end)
{
}

void token_parser::parse()
{
    parse(tokens_);
}

void token_parser::parse(std::vector<token>& tokens) const
{
    tokens.clear();
    parse_simple(tokens);
}

const u16string* token_parser::source() const
{
    if (str_ptr_) {
        return &(*str_ptr_);
    }
    return raw_str_ptr_;
}

char16_t token_parser::get(std::u16string::const_iterator pos) const
{
    return (string_begin_ <= pos && pos <  string_end_) ? *pos : 0x0;
}

void token_parser::parse_simple(std::vector<token>& tokens) const
{
    if (dbg_mode()) cerr <<  "Start parsing simple: '" << std::u16string(string_begin_, string_end_) << "'\n";
    auto cur = string_begin_;
    while (cur != string_end_) {
        tokens.emplace_back();
        token& tk = tokens.back();
///        tk.begin_ = cur;
///        tk.end_ = string_end_;
        cur = next_token(tk, cur);
        if (!tk.valid()) {
            tokens.pop_back();
            break;
        }
        if (dbg_mode()) cerr << tk << "\n";
        tk.dbg_u16str_ = tk.u16str(); // FIXMENM
    }
///    tokens.back().end_ = string_end_;

    if (dbg_mode()) {
        const auto len = std::distance(tokens.begin()->begin(), (tokens.end()-1)->end());
        cerr <<  "Done parsing len: " << len << "\n";
    }
}

/** Parse next token
@todo We might need to support + and - sign in front of digits.
*/
std::u16string::const_iterator token_parser::next_token(token& tk, std::u16string::const_iterator pos) const
{
    const auto prev_pos = pos - 1;
    if (prev_pos >= string_begin_ && chars16::is_space(*prev_pos)) {
        tk.attributes_.space_before(true);
    }
    tk.begin_ = pos;
    if (pos >= string_end_) {
        tk.begin_ = pos;
        tk.end_ = pos;  // Effectively sets token.invalid() true!
        return pos;
    }

    const auto c = *pos;
    if (chars16::is_digit(c)) {
        pos = next_number_token(tk, pos);
    }
    else if (token::is_white_space(c)) {
        pos = next_whitespace_token(tk, pos);
    }
    else if (is_special_start(pos)) {
        pos = next_special_token(tk, pos);
    }
    else if (token::is_separator(c)) {
        pos = next_separator_token(tk, pos);
    }
    else {
        pos = next_string_token(tk, pos);
    }

    if ( ((pos < string_end_) && chars16::is_space(*pos)) ||
         (pos >= string_end_)
        ) {
        tk.attributes_.space_after(true);
    }
    return pos;
}

std::u16string::const_iterator token_parser::next_whitespace_token(token& tk, std::u16string::const_iterator pos) const
{
    tk.attributes_.is_whitespace(true);
    const auto ch = *pos;
    if (chars16::is_blank(ch)) {
        tk.attributes_.is_blank(true);
    }
    if (chars16::is_space(ch)) {
        tk.attributes_.is_space(true);
    }
    ++pos;
    tk.end_ = pos;

    return pos;
}

std::u16string::const_iterator token_parser::next_special_token(token& tk, std::u16string::const_iterator pos) const
{
    tk.attributes_.is_special(true);
    bool done = false;
    while (!done) {
        ++pos;
        if ( pos == string_end_) {
            tk.attributes_.space_after(true);
            break;
        }
        if (is_special_end(pos)) {
            done = true;
            ++pos;
        }
    }
    if ( pos < string_end_ && token::is_white_space(*pos)) {
        tk.attributes_.space_after(true);
    }
    tk.end_ = pos;

    return pos;
}

std::u16string::const_iterator token_parser::next_separator_token(token& tk, std::u16string::const_iterator pos) const
{
    tk.attributes_.is_separator(true);
    ++pos;
    tk.end_ = pos;
    return pos;
}

std::u16string::const_iterator token_parser::next_string_token(token& tk, std::u16string::const_iterator pos) const
{
    tk.attributes_.is_string(true);
    bool done = false;
    while (!done) {
        ++pos;
        if ( pos == string_end_) {
            tk.attributes_.space_after(true);
            break;
        }
        const auto c = *pos;
        if (chars16::is_digit(c)) {
            tk.attributes_.has_digits(true);
            done = true;
        }
        else if (chars16::is_space(*pos)) {
            tk.attributes_.space_after(true);
            done = true;
        }
        else if (token::is_white_space(*pos)) {
            done = true;
        }
        else if (is_special_start(pos)) {
            done = true;
        }
        else if (token::is_separator(*pos)) {
            done = true;
        }
    }
    tk.end_ = pos;

    return pos;
}

std::u16string::const_iterator token_parser::next_number_token(token& tk, std::u16string::const_iterator pos) const
{
    tk.attributes_.is_number(true);
    bool done = false;
    while (!done) {
        ++pos;
        if ( pos == string_end_) {
            break;
        }
        const auto c = *pos;
        if ( chars16::is_comma(c) && chars16::is_digit(get(pos+1)) ) {
            tk.attributes_.has_comma(true);
        }
        else if ( chars16::is_dot(c) && chars16::is_digit(get(pos+1)) ) {
            tk.attributes_.has_dot(true);
        }
        else if (!chars16::is_digit(c)) {
            done = true;
        }
    }
    tk.end_ = pos;

// TODO: What about this ???
// If we have a comma or dot as last character we simply ignore it
//    if (is_comma_or_dot(*(tk.end_))) {
//        --tk.end_;
//    }

    return pos;
}

bool token_parser::is_special_start(std::u16string::const_iterator& pos) const
{
    const auto next_pos = pos+1;
    if (next_pos >= string_end_) {
        return false;
    }
    const bool res = (*pos == chars16::left_curly_bracket) && (*next_pos == chars16::left_curly_bracket);
    return res;
}

bool token_parser::is_special_end(std::u16string::const_iterator& pos) const
{
    const auto prev_pos = pos-1;
    if (prev_pos < string_begin_) {
        return false;
    }
    const bool res = (*prev_pos == chars16::right_curly_bracket) && (*pos == chars16::right_curly_bracket);
    return res;
}

// ----------------------------------------------
// --- Ostream ops: Debug u16string to stream ---
// ----------------------------------------------
/** \namespace cpaf::osops Debug u16string to stream
Mainly for debugging purposes.
Use for example like this:
\example
using namespace cpaf::u16string::osops;
\endexample
*/
namespace osops {
using namespace cpaf::unicode::osops;
std::ostream& operator<<(std::ostream& os, const cpaf::semantic::token& token)
{
    os << token.dbg_string();
//    std::u16string s(token.begin_, token.end_);
//    os << token.attributes_.dbg_string() << " : '";
//    os << cpaf::unicode::to_string(std::u16string(token.begin_, token.end_)) << "'";
    return os;
}
} // END namespace osops



} //end namespace cpaf::semantic
