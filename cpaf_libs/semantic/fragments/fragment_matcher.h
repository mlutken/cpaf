#ifndef CPAF_SENTENCE_MATCHER_H
#define CPAF_SENTENCE_MATCHER_H

#include <string>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
//#include <sparsehash/dense_hash_map>

#include <unicode/cpaf_unicode_chars.hpp>
#include <semantic/tokens/token_attributes.h>
#include <semantic/primitives/char_match.h>
#include <semantic/cpaf_semantic.h>
#include <semantic/fragments/fragment_node.h>
#include <semantic/primitives/pattern_matcher.h>
#include <cpaf_libs/semantic/fragments/skipped_tokens.h>
#include <cpaf_libs/semantic/fragments/fragment_matcher_branch_point.h>

// https://github.com/sparsehash/sparsehash-c11, https://github.com/sparsehash/sparsehash
// https://www.youtube.com/watch?v=fHNmRkzxHWs

namespace cpaf::semantic
{

enum class parse_mode {
    longest_sentence_multi_pass,
    longest_sentence,
    match_full_input
};

class word_classes_lookup;

class fragment_matcher
{
public:
    /** Hold match candidate info from parsing.

      Used both internally in the "normal" parse_all() functions for standard
      matching and in the parse_all_with_tokens() function that returns the complete
      parsed info. This means we have the detected fragment_node(s) along with the token
      / string::const_iterator positions where these nodes were matched and the remaining
      non matched positions.
      The try_order_ member is only used in the internal functions.
        */
    struct match_candidate
    {
        match_candidate() = default;
        match_candidate (const fragment_node* node,
                         try_order order,
                         const token::vector::const_iterator& it_begin,
                         const token::vector::const_iterator& it_end
                         );

        match_candidate& operator=(match_candidate&& other) = default;
        match_candidate (const match_candidate& other)  = default;
        match_candidate (match_candidate&& other)       = default;

        const fragment_node*            node_ptr_ = nullptr;
        try_order        try_order_;
        token::vector::const_iterator   begin_;
        token::vector::const_iterator   end_;
        bool                            space_after_ = false;

        const fragment_node*                    node            () const { return node_ptr_; }
        const token::vector::const_iterator&    begin           () const { return begin_; }
        const token::vector::const_iterator&    end             () const { return end_; }
        bool                                    space_after     () const { return space_after_; }
        void                                    space_after_set (bool space_after) { space_after_ = space_after; }

        friend bool operator< (const match_candidate& lhs, const match_candidate& rhs);
        unicode::u16_index_pair         token_u16_indices   (const std::u16string& source) const;
        unicode::const_u16_iter_pair    token_u16_iterators () const;
        std::u16string_view             token_u16view       () const;
        std::string                     token_str           () const;

        std::u16string dbg_u16string(bool show_try_order = false) const;
        std::u16string dbg_u16_token() const;
    };

    using candidates_vector = std::vector<match_candidate>;
    using nodes_vector      = std::vector<const fragment_node*>;

    static std::string dbg_str_candidates   (const candidates_vector& candidates, bool show_try_order = false);
    static void dbg_print_candidates        (const candidates_vector& candidates, bool show_try_order = false);

    // ------------------------------------------
    // --- PUBLIC: Functions sentence_matcher ---
    // ------------------------------------------

    fragment_matcher();
    fragment_node*          root                    () { return &root_; }
    const fragment_node*    root                    () const { return &root_; }
    fragment_node::vector   add                     (const std::u16string::const_iterator& begin,
                                                     const std::u16string::const_iterator& end);
    fragment_node::vector   add                     (const std::u16string& str);
    nodes_vector            parse_all               (skipped_tokens& skipped_ranges,
                                                     const std::u16string& str,
                                                     size_t size_hint = 4) const;
    nodes_vector            parse_all               (skipped_tokens& skipped_ranges,
                                                     const std::u16string& str,
                                                     const std::vector<std::u16string>& restrict_to_attributes,
                                                     size_t size_hint = 4) const;
    nodes_vector            parse_all               (skipped_tokens& skipped_ranges,
                                                     const std::u16string& str,
                                                     parse_mode pm,
                                                     size_t size_hint = 4) const;
    nodes_vector            parse_all               (skipped_tokens& skipped_ranges,
                                                     const std::u16string& str,
                                                     parse_mode pm,
                                                     const std::vector<std::u16string>& restrict_to_attributes,
                                                     size_t size_hint = 4) const;
    nodes_vector            parse_all               (skipped_tokens& skipped_ranges,
                                                     const token::vector::const_iterator& begin,
                                                     const token::vector::const_iterator& end,
                                                     parse_mode pm,
                                                     size_t size_hint = 4) const;
    nodes_vector            parse_all               (skipped_tokens& skipped_ranges,
                                                     const token::vector::const_iterator& begin,
                                                     const token::vector::const_iterator& end,
                                                     parse_mode pm,
                                                     const std::vector<std::u16string>& restrict_to_attributes,
                                                     size_t size_hint = 4) const;
    candidates_vector       parse_and_mark_all_tokens(const token::vector& tokens,
                                                     parse_mode pm,
                                                     const std::vector<std::u16string>& restrict_to_attributes,
                                                     size_t size_hint = 8) const;
    candidates_vector       parse_and_mark_all_tokens(token::vector::const_iterator begin,
                                                     token::vector::const_iterator end,
                                                     parse_mode pm,
                                                     const std::vector<std::u16string>& restrict_to_attributes,
                                                     size_t size_hint = 8) const;

//    candidates_vector       remove_matching_types   (token::vector::const_iterator& begin,
//                                                     const token::vector::const_iterator& end,
//                                                     const std::vector<std::u16string>& types_to_remove,
//                                                     parse_mode pm,
//                                                     size_t size_hint = 4) const;
//    candidates_vector       keep_matching_types     (token::vector::const_iterator& begin,
//                                                     const token::vector::const_iterator& end,
//                                                     const std::vector<std::u16string>& types_to_keep,
//                                                     parse_mode pm,
//                                                     size_t size_hint = 4) const;

    candidates_vector       parse_next_all_candidates(skipped_tokens& skipped_ranges,
                                                      token::vector::const_iterator& parse_from_token,
                                                      token::vector::const_iterator end,
                                                      parse_mode pm,
                                                      const std::vector<std::u16string>& restrict_to_attributes) const;
    const fragment_node*    parse_next_node         (skipped_tokens& skipped_ranges,
                                                     token::vector::const_iterator& begin,
                                                     const token::vector::const_iterator& end,
                                                     parse_mode pm,
                                                     const std::vector<std::u16string>& restrict_to_attributes) const;
    match_candidate         parse_next_candidate    (skipped_tokens& skipped_ranges,
                                                     token::vector::const_iterator& begin,
                                                     const token::vector::const_iterator& end,
                                                     parse_mode pm,
                                                     const std::vector<std::u16string>& restrict_to_attributes) const;
    const fragment_node*    lookup_sentence         (skipped_tokens& skipped_ranges,
                                                     const token::vector& tokens) const;
    const fragment_node*    lookup_sentence         (skipped_tokens& skipped_ranges,
                                                     const std::u16string& str) const;
    std::u16string          match_sentence          (const token::vector& tokens) const;
    std::u16string          match_sentence          (const std::u16string& str) const;
    std::u16string          match_subsentence       (const token::vector& tokens) const;
    std::u16string          match_subsentence       (const std::u16string& str) const;
    std::u16string          match_subsentence_strict(const token::vector& tokens) const;
    std::u16string          match_subsentence_strict(const std::u16string& str) const;

    pattern_matcher&          word_pattern_lookup       ()         { return pattern_matcher_; }
    const pattern_matcher&    word_pattern_lookup       () const   { return pattern_matcher_; }
    pattern_matcher*          word_pattern_lookup_ptr   ()         { return &pattern_matcher_; }
    const pattern_matcher*    word_pattern_lookup_ptr   () const   { return &pattern_matcher_; }

    const word_classes_lookup*  word_class_lookup       () const   { return word_classes_lookup_; }
    void                        word_class_lookup_set   (const word_classes_lookup* wcl) { word_classes_lookup_ = wcl; }
    void                        dbg_mode                (bool dbg_mode);
    bool                        dbg_mode                () const        { return dbg_mode_;}

    void                        input_is_lowercase_and_simplified_set (bool is_lc_and_white_space_simplified) { input_is_lowercase_and_simplified_ = is_lc_and_white_space_simplified; }

private:
    bool                        can_use_node            (const fragment_node* node_ptr,
                                                         const std::vector<std::u16string>& restrict_to_attributes) const;

    // --------------------------------
    // --- classifier::branch_point ---
    // --------------------------------

    using branch_point_map      = std::map<const fragment_node*, fragment_matcher_branch_point>;

    // -------------------------------------
    // --- PRIVATE: Build matcher helpers ---
    // -------------------------------------
    bool add_to_word_pattern_lookup   (const std::u16string& word) const;

    std::unique_ptr<std::vector<std::u16string>>
         parse_and_add_string_special     (const std::u16string::const_iterator& begin,
                                           const std::u16string::const_iterator& end);
    fragment_node*  parse_and_add_string  (const std::u16string::const_iterator& begin,
                                           const std::u16string::const_iterator& end);

    bool parse_next_word(bool& add_to_word_pattern_lookup,
                         std::u16string::const_iterator& word_begin,
                         std::u16string::const_iterator& word_end,
                         const std::u16string::const_iterator& end) const;

    // -----------------------------------------------
    // --- PRIVATE: Parse lookup sentences helpers ---
    // -----------------------------------------------
    fragment_matcher_branch_point* get_branch_point         (branch_point_map& branch_points_map,
                                                             const fragment_node* node_ptr,
                                                             const token::vector::const_iterator& lookup_iterator) const;
    const fragment_node*            step_forward            (skipped_tokens& skipped_tokens,
                                                             const fragment_node* node_ptr,
                                                             const std::u16string_view& word,
                                                             const token::vector::const_iterator& tk_iter,
                                                             fragment_matcher_branch_point* branch_point) const;
    const fragment_node*            step_back               (branch_point_map& branch_points_map,
                                                             fragment_matcher_branch_point*& new_branch_point,
                                                             token::vector::const_iterator& new_token_iterator,
                                                             const fragment_node* node_ptr
                                                             ) const;
    const fragment_node*            find_best_candidate_node(token::vector::const_iterator& parse_from_token,
                                                              const candidates_vector& candidate_nodes,
                                                              parse_mode pm
                                                             ) const;
    candidates_vector::const_iterator  find_best_candidate  (token::vector::const_iterator& parse_from_token,
                                                             const candidates_vector& candidate_nodes,
                                                             parse_mode pm
                                                             ) const;

    token::vector::const_iterator   look_for_compound_token(token::vector::const_iterator begin,
                                                          const token::vector::const_iterator& end) const;
    pattern_matcher             pattern_matcher_;
    fragment_node               root_;
    const word_classes_lookup*  word_classes_lookup_ = nullptr;
    bool                        input_is_lowercase_and_simplified_ = false; // FIXME
    bool                        dbg_mode_ = false;
};

// ----------------------
// --- Free functions ---
// ----------------------
void dbg_print (const fragment_matcher::nodes_vector& nodes);


} //end namespace cpaf::semantic


#endif //CPAF_SENTENCE_MATCHER_H
