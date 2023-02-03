#include "fragment_node.h"
#include <iostream>
#include <unicode/cpaf_u16string_utils.h>
#include <semantic/primitives/word_classes_lookup.h>
#include <semantic/scores/scores_accumulator.h>
#include "skipped_tokens.h"
#include "fragment_matcher.h"

using namespace cpaf::unicode::osops;
using namespace cpaf::unicode;
using namespace std;

namespace cpaf::semantic
{
const std::u16string fragment_node::number          (u"{{¤}}");
const std::u16string fragment_node::any_word        (u"{{~}}");
const std::u16string fragment_node::wildcard        (u"{{*}}");
const std::u16string fragment_node::maybe_wildcard  (u"{{*?}}");
const std::u16string fragment_node::maybe_any_word  (u"{{~?}}");

fragment_node* fragment_node::append(const std::u16string& word)
{
    auto new_node_ptr = append_only(word);
    new_node_ptr->ensure_children();
    return new_node_ptr;
}

fragment_node* fragment_node::append_last(const std::u16string& word)
{
    auto new_node_ptr = append_only(word);
    new_node_ptr->is_complete_fragment_ = true;
    return new_node_ptr;
}

fragment_node* fragment_node::append_only(const std::u16string& word)
{
    word_map& wm = *children_;
    fragment_node* new_node_ptr = &(wm[word]);
    fragment_node& new_node = *new_node_ptr;

    new_node.word_ = word;
    new_node.level_ = level_ + 1;
    new_node.parent_= this;
    new_node.dbg_word_ = new_node.dbg_string(); // FIXMENM
    return new_node_ptr;
}

const fragment_node* fragment_node::step_forward(
        fragment_matcher_branch_point& branch_point,
        skipped_tokens& skipped_tokens,
        const u16string_view& word,
        const token::vector::const_iterator& tk_iter) const
{
    try_order& try_from_index = branch_point.try_order_index_;
    const token& tk = *(tk_iter);

    if (try_from_index == try_order::try_start) {
        ++try_from_index; // TODO: Do we really need the try_order::try_start at all?
    }
    const fragment_node* next_node_ptr = nullptr;
    while (try_from_index != try_order::try_end) {
        next_node_ptr = try_step_forward(word, tk, try_from_index);
        if (next_node_ptr) {
            skipped_tokens.update(tk_iter, try_from_index);
            return next_node_ptr;
        }
        ++try_from_index;
    }
    const auto parent_wildcard_node = find_parent_wildcard();
    if (parent_wildcard_node) {
        if (skipped_tokens.skip_currently_active()) {
            skipped_tokens.update(tk_iter, try_order::wildcard);
        }
        try_from_index = try_order::plain_word;
        return parent_wildcard_node;
    }
    return next_node_ptr;
}

bool fragment_node::has_children() const
{
    return  (children_ && !children_->empty());
}


std::u16string fragment_node::u16string() const
{
    return u16string_slow();
}


std::u16string fragment_node::u16string_slow() const
{
    std::vector<const fragment_node*> nodes_reversed = path_to_root();
    if (nodes_reversed.size() <= 1) {
        return u"";
    }
    std::u16string s;
    const auto END = nodes_reversed.rend();
    const auto END_1 = END -1;
    auto it = nodes_reversed.rbegin() +1;
    for (; it != END_1; ++it) {
        const fragment_node* node = *it;
        s += node->word();
////        s += u" "; // SPACES SUPPORT
    }
    const fragment_node* node = *it;
    s += node->word();
    return s;
}

std::u16string fragment_node::dbg_u16string() const
{
    std::u16string s;
    s += u"[" + cpaf::unicode::to_u16string(level_) + u"] ";
    if (is_complete_fragment()) s += u"'";
    s += u16string();
    if (is_complete_fragment()) s += u"'";
    return s;
}

std::string fragment_node::dbg_string() const
{
    auto s16 = u16string();
    std::string s;
    s.reserve(s16.size() +7); // The "magic" 7 is the extra chars possibly added here!
    s += "[" + std::to_string(level_) + "] ";
    if (is_complete_fragment()) s += "'";
    s += to_string(s16);
    if (is_complete_fragment()) s += "'";
    return s;
}

void fragment_node::add_attribute(const std::u16string& attribute_type,
                                  const std::u16string& val,
                                  int weight,
                                  const std::u16string& source_field_name,
                                  const std::u16string& /*class_name*/)
{
    fragment_attribute_weights&  aw = attribute_weights_map_[attribute_type];
    aw.type(attribute_type);
    aw.set_weight(val, source_field_name, weight);
}

void fragment_node::add_attribute (const std::u16string& attribute_type, const std::u16string& val,
                                   int weight, const std::u16string& source_field_name )
{
    add_attribute(attribute_type, val, weight, source_field_name, u"");
}

void fragment_node::add_attribute(const std::u16string& attribute_type, const std::u16string& val,
                                  int weight)
{
    add_attribute(attribute_type, val, weight, u"*", u"");
}

/** Get fragment attribute weights for a given type. */
const fragment_attribute_weights* fragment_node::attributes(const std::u16string& type) const
{
    const auto it  = attribute_weights_map_.find(type);
    if (it != attribute_weights_map_.end()) {
        return &(it->second);
    }
    return nullptr;
}

bool fragment_node::has_attribute(const std::u16string& type) const
{
    return attribute_weights_map_.find(type) != attribute_weights_map_.end();
}

bool fragment_node::has_any_of_attributes(const std::vector<std::u16string>& types) const
{
    for (const auto& type: types) {
        if (has_attribute(type)) {
            return true;
        }
    }
    return false;
}

const word_classes_lookup* fragment_node::word_class_lookup() const
{
    return owner_matcher()->word_class_lookup();
}

fragment_node* fragment_node::non_space_parent() const
{
    auto non_space_node_ptr = parent();
    while (non_space_node_ptr && non_space_node_ptr->is_space()) {
        non_space_node_ptr = non_space_node_ptr->parent();
    }
    return non_space_node_ptr;
}

/** Get vector of nodes from this node up the root */
std::vector<const fragment_node*> fragment_node::path_to_root() const
{
    std::vector<const fragment_node*> v;
    v.reserve(static_cast<size_t>(level_+2)); // +2 and not +1 because we add root node
    auto node_ptr = this;
    while (node_ptr) {
        v.push_back(node_ptr);
        node_ptr = node_ptr->step_back();
    }
    return v;
}

// ---------------------------------
// --- PRIVATE: Helper functions ---
// ---------------------------------
const fragment_node* fragment_node::try_step_forward(
        const u16string_view& word,
        const token& token,
        try_order try_from_index) const
{
    switch (try_from_index) {
        case try_order::plain_word: {
            return try_step_forward_plain_word(word, token);
        }
        case try_order::classified_word: {
            return try_step_forward_classified(word, token);
        }
        case try_order::word_pattern: {
            return try_step_forward_pattern(word, token);
        }
        case try_order::number: {
            return try_step_forward_number(token);
        }
        case try_order::any_word: {
            return try_step_forward(any_word, token);
        }
        case try_order::wildcard: {
            return try_step_forward(wildcard, token);
        }
        default: {
            return nullptr;
        }
    }
}

const fragment_node* fragment_node::try_step_forward(const u16string_view& word, const token& /*token*/) const
{
    if (!children_) return nullptr;
    if (const auto it = children_->find(word); it != children_->end()) {
        const fragment_node* n = &(it->second);
        if (n->is_root()) {
            return nullptr;
        }
        return n;
    }
    return nullptr;
}

const fragment_node* fragment_node::try_step_forward_plain_word(const u16string_view& word, const token& token) const
{
    if (!children_) return nullptr;
    auto node_ptr = try_step_forward(word, token);

//    FIXMENM TODO: Enable again! ... In some form!
//    if (!node_ptr) {
//        auto it_end = token.end();
//        auto it_begin = token.begin();
//        // Shave of any trailing separator chars like . , : ! / etc at the end of the current token and try to get a match.
//        while (((it_end-1) >= it_begin) && !node_ptr && token.is_separator(it_end-1)) {
//            --it_end;
//            const u16string_view& w = mk_u16_view(it_begin, it_end);
//            node_ptr = try_step_forward(w, token);
//        }
//        if (it_begin >= it_end) {
//            return node_ptr;
//        }

//        // Shave of any prepended separator chars like . , : ! / etc at the beginning of the current token and try to get a match.
//        while ( (it_begin+1 < it_end) && !node_ptr && token.is_separator(it_begin)) {
//            ++it_begin;
//            const u16string_view& w = mk_u16_view(it_begin, it_end);
//            node_ptr = try_step_forward(w, token);
//        }
//    }
    return node_ptr;
}

const fragment_node* fragment_node::try_step_forward_classified(const u16string_view& word, const token& /*token*/) const
{
    if (!children_) { return nullptr; }
    const word_classes_lookup* wcl = word_class_lookup();
    if (!wcl) { return nullptr; }

    const auto& classes = wcl->classes_for_word(std::u16string(word)); /// @todo Speed can be improved here!!
    for (auto class_name: classes) {
        /// @todo: We need the branch_point here to also support continuing from next class_name when returning to a previously visited branch_point
        if (const auto it = children_->find(class_name); it != children_->end()) {
            const fragment_node* n = &(it->second);
            if (n->is_root()) {
                return nullptr;
            }
            return n;
        }
    }
    return nullptr;
}

/** step forward pattern.
 *
 * @todo optimze for speed word_pattern_lookup().lookup() should accept iter pair
*/

const fragment_node* fragment_node::try_step_forward_pattern(const u16string_view& word, const token& /*token*/) const
{
    if (!children_) { return nullptr; }

    const auto wd = std::u16string(word);
    const pattern_classifier_node* str_node = owner_matcher_->word_pattern_lookup().lookup(wd);
    if (!str_node) {
        return nullptr;
    }
    if (const auto it = children_->find(str_node->u16string()); it != children_->end()) {
        const fragment_node* n = &(it->second);
        if (n->is_root()) {
            return nullptr;
        }
        return n;
    }

    return nullptr;
}

const fragment_node* fragment_node::try_step_forward_number(const token& token) const
{
    if (!token.attributes_.is_number()) {
        return nullptr;
    }
    if (!children_) return nullptr;
    if (const auto it = children_->find(fragment_node::number); it != children_->end()) {
        const fragment_node* n = &(it->second);
        if (n->is_root()) {
            return nullptr;
        }
        return n;
    }
    return nullptr;
}

#if 1
const fragment_node* fragment_node::find_parent_wildcard() const
{
    const fragment_node* try_node_ptr = this;
    while (try_node_ptr) {
        if (try_node_ptr->word_ == wildcard) {
            return try_node_ptr;
        }
        try_node_ptr = try_node_ptr->parent_;
    }
    return nullptr;
}

#else
const fragment_node* fragment_node::find_parent_wildcard() const
{
    const fragment_node* try_node_ptr = this;
    if (try_node_ptr) {
        if (try_node_ptr->word_ == wildcard) {
            return try_node_ptr;
        }
        else if (try_node_ptr->word_ == u" ") {
            try_node_ptr = try_node_ptr->parent_;
            if (try_node_ptr->word_ == wildcard) {
                return try_node_ptr;
            }
        }
    }
    return nullptr;
}
#endif

bool fragment_node::has_parent_wildcard() const
{
    return find_parent_wildcard() != nullptr;
}

/*
SAVE for a few days only!
const fragment_node* fragment_node::try_step_forward_plain_word(const u16string_view& word, const token& token) const
{
    const std::u16string word_str0_FIXMENM(word);
    std::u16string word_str1_FIXMENM;
    std::u16string word_str2_FIXMENM;
    std::u16string word_str3_FIXMENM;
    if (!children_) return nullptr;
    auto node_ptr = try_step_forward(word, token);
    if (!node_ptr) {
        auto it_end = token.end();
        auto it_begin = token.begin();
        // Shave of any trailing separator chars like . , : ! / etc at the end of the current token and try to get a match.
        while (((it_end-1) >= it_begin) && !node_ptr && token.is_separator(it_end-1)) {
            --it_end;
            const u16string_view& w = mk_u16_view(it_begin, it_end);
            word_str1_FIXMENM = std::u16string(w);
            node_ptr = try_step_forward(w, token);
        }
        if (it_begin >= it_end) {
            return node_ptr;
        }

        if (it_end >= it_begin) {
            word_str2_FIXMENM = std::u16string(it_begin, it_end);
        }

        // Shave of any prepended separator chars like . , : ! / etc at the beginning of the current token and try to get a match.
        while ( (it_begin+1 < it_end) && !node_ptr && token.is_separator(it_begin)) {
            ++it_begin;
            const u16string_view& w = mk_u16_view(it_begin, it_end);
            word_str2_FIXMENM = std::u16string(w);
            node_ptr = try_step_forward(w, token);
        }
    }
    return node_ptr;
}

*/

} //end namespace cpaf::semantic


