#include "fragment_matcher.h"
#include <iostream>
#include <sstream>
#include <unicode/cpaf_u16string_utils.h>
#include <semantic/primitives/word_classes_lookup.h>
#include <semantic/primitives/pattern_matcher_node.h>

using namespace cpaf::unicode::osops;
using namespace cpaf::unicode;
using namespace std;

namespace cu = cpaf::unicode;

namespace cpaf::semantic
{

// -------------------------------------
// --- nested class: match_candidate ---
// -------------------------------------

fragment_matcher::match_candidate::match_candidate(
        const fragment_node* node,
        try_order order,
        const token::vector::const_iterator& it_begin,
        const token::vector::const_iterator& it_end)
    : node_ptr_(node),
      try_order_(order),
      begin_(it_begin),
      end_(it_end)
{
}

u16_index_pair fragment_matcher::match_candidate::token_u16_indices(const u16string& source) const
{
    const auto iter_positions = token_u16_iterators();
    const auto source_begin_it = source.begin();
    u16_index_pair positions;
    positions.first = static_cast<size_t>(iter_positions.first - source_begin_it);
    positions.second = static_cast<size_t>(iter_positions.second - source_begin_it);
    return positions;
}

const_u16_iter_pair fragment_matcher::match_candidate::token_u16_iterators() const
{
    return cpaf::semantic::mk_u16_iterator_pair(begin(), end());
}

u16string_view fragment_matcher::match_candidate::token_u16view() const
{
    return u16_view(begin(), end());
}

std::string fragment_matcher::match_candidate::token_str() const
{
    return cpaf::unicode::to_string(u16_view(begin(), end()));
}

std::u16string fragment_matcher::match_candidate::dbg_u16string(bool show_try_order) const
{
    std::u16string s(u"token: '");
    s += token_u16view();
    s += u"' node: ";
    if (node_ptr_) {
        s += node_ptr_->dbg_u16string();
    }
    if (show_try_order) {
        s += u" , try_order: " + unicode::to_u16string(static_cast<int>(try_order_));
    }
    return s;
}

std::u16string fragment_matcher::match_candidate::dbg_u16_token() const
{
    std::u16string s;
    for (auto token_it = begin_; token_it != end_; ++token_it ) {
        s += u" " + token_it->u16str();
    }
    return s;
}


// ------------------------------------------
// --- STATIC: Functions fragment_matcher ---
// ------------------------------------------

string fragment_matcher::dbg_str_candidates(
        const fragment_matcher::candidates_vector& candidates,
        bool show_try_order)
{
    std::stringstream ss;
    ss << "\ncandidates.size(): " <<  candidates.size() << "\n";
    size_t i = 0;
    for (const auto& candidate : candidates) {
        const auto str = candidate.token_u16view();
        ss << "[" << i << "] "
           << " str => "
           << "\"" << str << "\""
           << " ; node => ";

           if (candidate.node()) {
               ss << "{"
                  << candidate.node()->dbg_u16string()
                  << "}";
           }
           if (show_try_order) {
               ss << " , try_order: " << static_cast<int>(candidate.try_order_) << " ";
           }
           ss << "\n";
        ++i;
    }
    return ss.str();
}

void fragment_matcher::dbg_print_candidates(
        const fragment_matcher::candidates_vector& candidates,
        bool show_try_order
        )
{
    cerr << dbg_str_candidates(candidates, show_try_order) << "\n";
}

// ------------------------------------------
// --- PUBLIC: Functions fragment_matcher ---
// ------------------------------------------

fragment_matcher::fragment_matcher()
{
    root_.ensure_children();
    root_.parent(nullptr);
    root_.level(-1);
    root_.word(u"");
    root_.owner_matcher_set(this);
}

fragment_node::vector fragment_matcher::add(const std::u16string::const_iterator& begin, const std::u16string::const_iterator& end)
{
    fragment_node::vector nodes;
    auto strings_to_add = parse_and_add_string_special(begin, end);
    if (strings_to_add && !strings_to_add->empty()) {
        for (const auto& str : *strings_to_add ) {
            nodes.push_back(parse_and_add_string(str.begin(), str.end()));
        }
    }
    else {
        nodes.push_back(parse_and_add_string(begin, end));
    }
    return nodes;
}

fragment_node::vector fragment_matcher::add(const std::u16string& str)
{
    if (input_is_lowercase_and_simplified_) {
        return add(str.begin(), str.end());
    }
    else {
        auto str_simplified = unicode::simplify_white_space_copy(str);
        unicode::to_lower(str_simplified);
        return add(str_simplified.begin(), str_simplified.end());
    }
}

fragment_matcher::nodes_vector fragment_matcher::parse_all(
        skipped_tokens& skipped_ranges,
        const u16string& str,
        size_t size_hint) const
{
    const std::vector<std::u16string> restrict_to_attributes;
    return parse_all(skipped_ranges, str, parse_mode::longest_sentence, restrict_to_attributes, size_hint);
}

fragment_matcher::nodes_vector fragment_matcher::parse_all(
        skipped_tokens& skipped_ranges,
        const u16string& str,
        const std::vector<std::u16string>& restrict_to_attributes,
        size_t size_hint) const
{
    return parse_all(skipped_ranges, str, parse_mode::longest_sentence, restrict_to_attributes, size_hint);
}

fragment_matcher::nodes_vector fragment_matcher::parse_all(
        skipped_tokens& skipped_ranges,
        const std::u16string& str,
        parse_mode pm,
        const std::vector<std::u16string>& restrict_to_attributes,
        size_t size_hint) const
{
    token_parser p(str);
    ////p.dbg_mode(true); // DEBUG
    p.parse();
    auto begin = p.tokens().begin();
    const auto end = p.tokens().end();
    return parse_all(skipped_ranges, begin, end, pm, restrict_to_attributes, size_hint);
}

fragment_matcher::nodes_vector fragment_matcher::parse_all(
        skipped_tokens& skipped_ranges,
        const token::vector::const_iterator& begin,
        const token::vector::const_iterator& end,
        parse_mode pm,
        size_t size_hint) const
{
    const std::vector<std::u16string> restrict_to_attributes;
    return parse_all(skipped_ranges, begin, end, pm, restrict_to_attributes, size_hint);
}

fragment_matcher::nodes_vector fragment_matcher::parse_all(
        skipped_tokens& skipped_ranges,
        const u16string& str,
        parse_mode pm,
        size_t size_hint) const
{
    const std::vector<std::u16string> restrict_to_attributes;
    return parse_all(skipped_ranges, str, pm, restrict_to_attributes, size_hint);
}

fragment_matcher::nodes_vector fragment_matcher::parse_all(
        skipped_tokens& skipped_ranges,
        const token::vector::const_iterator& begin,
        const token::vector::const_iterator& end,
        parse_mode pm,
        const std::vector<std::u16string>& restrict_to_attributes,
        size_t size_hint) const
{
    token::vector::const_iterator beg = begin;
    nodes_vector nodes;
    nodes.reserve(size_hint);
    token::vector::const_iterator fall_back_pos = beg;
    bool done = false;
    while (!done) {
        auto node = parse_next_node(skipped_ranges, beg, end, pm, restrict_to_attributes);
        if (node) {
            nodes.push_back(node);
            if (beg < end && beg->is_white_space()) {
                ++beg;
            }
            fall_back_pos = beg;
        }
        else {
            ++fall_back_pos;
            beg = fall_back_pos;
        }
        done = fall_back_pos >= end;
    }
    return nodes;
}

fragment_matcher::candidates_vector fragment_matcher::parse_and_mark_all_tokens(const token::vector& tokens,
        parse_mode pm,
        const std::vector<std::u16string>& restrict_to_attributes,
        size_t size_hint) const
{
    return parse_and_mark_all_tokens(tokens.begin(), tokens.end(), pm, restrict_to_attributes, size_hint);
}

/** Mark all found tokens but return all tokens as candidates.
 * So we parse all we can and create cadidates as usual with non zero
 * node pointers for matching token ranges. But those ranges not maching
 * anything are also returned as candidates, but with nullpointer node
 * pointers.
 *
 * Used when collapsing string for example where we want to keep all we
 * did not match, i.e. those candidates with null pointer nodes.
 */
fragment_matcher::candidates_vector fragment_matcher::parse_and_mark_all_tokens(token::vector::const_iterator begin,
        token::vector::const_iterator end,
        parse_mode pm,
        const std::vector<std::u16string>& restrict_to_attributes,
        size_t size_hint) const
{
    [[maybe_unused]] skipped_tokens skipped_ranges_unused; // For now, there seems no reason to return these to the caller in this case.
    if (begin == end) {
        return candidates_vector();
    }
    // Skip whitespace tokens from start.
    while ( (begin < end) && begin->attributes_.is_whitespace()) {
        ++begin;
    }

    // Skip whitespace tokens from end.
    while ( ((end -1) > begin) && (end -1)->attributes_.is_whitespace()) {
        --end;
    }

    candidates_vector candidates;
    candidates.reserve(size_hint);
    bool done = false;
    while (!done) {
        token::vector::const_iterator fall_back_pos = begin;
        match_candidate mc = parse_next_candidate(skipped_ranges_unused, begin, end, pm, restrict_to_attributes);

        if (mc.node_ptr_) {
            if (begin < end && (*begin).attributes().is_whitespace()) {
                mc.space_after_set(true);
            }
            ++begin;
        }
        else {
            mc.begin_ = fall_back_pos;
            mc.end_ = begin +1;
            ++begin;
            while (begin < end && (*begin).attributes().is_whitespace()) {
                mc.space_after_set(true);
                ++begin;
            }
        }
        const auto view = u16_view(mc.begin(), mc.end());

        if (view.length() > 0) {
            candidates.push_back(std::move(mc));
        }

        done = begin >= end;
    }
    return candidates;
}

/**
 * Parse forward as long as possible and return a vector with candidates.
@todo Should most likely be private
@note key algorithm! */
fragment_matcher::candidates_vector fragment_matcher::parse_next_all_candidates(
        skipped_tokens& skipped_ranges,
        token::vector::const_iterator& parse_from_token,
        token::vector::const_iterator end,
        parse_mode pm,
        const std::vector<std::u16string>& restrict_to_attributes
        ) const
{
    const auto LEN = std::distance(parse_from_token, end);
    if (LEN == 0 || !root_.has_children()) {
        return candidates_vector();
    }
    const token::vector::const_iterator begin_token = parse_from_token;
    candidates_vector candidates;
    const fragment_node* node_ptr = &root_;

//    // Skip separator tokens from start. NOTE: For now at least!!!
//    while ( (parse_from_token < end) && parse_from_token->attributes_.is_whitespace()) {
//        ++parse_from_token;
//    }

//    // Skip separator tokens from end. NOTE: For now at least!!!
//    while ( ((end -1) > parse_from_token) && (end -1)->attributes_.is_whitespace()) {
//        --end;
//    }

    branch_point_map branch_points;
    fragment_matcher_branch_point* current_bp = nullptr;
    current_bp = get_branch_point(branch_points, node_ptr, parse_from_token); // Create initial branch_point at root
    skipped_ranges.new_parse();
    if (dbg_mode()) cerr << "Try: " ;
    while (node_ptr) {
//        cerr << "FIXMENM ITER '" << node_ptr->u16string() << "'\n";
        if ((parse_from_token >= end)) {
            break;
        }

        current_bp = get_branch_point(branch_points, node_ptr, parse_from_token);

        auto parse_to_token = parse_from_token+1;
        const auto& token_str_view = u16_view(parse_from_token, parse_to_token);
        const auto c_str_debug = unicode::to_string(token_str_view); // FIXMENM

//        cerr << "DEBUG TRY: '" << token_str_view << "'\n" ;
        auto next_node_ptr = step_forward(skipped_ranges, node_ptr, token_str_view, parse_from_token, current_bp);

//       cerr << "DEBUG TRY: '" << token_str_view << "'(" << static_cast<int>(current_bp->try_order_index_) << ")" << "\n" ;
        if (dbg_mode()) cerr << "'" << token_str_view << "'(" << static_cast<int>(current_bp->try_order_index_) << ")" << "->" ;

#if 1
        if (next_node_ptr) {
            if ((pm == parse_mode::longest_sentence_multi_pass || pm == parse_mode::longest_sentence) &&
                 next_node_ptr->is_complete_fragment())
            {
//                cerr << "DEBUG AAA is_complete_fragment: '" << next_node_ptr->u16string() << "' \n\n";
                if (can_use_node(next_node_ptr, restrict_to_attributes)) {
                    candidates.emplace_back(next_node_ptr, current_bp->try_order_index_, begin_token, parse_from_token+1);
                }
//                else {
//                    next_node_ptr = nullptr;
//                }
            }
            else if ( pm == parse_mode::match_full_input && (parse_from_token+1 >= end) ) {
                if (can_use_node(next_node_ptr, restrict_to_attributes)) {
                    candidates.emplace_back(next_node_ptr, current_bp->try_order_index_, begin_token, parse_from_token+1);
                }
//                else {
//                    next_node_ptr = nullptr;
//                }
            }
        }
        if (next_node_ptr) {
            parse_from_token = parse_from_token +1;
        }
        else {
            next_node_ptr = step_back(branch_points, current_bp, parse_from_token, node_ptr);
        }

/// ------------------------
/// ---- New mode ----------
/// ------------------------
#else
        if (next_node_ptr) {
            const auto next_token = parse_from_token +1;
            if ((next_token >= end)) {
                // We are at end of input so here we can't wait testing for match until after a failed forward step
                // As we will exit the main loop next time we reach the beginning
                if ((pm == parse_mode::longest_sentence_multi_pass || pm == parse_mode::longest_sentence) &&
                     next_node_ptr->is_complete_fragment())
                {
                    if (can_use_node(next_node_ptr, restrict_to_attributes)) {
//                        cerr << "FIXMENM AAA is_complete_fragment: '" << next_node_ptr->u16string() << "' try order: " << (int)current_bp->try_order_index_ <<  "\n";
                        candidates.emplace_back(next_node_ptr, current_bp->try_order_index_, begin_token, parse_from_token+1);
                    }
                    ///else { next_node_ptr = nullptr; }
                }
                else if ( pm == parse_mode::match_full_input && (parse_from_token+1 >= end) ) {
                    if (can_use_node(next_node_ptr, restrict_to_attributes)) {
                        candidates.emplace_back(next_node_ptr, current_bp->try_order_index_, begin_token, parse_from_token+1);
                    }
                    ////else { next_node_ptr = nullptr; }
                }

            }
            parse_from_token = next_token;
        }
        else {
            if (node_ptr) {
                auto use_node_ptr = node_ptr;
                if (use_node_ptr->is_space()) {
                    use_node_ptr = node_ptr->parent();
                }

                bool should_step_back = true;

                if (use_node_ptr) {
                    if ((pm == parse_mode::longest_sentence_multi_pass || pm == parse_mode::longest_sentence) &&
                         use_node_ptr->is_complete_fragment())
                    {
                        if (can_use_node(use_node_ptr, restrict_to_attributes)) {
                            const branch_point* bp = get_branch_point(branch_points, use_node_ptr->non_space_parent(), parse_from_token);
                            candidates.emplace_back(use_node_ptr, bp->try_order_index_, begin_token, bp->token_iterator() +1);
                            should_step_back = false;
//                            cerr << "FIXMENM BBB is_complete_fragment: '" << use_node_ptr->u16string() << "' try order: " << (int)bp->try_order_index_ <<  "\n";
                        }
                    }
                    else if ( pm == parse_mode::match_full_input && (parse_from_token >= end) ) {
                        // TODO: Not sure we will ever reach this as we already take care of the match_full_input case above!
                        if (can_use_node(use_node_ptr, restrict_to_attributes)) {
                            cerr << "FIXMENM TEST IF WE EVER GET HERE '" << use_node_ptr->u16string() << "'\n";
                            assert(false);
                            exit(1);
                            const branch_point* bp = get_branch_point(branch_points, use_node_ptr, parse_from_token);
                            candidates.emplace_back(use_node_ptr, bp->try_order_index_, begin_token, parse_from_token);
                            should_step_back = false;
                        }
                    }
                }
                if (should_step_back) {
                    next_node_ptr = step_back(branch_points, current_bp, parse_from_token, use_node_ptr);
                }
            }
            else {
                next_node_ptr = step_back(branch_points, current_bp, parse_from_token, node_ptr);
            }
        }
#endif
        node_ptr = next_node_ptr;
    }
    if (dbg_mode()) cerr << "\n";

    if (candidates.empty()) {
        skipped_ranges.drop_ranges_from_this_parse();
        parse_from_token = begin_token; // Make sure we do not change parse_from_token in this case
    }
    return candidates;
}

const fragment_node* fragment_matcher::parse_next_node(skipped_tokens& skipped_ranges,
        token::vector::const_iterator& parse_from_token,
        const token::vector::const_iterator& end,
        parse_mode pm,
        const std::vector<std::u16string>& restrict_to_attributes
        ) const
{
    const candidates_vector candidates = parse_next_all_candidates(skipped_ranges, parse_from_token, end, pm, restrict_to_attributes);
    return find_best_candidate_node(parse_from_token, candidates, pm);
}

fragment_matcher::match_candidate fragment_matcher::parse_next_candidate(skipped_tokens& skipped_ranges,
        token::vector::const_iterator& parse_from_token,
        const token::vector::const_iterator& end,
        parse_mode pm,
        const std::vector<std::u16string>& restrict_to_attributes
        ) const
{
    const candidates_vector candidates = parse_next_all_candidates(skipped_ranges, parse_from_token, end, pm, restrict_to_attributes);
    const auto it_candidate = find_best_candidate(parse_from_token, candidates, pm);
    if (it_candidate != candidates.end()) {
        return *it_candidate;
    }
    return match_candidate{};
}

const fragment_node* fragment_matcher::lookup_sentence(
        skipped_tokens& skipped_ranges,
        const token::vector& tokens) const
{
    const std::vector<std::u16string> restrict_to_attributes;
    auto begin = tokens.begin();
    return fragment_matcher::parse_next_node(skipped_ranges, begin, tokens.end(), parse_mode::match_full_input, restrict_to_attributes);
}

const fragment_node* fragment_matcher::lookup_sentence(
        skipped_tokens& skipped_ranges,
        const std::u16string& str) const
{
    if(input_is_lowercase_and_simplified_) {
        token_parser p(str);
        p.parse();
        return lookup_sentence(skipped_ranges, p.tokens());
    }
    else {
        const auto str_simplified = unicode::simplify_white_space_copy(str);
        token_parser p(str_simplified);
        p.parse();
        return lookup_sentence(skipped_ranges, p.tokens());
    }

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
std::u16string fragment_matcher::match_sentence(const token::vector& tokens) const
{
    const std::vector<std::u16string> restrict_to_attributes;
    [[maybe_unused]] skipped_tokens skipped_ranges_unused; // For now, there seems no reason to return these to the caller in this case.
    auto begin = tokens.begin();
    auto node_ptr = fragment_matcher::parse_next_node(skipped_ranges_unused, begin, tokens.end(), parse_mode::match_full_input, restrict_to_attributes);
    if (node_ptr && node_ptr->is_complete_fragment()) {
        return node_ptr->u16string();
    }
    return u"";
}

u16string fragment_matcher::match_sentence(const u16string& str) const
{
    if(input_is_lowercase_and_simplified_) {
        token_parser p(str);
        p.parse();
        return match_sentence(p.tokens());
    }
    else {
        const auto str_simplified = unicode::simplify_white_space_copy(str);
        token_parser p(str_simplified);
        p.parse();
        return match_sentence(p.tokens());
    }

}

/** Mainly for unit testing for now */
std::u16string fragment_matcher::match_subsentence(const token::vector& tokens) const
{
    const std::vector<std::u16string> restrict_to_attributes;
    [[maybe_unused]] skipped_tokens skipped_ranges_unused; // For now, there seems no reason to return these to the caller in this case.
    auto begin = tokens.begin();
    auto node_ptr = fragment_matcher::parse_next_node(skipped_ranges_unused, begin, tokens.end(), parse_mode::longest_sentence, restrict_to_attributes);
    if (node_ptr) {
        return node_ptr->u16string();
    }
    return u"";
}

/** Mainly for unit testing for now */
u16string fragment_matcher::match_subsentence(const u16string& str) const
{
    if (input_is_lowercase_and_simplified_) {
        token_parser p(str);
        p.parse();
        return match_subsentence(p.tokens());
    }
    else {
        const auto str_simplified = unicode::simplify_white_space_copy(str);
        token_parser p(str_simplified);
        p.parse();
        return match_subsentence(p.tokens());
    }
}

std::u16string fragment_matcher::match_subsentence_strict(const token::vector& tokens) const
{
    const std::vector<std::u16string> restrict_to_attributes;

    [[maybe_unused]] skipped_tokens skipped_ranges_unused; // For now, there seems no reason to return these to the caller in this case.
    auto begin = tokens.begin();
    auto node_ptr = fragment_matcher::parse_next_node(skipped_ranges_unused, begin, tokens.end(), parse_mode::match_full_input, restrict_to_attributes);
    if (node_ptr && !node_ptr->is_complete_fragment()) {
        return node_ptr->u16string();
    }
    return u"";
}

u16string fragment_matcher::match_subsentence_strict(const u16string& str) const
{
    if(input_is_lowercase_and_simplified_) {
        token_parser p(str);
        p.parse();
        return match_subsentence_strict(p.tokens());
    }
    else {
        const auto str_simplified = unicode::simplify_white_space_copy(str);
        token_parser p(str_simplified);
        p.parse();
        return match_subsentence_strict(p.tokens());
    }

}

void fragment_matcher::dbg_mode(bool dbg_mode)
{
    dbg_mode_ = dbg_mode;
    pattern_matcher_.dbg_mode(dbg_mode);
}


// ------------------------
// --- PRIVATE: Helpers ---
// ------------------------
bool fragment_matcher::can_use_node(
        const fragment_node* node_ptr,
        const std::vector<u16string>& restrict_to_attributes) const
{
    if (restrict_to_attributes.empty()) return true;
    if (node_ptr->has_any_of_attributes(restrict_to_attributes)) {
        return true;
    }

    return false;
}

// --------------------------------------
// --- PRIVATE: Build matcher helpers ---
// --------------------------------------
bool fragment_matcher::add_to_word_pattern_lookup(const u16string& word) const
{
    if (word.size() < 2) return false;
    if (cu::chars16::left_curly_bracket == word[0]) return false;

    for (const auto c: word) {
        if (pattern_classifier_node::is_pattern_match_char(c)) {
            return true;
        }
    }
    return false;
}

// parse_and_add_string_special() helper
static void add_to_all(std::vector<std::u16string>& strings_to_add,
                       const std::u16string& word,
                       bool is_last_word)
{
    if (word.empty()) return;
    for (auto& str: strings_to_add) {
        str += word;
        if (!is_last_word) {
            str.push_back(cpaf::unicode::chars16::space);
        }
    }
}

// parse_and_add_string_special() helper
static void add_to_interval(std::vector<std::u16string>& strings_to_add,
                            const std::u16string& word,
                            size_t begin_pos,
                            size_t end_pos,
                            bool is_last_word)
{
    if (word.empty()) return;
    for (size_t i = begin_pos; i < end_pos; ++i) {
        auto& str = strings_to_add[i];
        str += word;
        if (!is_last_word) {
            str.push_back(cpaf::unicode::chars16::space);
        }
    }
}

// parse_and_add_string_special() helper
static void double_up(std::vector<std::u16string>& strings_to_add)
{
    const std::vector<std::u16string> copy_of_vec(strings_to_add);
    for (const auto& str: copy_of_vec) {
        strings_to_add.push_back(str);
    }
}

/** Currently */
std::unique_ptr<std::vector<std::u16string>>
fragment_matcher::parse_and_add_string_special(
        const std::u16string::const_iterator& begin,
        const std::u16string::const_iterator& end)
{
    auto strings_to_add = std::make_unique<std::vector<std::u16string>>();
    strings_to_add->push_back(u"");
    auto word_begin = begin;
    std::u16string::const_iterator word_end;

    while (word_begin < end) {
        bool add_to_word_pattern_lookup_unused;
        if (parse_next_word(add_to_word_pattern_lookup_unused, word_begin, word_end, end)) {
            const std::u16string word(word_begin, word_end);
            const bool is_last_word = word_end >= end;
            if (word == fragment_node::maybe_wildcard) {
                double_up(*strings_to_add);
                const size_t size = strings_to_add->size();
                const size_t mid_point = size / 2;
                add_to_interval(*strings_to_add, u"", 0, mid_point, is_last_word);    // Add the no words between variant!
                add_to_interval(*strings_to_add, fragment_node::wildcard, mid_point, size, is_last_word); // Add the normal wildcard variant!
            }
            else if (word == fragment_node::maybe_any_word) {
                double_up(*strings_to_add);
                const size_t size = strings_to_add->size();
                const size_t mid_point = size / 2;
                add_to_interval(*strings_to_add, u"", 0, mid_point, is_last_word);    // Add the no words between variant!
                add_to_interval(*strings_to_add, fragment_node::any_word, mid_point, size, is_last_word); // Add the normal wildcard variant!
            }
            else {
                add_to_all(*strings_to_add, word, is_last_word);
            }
            word_begin = word_end;
        }
    }
    return strings_to_add;
}

fragment_node* fragment_matcher::parse_and_add_string(
        const std::u16string::const_iterator& begin,
        const std::u16string::const_iterator& end)
{
    auto cur_node_ptr = &root_;
    cur_node_ptr->owner_matcher_set(this);
    token_parser p(begin, end);
    p.parse();
    const auto tokens = p.tokens();

    const auto tokens_size = tokens.size();
    for (size_t i = 0; i < tokens_size; ++i) {
        const token& tk = tokens[i];
        const std::u16string& word = tk.u16str();
        if (add_to_word_pattern_lookup(word)) {
            pattern_matcher_.add(word.begin(), word.end()); // Add word/token to the embedded pattern_matcher
        }

        cur_node_ptr = cur_node_ptr->append(word);
        cur_node_ptr->owner_matcher_set(this);
    }

    cur_node_ptr->owner_matcher_set(this);
    if (cur_node_ptr != &root_) {
        cur_node_ptr->is_complete_fragment(true);
    }
    else {
        cur_node_ptr = nullptr;
    }
    return cur_node_ptr;
}

bool fragment_matcher::parse_next_word(bool& add_to_word_pattern_lookup,
                                       std::u16string::const_iterator& word_begin,
                                       std::u16string::const_iterator& word_end,
                                       const std::u16string::const_iterator& end) const
{
    add_to_word_pattern_lookup = false;
    // Consume blanks
    while (word_begin != end && chars16::is_blank(*word_begin)) {
        ++word_begin;
    }
    word_end = word_begin;
    if (word_begin == end) {
        return false;
    }
    const bool is_word_class_word = *word_begin == chars16::left_curly_bracket;
    while (word_end != end && !chars16::is_blank(*word_end)) {
        if (!add_to_word_pattern_lookup) {
            add_to_word_pattern_lookup = !is_word_class_word && pattern_classifier_node::is_pattern_match_char(*word_end);
        }
        ++word_end;
    }
    return true;
}

// -----------------------------------------------
// --- PRIVATE: Parse lookup sentences helpers ---
// -----------------------------------------------

fragment_matcher_branch_point* fragment_matcher::get_branch_point(
        branch_point_map& branch_points_map,
        const fragment_node* node_ptr,
        const token::vector::const_iterator& lookup_iterator) const
{
    auto it = branch_points_map.find(node_ptr);
    if (it != branch_points_map.end()) {
        return &(it->second);
    }
    else {
        auto pair = branch_points_map.emplace(node_ptr, fragment_matcher_branch_point(lookup_iterator));
        return &(pair.first->second);
    }
}

const fragment_node* fragment_matcher::step_forward(
        skipped_tokens& skipped_tokens,
        const fragment_node* node_ptr,
        const std::u16string_view& word,
        const token::vector::const_iterator& tk_iter,
        fragment_matcher_branch_point* branch_point) const
{
    if (!branch_point || !node_ptr) {
        return nullptr;
    }
    return node_ptr->step_forward(*branch_point, skipped_tokens, word, tk_iter);
}

/** Step back one or more branch points.
*/
const fragment_node* fragment_matcher::step_back(
        fragment_matcher::branch_point_map& branch_points_map,
        fragment_matcher_branch_point*& new_branch_point,
        token::vector::const_iterator& new_token_iterator,
        const fragment_node* node_ptr) const
{
    new_branch_point = nullptr;
    if (!node_ptr) { return nullptr; }
    node_ptr = node_ptr->parent();
    if (!node_ptr) { return nullptr; }

    while (node_ptr) {
        auto it = branch_points_map.find(node_ptr);
        fragment_matcher_branch_point* bp = &(it->second);
        bp->advance_try_order();
        if (!bp->done()) {
            new_branch_point = bp;
            new_token_iterator = bp->token_iterator();
            return node_ptr;
        }
        else {
            // Go one level up
            node_ptr = node_ptr->parent();
            it = branch_points_map.find(node_ptr);
        }
    }
    return nullptr;
}

const fragment_node* fragment_matcher::find_best_candidate_node(
        token::vector::const_iterator& parse_from_token,
        const candidates_vector& candidate_nodes,
        parse_mode pm) const
{
    const auto it = find_best_candidate(parse_from_token, candidate_nodes, pm);
    if (it != candidate_nodes.end()) {
        return it->node_ptr_;
    }
    return nullptr;
}

fragment_matcher::candidates_vector::const_iterator
fragment_matcher::find_best_candidate(
        token::vector::const_iterator& parse_from_token,
        const candidates_vector& candidate_nodes,
        parse_mode /*pm*/) const
{
    candidates_vector::const_iterator it = std::max_element(candidate_nodes.begin(), candidate_nodes.end());
    if (it != candidate_nodes.end()) {
        parse_from_token = it->end_;
    }
    return it;
}

// ----------------------
// --- Free functions ---
// ----------------------

void dbg_print(const fragment_matcher::nodes_vector& nodes)
{
    auto i = 0;
    for (const auto node: nodes) {
        std::cerr << "[" << i << "]: '" << node->u16string() << "'\n";
        ++i;
    }
}

/// Friend operator less than for match candidates
bool operator<(const fragment_matcher::match_candidate& lhs, const fragment_matcher::match_candidate& rhs) {
    const auto lhs_lvl = lhs.node_ptr_->level();
    const auto rhs_lvl = rhs.node_ptr_->level();
    if (lhs_lvl != rhs_lvl) {
        return lhs_lvl < rhs_lvl;
    }
    else {
        const auto lhs_try_order = static_cast<int>(lhs.try_order_);
        const auto rhs_try_order = static_cast<int>(rhs.try_order_);
        return lhs_try_order > rhs_try_order; // REMARK: The higher the try order, the less "points" we give it.
    }
}

} //end namespace cpaf::semantic
