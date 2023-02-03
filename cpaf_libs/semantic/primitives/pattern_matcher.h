#ifndef CPAF_PATTERN_MATCHER_H
#define CPAF_PATTERN_MATCHER_H

#include <string>
#include <string_view>
#include <memory>
#include <array>
#include <map>
#include <unordered_map>
//#include <sparsehash/dense_hash_map>

#include <unicode/cpaf_unicode_chars.hpp>
#include <semantic/tokens/token_attributes.h>
#include <semantic/primitives/char_match.h>
#include <semantic/primitives/pattern_matcher_node.h>

// https://github.com/sparsehash/sparsehash-c11, https://github.com/sparsehash/sparsehash
// https://www.youtube.com/watch?v=fHNmRkzxHWs

namespace cpaf::semantic
{

/** Matches complete pattern/strings.
 * Strings are often a single word but can be more words seperated by spaces (TODO: Is this true yet?)
 * @see sentence_matcher matches sentences consisting of one or more strings. */
class pattern_matcher
{
public:
    pattern_matcher();

    pattern_classifier_node*        root                    () { return &root_; }
    const pattern_classifier_node*  root                    () const { return &root_; }
    pattern_classifier_node*        add                     (const std::u16string_view::const_iterator& begin,
                                                             const std::u16string_view::const_iterator& end);
    pattern_classifier_node*        add                     (const std::u16string::const_iterator& begin,
                                                             const std::u16string::const_iterator& end);
    pattern_classifier_node*        add                     (const std::u16string_view& str);
    pattern_classifier_node*        append_char             (pattern_classifier_node* node_ptr, char16_t c, bool is_last);

//    const string_node*  search_longest_complete (std::u16string::const_iterator& start_from,
//                                                 const std::u16string::const_iterator& end) const;
    const pattern_classifier_node*  lookup                  (const std::u16string::const_iterator& begin,
                                                 const std::u16string::const_iterator& end) const;
    const pattern_classifier_node*  lookup                  (const std::u16string& str) const;
    std::u16string      match_string            (const std::u16string& str) const;
    std::u16string      match_substring         (const std::u16string& str) const;
    std::u16string      match_substring_strict  (const std::u16string& str) const;
    void                complete_match_mode     (bool strict_mode)  { complete_match_mode_ = strict_mode; }
    bool                complete_match_mode     () const            { return complete_match_mode_;        }

    void                dbg_mode                (bool dbg_mode)     { dbg_mode_ = dbg_mode;             }
    bool                dbg_mode                () const            { return dbg_mode_;                 }

private:
    using matcher_map = pattern_classifier_node::matcher_map;

    struct branch_point
    {
        explicit    branch_point(const pattern_classifier_node* node_ptr, pattern_classifier_node::try_order try_order_index,
                                 const std::u16string::const_iterator& lookup_iterator)
            : node_ptr_(node_ptr)
            , try_order_index_(try_order_index)
            , lookup_iterator_(lookup_iterator)
        {
        }

        void advance_try_order  () { ++try_order_index_; }
        bool done               () const { return try_order_index_ == pattern_classifier_node::try_order::try_end; }

        const pattern_classifier_node*              node_ptr_;
        pattern_classifier_node::try_order          try_order_index_;
        std::u16string::const_iterator              lookup_iterator_;
    };

    using branch_point_map      = std::unordered_map<const pattern_classifier_node*, branch_point>;

    // --- PRIVATE: Helper functions ---
    const pattern_classifier_node*  lookup_helper           (const pattern_classifier_node*& TODO_last_valid_node_ptr,
                                                 std::u16string::const_iterator& start_from,
                                                 const std::u16string::const_iterator& end) const;
    bool                maybe_done              (const pattern_classifier_node*& node_ptr,
                                                 const std::u16string::const_iterator& start_from,
                                                 const std::u16string::const_iterator& end) const;
    void                update_branch_point     (branch_point_map& branch_points_map,
                                                 const pattern_classifier_node* node_ptr,
                                                 pattern_classifier_node::try_order try_order_index,
                                                 const std::u16string::const_iterator& lookup_iterator) const;
    branch_point_map::iterator
    search_back_for_branch_point                (branch_point_map& branch_points_map,
                                                 const pattern_classifier_node* node_ptr
                                                ) const;

    bool                step_back               (branch_point_map& branch_points_map,
                                                 const pattern_classifier_node* parent_node_ptr,
                                                 const pattern_classifier_node*& node_ptr,
                                                 pattern_classifier_node::try_order& try_order_index,
                                                 std::u16string::const_iterator& start_from
                                                ) const;

    void                dbg_print_next_node    (char16_t c,
                                                pattern_classifier_node::try_order try_order,
                                                const pattern_classifier_node* node_ptr) const;

    pattern_classifier_node root_;
    bool                    complete_match_mode_    = true;
    bool                    dbg_mode_               = false;
};

} //end namespace cpaf::semantic


#endif //CPAF_PATTERN_MATCHER_H

