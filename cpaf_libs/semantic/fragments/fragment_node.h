#ifndef CPAF_FRAGMENT_NODE_H
#define CPAF_FRAGMENT_NODE_H

#include <string>
#include <string_view>
#include <unordered_map>

#include <semantic/tokens/token.h>
#include <semantic/tokens/token_attributes.h>
#include <semantic/fragments/fragment_attribute_weights.h>
#include <cpaf_libs/semantic/fragments/fragment_matcher_branch_point.h>

//#include <sparsehash/dense_hash_map>
// https://github.com/sparsehash/sparsehash-c11, https://github.com/sparsehash/sparsehash
// https://www.youtube.com/watch?v=fHNmRkzxHWs

namespace cpaf::semantic
{

class skipped_tokens;
class scores_accumulator;
class fragment_matcher;
class word_classes_lookup;

class fragment_node
{
public:
    using vector = std::vector<fragment_node*>;
    using attribute_weights_map     = std::map<std::u16string, fragment_attribute_weights, std::less<> >;
    using attributes_const_iterator = attribute_weights_map::const_iterator;
    static const std::u16string number;         // {{¤}}: 0 1 2 3 4 5 6 7 8 9 , .
    static const std::u16string any_word;       // {{~}}
    static const std::u16string wildcard;       // {{*}}
    static const std::u16string maybe_wildcard; // {{*?}}
    static const std::u16string maybe_any_word; // {{~?}}

    // https://stackoverflow.com/questions/20317413/what-are-transparent-comparators
    // We need the explicit std::less<> parameter to enable u16string to u16string_view comparisons
    using word_map          = std::map<std::u16string, fragment_node, std::less<> >;
    using word_map_ptr      = std::unique_ptr<word_map>;


    fragment_node() = default;

    fragment_node*          append                  (const std::u16string& word);
    fragment_node*          append_last             (const std::u16string& word);
    fragment_node*          append_only             (const std::u16string& word);
    const fragment_node*    step_forward            (fragment_matcher_branch_point& branch_point,
                                                     skipped_tokens& skipped_ranges,
                                                     const std::u16string_view& word,
                                                     const token::vector::const_iterator& tk_iter) const;

private:
public:

    void                    ensure_children         () { if (!children_) { children_ = std::make_unique<word_map>();}  }
    void                    delete_children         () { children_.reset(); }
    bool                    has_children            () const;
    bool                    is_root                 () const { return parent_ == nullptr; }
    bool                    equal_subsentence       (const std::u16string_view& str) const { return u16string() == str; }
    bool                    equal_subsentence_strict(const std::u16string_view& str) const { return !is_complete_fragment_ && equal_subsentence(str); }
    bool                    equal                   (const std::u16string_view& str) const { return is_complete_fragment_ && equal_subsentence(str); }
    friend bool             operator==              (const fragment_node& n, const std::u16string_view& str) { return n.equal(str);     }
    friend bool             operator!=              (const fragment_node& n, const std::u16string_view& str) { return !n.equal(str);    }
    std::u16string          u16string               () const;
    std::u16string          u16string_slow          () const;
    std::u16string          dbg_u16string           () const;
    std::string             dbg_string              () const;

    void                    add_attribute           (const std::u16string& attribute_type, const std::u16string& val,
                                                     int weight, const std::u16string& source_field_name,
                                                     const std::u16string& class_name);
    void                    add_attribute           (const std::u16string& attribute_type, const std::u16string& val,
                                                     int weight, const std::u16string& source_field_name );
    void                    add_attribute           (const std::u16string& attribute_type, const std::u16string& val,
                                                     int weight );
    const fragment_attribute_weights* attributes    (const std::u16string& type) const;

    bool                    has_attribute           (const std::u16string& type) const;
    bool                    has_any_of_attributes   (const std::vector<std::u16string>& types) const;

    attributes_const_iterator attributes_begin      () const { return attribute_weights_map_.begin();  }
    attributes_const_iterator attributes_end        () const { return attribute_weights_map_.end();    }

    // --- Setters and getters ---
    const fragment_matcher*     owner_matcher           () const   { return owner_matcher_; }
    void                        owner_matcher_set       (const fragment_matcher* sc) { owner_matcher_ = sc; }
    const word_classes_lookup*  word_class_lookup       () const;
    fragment_node*              parent                  () const                        { return parent_;                   }
    fragment_node*              non_space_parent        () const;
    void                        parent                  (fragment_node* parent_node)    { parent_ = parent_node;            }
    const std::u16string&       word                    () const                        { return word_;                     }
    void                        word                    (const std::u16string& word)    { word_ = word;                     }
    int16_t                     level                   () const                        { return level_;                    }
    void                        level                   (int16_t level)                 { level_ = level;                   }
    bool                        is_complete_fragment    () const                        { return is_complete_fragment_;     }
    void                        is_complete_fragment    (bool complete)                 { is_complete_fragment_ = complete; }
    bool                        is_space                () const                        { return word() == u" "; }
    std::vector<const cpaf::semantic::fragment_node*> path_to_root() const;

private:
    // --- PRIVATE: Helper functions ---
    const fragment_node*    try_step_forward            (const std::u16string_view& word, const token& token, try_order try_from_index) const;
    const fragment_node*    try_step_forward            (const std::u16string_view& word, const token& token) const;
    const fragment_node*    try_step_forward_plain_word (const std::u16string_view& word, const token& token) const;
    const fragment_node*    try_step_forward_classified (const std::u16string_view& word, const token& token) const;
    const fragment_node*    try_step_forward_pattern    (const std::u16string_view& word, const token& token) const;
    const fragment_node*    try_step_forward_number     (const token& token) const;

    const fragment_node*    step_back                   () const { return parent_; }
    fragment_node*          step_back                   ()       { return parent_; }
    const fragment_node*    find_parent_wildcard        () const;
    bool                    has_parent_wildcard         () const;

    // --- PRIVATE: Member data ---
    word_map_ptr                children_;
    attribute_weights_map       attribute_weights_map_;
    fragment_node*              parent_;
    const fragment_matcher*     owner_matcher_ = nullptr;
    std::u16string              word_;
    int16_t                     level_;
    bool                        is_complete_fragment_ = false;

    // Debugging only!!
    std::string                 dbg_word_;
};

} //end namespace cpaf::semantic


#endif //CPAF_FRAGMENT_NODE_H

