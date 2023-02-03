#include <iostream>
#include <unicode/cpaf_u16string_utils.h>
#include "pattern_matcher_node.h"

using namespace cpaf::unicode::osops;
using namespace cpaf::unicode;
using namespace std;

namespace cpaf::semantic
{
/** Test if char is one of the special pattern match chars
 *  These are: #, ¤, ~, , *
 * */
bool pattern_classifier_node::is_pattern_match_char(const char16_t c)
{
    return c == digit || c == number_char || c == any_char || c == wildcard;
}

pattern_classifier_node* pattern_classifier_node::append_char(char16_t c, bool is_last)
{
    pattern_classifier_node* new_node_ptr = &(*plain_char_children_)[c];
    pattern_classifier_node& new_node = *new_node_ptr;
    new_node.c_ = c;
    new_node.level_ = level_ + 1;
    new_node.parent_= this;
    if (is_last) {
        new_node.finalize_complete_string();
    }
    else {
        new_node.ensure_plain_children();
    }
    return new_node_ptr;
}

/** Finalizes after adding the last char of a complete
 * string. */
void pattern_classifier_node::finalize_complete_string()
{
    get_string_value(complete_string_);
}

const pattern_classifier_node* pattern_classifier_node::step_forward(
        char16_t c,
        try_order& try_from_index,
        matcher_map::const_iterator& /*try_from_iterator*/) const
{
    const pattern_classifier_node* next_node_ptr = nullptr;
    while (try_from_index != try_order::try_end) {
        next_node_ptr = try_step_forward(c, try_from_index);
        if (next_node_ptr) {
            return next_node_ptr;
        }
        ++try_from_index;
    }
    const auto parent_wildcard_node = find_parent_wildcard();
    if (parent_wildcard_node) {
//        cerr << " [*:fwd] " << has_children() << " ";
        return parent_wildcard_node;
    }
    else {
//        cerr << " [err:fwd] ";
    }
    return next_node_ptr;
}

const pattern_classifier_node* pattern_classifier_node::try_step_forward(char16_t c, try_order try_from_index) const
{
    switch (try_from_index) {
        case try_order::plain_char: {
            return try_step_forward(c);
            break;
        }
//        case try_order::matcher: {
//            const matcher_child<node>& matchchild = try_from_matcher_it->second;
//            if (matchchild.matcher_->match(c)) {
//                return &matchchild.node_;
//            }
//            break;
//        }
        case try_order::digit: {
            if (chars16::is_digit(c)) {
                return try_step_forward(digit);
            }
            else {
                return nullptr;
            }
            break;
        }
        case try_order::number_char: {
            if (chars16::is_number_char(c)) {
                return try_step_forward(number_char);
            }
            else {
                return nullptr;
            }
            break;
        }
        case try_order::any_char: {
            return try_step_forward(any_char);
            break;
        }
        case try_order::wildcard: {
            return try_step_forward(wildcard);
            break;
        }
        default: {
            return nullptr;
        }
    }

    return nullptr;
}

const pattern_classifier_node* pattern_classifier_node::try_step_forward(char16_t c, matcher_map::const_iterator try_from_iterator) const
{
    if (matcher_children_ && try_from_iterator != matcher_children_->end()) {
        const matcher_child<pattern_classifier_node>& matchchild = try_from_iterator->second;
        if (matchchild.matcher_->match(c)) {
            return &matchchild.node_;
        }
    }
    return nullptr;
}


const pattern_classifier_node* pattern_classifier_node::try_step_forward(char16_t c) const
{
    if (!plain_char_children_) return nullptr;
    if (const auto it = plain_char_children_->find(c); it != plain_char_children_->end()) {
        return &(it->second);
    }
    return nullptr;
}

#if 1
const pattern_classifier_node* pattern_classifier_node::find_parent_wildcard() const
{
    const pattern_classifier_node* try_node_ptr = this;
    while (try_node_ptr) {
        if (try_node_ptr->c_ == wildcard) {
            return try_node_ptr;
        }
        try_node_ptr = try_node_ptr->parent_;
    }
    return nullptr;
}

#else
// Seems this version works with all tests
const pattern_classifier_node* pattern_classifier_node::find_parent_wildcard() const
{
    const pattern_classifier_node* try_node_ptr = this;
    if (try_node_ptr) {
        if (try_node_ptr->c_ == wildcard) {
            return try_node_ptr;
        }
        try_node_ptr = try_node_ptr->parent_;
        if (try_node_ptr && try_node_ptr->c_ == wildcard) {
            return try_node_ptr;
        }
    }
    return nullptr;
}
#endif

bool pattern_classifier_node::has_parent_wildcard() const
{
    return find_parent_wildcard() != nullptr;
}

bool pattern_classifier_node::has_children() const
{
    return  (plain_char_children_ && !plain_char_children_->empty() ) ||
            (matcher_children_ && !matcher_children_->empty() )
            ;
}


u16string pattern_classifier_node::u16string() const
{
    size_t n = static_cast<size_t>(level_ +1);
    std::u16string s(n, 0);
    auto node_ptr = this;
    while(n > 0) {
        --n;
        s[n] = node_ptr->c_;
        node_ptr = node_ptr->step_back();
    }
    return s;
}


string pattern_classifier_node::dbg_string() const
{
    auto s16 = u16string();
    std::string s;
    s.reserve(s16.size() +7); // The "magic" 7 is the extra chars possibly added here!
    s += "[" + std::to_string(level_) + "] ";
    if (is_complete_word()) s += "'";
    s += to_string(s16);
    if (is_complete_word()) s += "'";
    return s;
}

/** Get the string representation by tracing back the charater tree. */
void pattern_classifier_node::get_string_value(std::u16string& string_val) const
{
    size_t n = static_cast<size_t>(level_ +1);
    string_val.assign(n, 0);
    auto node_ptr = this;
    while(n > 0) {
        --n;
        string_val[n] = node_ptr->c_;
        node_ptr = node_ptr->step_back();
    }
}

} //end namespace cpaf::semantic
