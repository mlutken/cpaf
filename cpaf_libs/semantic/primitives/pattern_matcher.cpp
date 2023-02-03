#include <iostream>
#include <unicode/cpaf_u16string_utils.h>
#include "pattern_matcher.h"


using namespace cpaf::unicode::osops;
using namespace cpaf::unicode;
using namespace std;


namespace cpaf::semantic
{

pattern_matcher::pattern_matcher()
{
    root_.ensure_plain_children();
    root_.parent(nullptr);
    root_.level(-1);
    root_.c(0);
}

pattern_classifier_node* pattern_matcher::add(
        const std::u16string_view::const_iterator& begin,
        const std::u16string_view::const_iterator& end)
{
    pattern_classifier_node* cur_node_ptr = &root_;
    const auto last = end -1;
    for (auto it = begin; it != last; ++it) {
        const auto c = *it;
        cur_node_ptr = cur_node_ptr->append_char( c, false);
    }
    const auto c = *last;
    cur_node_ptr = cur_node_ptr->append_char(c, true);
    return cur_node_ptr;
}

pattern_classifier_node* pattern_matcher::add(
        const std::u16string::const_iterator& begin,
        const std::u16string::const_iterator& end)
{
    pattern_classifier_node* cur_node_ptr = &root_;
    const auto last = end -1;
    for (auto it = begin; it != last; ++it) {
        const auto c = *it;
        cur_node_ptr = cur_node_ptr->append_char(c, false);
    }
    const auto c = *last;
    cur_node_ptr = cur_node_ptr->append_char(c, true);
    return cur_node_ptr;
}


/// @todo Use iterators instead.
pattern_classifier_node* pattern_matcher::add(const std::u16string_view& str)
{
    return add (str.begin(), str.end());
}

pattern_classifier_node* pattern_matcher::append_char(pattern_classifier_node* node_ptr, char16_t c, bool is_last)
{
    return node_ptr->append_char(c, is_last);
}

//const string_node* pattern_matcher::search_longest_complete(
//        std::u16string::const_iterator& start_from,
//        const std::u16string::const_iterator& end) const
//{
//    const string_node* last_valid_node_ptr;
//    const string_node* node = lookup_helper(last_valid_node_ptr, start_from, end);

//    node = nullptr;

//    return node;
//}

const pattern_classifier_node* pattern_matcher::lookup(
        const std::u16string::const_iterator& begin,
        const std::u16string::const_iterator& end) const
{
    const pattern_classifier_node* TODO_last_valid_node_ptr;
    std::u16string::const_iterator start_from = begin;
    return lookup_helper(TODO_last_valid_node_ptr, start_from, end);
}

const pattern_classifier_node* pattern_matcher::lookup(const u16string& str) const
{
    return lookup(str.begin(), str.end());
}

/** Lookup the string argument and return the matched version.
 A match is only valid for "complete" words.
 If you want to match on non complete words please use match_substring()
Mostly for debugging and unit test purposes.
\example
\endexample

\sa match_substring()
\return The match version of the lookup string or an empty string if not found.
*/
u16string pattern_matcher::match_string(const u16string& str) const
{
    auto node_ptr = lookup(str);
    if (node_ptr && node_ptr->is_complete_word()) {
        return node_ptr->u16string();
    }
    return u"";
}

u16string pattern_matcher::match_substring(const u16string& str) const
{
    auto node_ptr = lookup(str);
    if (node_ptr) {
        return node_ptr->u16string();
    }
    return u"";
}

u16string pattern_matcher::match_substring_strict(const u16string& str) const
{
    auto node_ptr = lookup(str);
    if (node_ptr && !node_ptr->is_complete_word()) {
        return node_ptr->u16string();
    }
    return u"";
}

const pattern_classifier_node* pattern_matcher::lookup_helper(
        const pattern_classifier_node*& TODO_last_valid_node_ptr,
        std::u16string::const_iterator& start_from,
        const std::u16string::const_iterator& end) const
{
    TODO_last_valid_node_ptr = &root_;
    const pattern_classifier_node* node_ptr = &root_;
    if (start_from == end) {
        return nullptr;
    }
    branch_point_map branch_points_map;
    matcher_map::const_iterator try_from_iterator;
    pattern_classifier_node::try_order try_order_index = pattern_classifier_node::try_order::plain_char;
    bool done = false;
    if (dbg_mode()) cerr << "try: " ;
    while (!done) {
        if (!node_ptr) {
            if (dbg_mode()) cerr << " NOT FOUND\n";
            break;
        }
        update_branch_point(branch_points_map, node_ptr, try_order_index, start_from);



        auto parent_node_ptr = node_ptr;
        auto c = *start_from;
        node_ptr = node_ptr->step_forward(c, try_order_index, try_from_iterator);

        if (dbg_mode()) dbg_print_next_node(c, try_order_index, node_ptr);

        ++start_from;
        try_order_index = pattern_classifier_node::try_order::plain_char; // Reset try_order when stepping to next char

        if (maybe_done(node_ptr, start_from, end) ) {
            if (node_ptr) {
                done = !complete_match_mode() || node_ptr->is_complete_word();
            }

            if (!done) { // !node_ptr
                step_back(branch_points_map, parent_node_ptr, node_ptr, try_order_index, start_from);
            }
        }
    }
    if (dbg_mode()) cerr << " ";

    return node_ptr;
}

bool pattern_matcher::maybe_done(
        const pattern_classifier_node*& node_ptr,
        const std::u16string::const_iterator& start_from,
        const std::u16string::const_iterator& end) const
{
    if (!node_ptr || start_from >= end) {
        return true;
    }

    return false;
}

void pattern_matcher::update_branch_point(
        branch_point_map& branch_points_map,
        const pattern_classifier_node* node_ptr,
        pattern_classifier_node::try_order try_order_index,
        const std::u16string::const_iterator& lookup_iterator) const
{
    if (!node_ptr) {
        return;
    }
    const auto it_end = branch_points_map.end();
    auto it = branch_points_map.find(node_ptr);
    if (it != it_end) {
        branch_point& bp = it->second;
        bp.try_order_index_ = try_order_index;
        bp.lookup_iterator_ = lookup_iterator;
    }
    else {
//        branch_points_map.try_emplace(node_ptr, node_ptr, try_order_index, lookup_iterator);
        // INVESTIGATE: Why do this fail while try_emplace work??        branch_points_map.try_emplace(node_ptr, node_ptr, try_order_index, lookup_iterator);
        branch_points_map.emplace(node_ptr, branch_point{node_ptr, try_order_index, lookup_iterator});
    }
}

pattern_matcher::branch_point_map::iterator
pattern_matcher::search_back_for_branch_point(
        branch_point_map& branch_points_map,
        const pattern_classifier_node* node_ptr
) const
{
    const auto it_end = branch_points_map.end();
    auto it = branch_points_map.find(node_ptr);
    while (it != it_end) {
        branch_point* bp = &(it->second);
        bp->advance_try_order();
        if (!bp->done()) {
            return it;
        }

        // Go one level up
        node_ptr = node_ptr->parent();
        it = branch_points_map.find(node_ptr);
    }

    return it_end;
}

bool pattern_matcher::step_back( branch_point_map& branch_points_map,
                                   const pattern_classifier_node* parent_node_ptr,
                                   const pattern_classifier_node*& node_ptr,
                                   pattern_classifier_node::try_order& try_order_index,
                                   std::u16string::const_iterator& start_from
                                  ) const
{
    auto it = search_back_for_branch_point(branch_points_map, parent_node_ptr);
    if (it != branch_points_map.end()) {
        const branch_point& bp = it->second;
        if (!bp.done()) {
            //const string_node* bp_node_ptr = it->first;
            try_order_index = bp.try_order_index_;
            start_from = bp.lookup_iterator_;
            node_ptr = bp.node_ptr_;
            return true;
        }
    }
    node_ptr = nullptr;
    return false;
}

void pattern_matcher::dbg_print_next_node(
        char16_t c,
        pattern_classifier_node::try_order try_order,
        const pattern_classifier_node* node_ptr) const
{
    if (dbg_mode()) {
        if (node_ptr) {
            cerr << to_string(c) << "/" << to_string(node_ptr->c()) << "." << (int)try_order << ">" ;
        }
        else {
            cerr << to_string(c) << "/_." << (int)try_order << ">" ;
        }
    }
}

// --------------------------------
// --- classifier::branch_point ---
// --------------------------------


} //end namespace cpaf::semantic


