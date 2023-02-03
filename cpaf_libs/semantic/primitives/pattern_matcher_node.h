#ifndef CPAF_PATTERN_MATCHER_NODE_H
#define CPAF_PATTERN_MATCHER_NODE_H

#include <string>
#include <string_view>
#include <memory>
#include <array>
#include <map>
#include <unordered_map>
//#include <sparsehash/dense_hash_map>

#include <semantic/tokens/token_attributes.h>
#include <semantic/primitives/char_match.h>
#include <unicode/cpaf_unicode_chars.hpp>

// https://github.com/sparsehash/sparsehash-c11, https://github.com/sparsehash/sparsehash
// https://www.youtube.com/watch?v=fHNmRkzxHWs

namespace cpaf::semantic
{
class pattern_classifier_node {
public:
    static constexpr char16_t plain_char    = 1;                                // Just any unicode char not used in real text
    static constexpr char16_t matcher       = 2;                                // Just any unicode char not used in real text
    static constexpr char16_t digit         = unicode::chars16::number_sign;    // #: 0 1 2 3 4 5 6 7 8 9
    static constexpr char16_t number_char   = unicode::chars16::currency;       // ¤: 0 1 2 3 4 5 6 7 8 9 , .
    static constexpr char16_t any_char      = unicode::chars16::tilde;          // ~
    static constexpr char16_t wildcard      = unicode::chars16::asterisk;       // *
    static constexpr char16_t try_end       = 3;                                // Just any unicode char not used in real text

    static bool is_pattern_match_char(const char16_t c);

    template <class NODE>
    struct matcher_child {
        NODE                                node_;
        std::unique_ptr<char_match_base>    matcher_;
    };

    using char_map              = std::unordered_map<char16_t, pattern_classifier_node>;
    using matcher_map           = std::map<std::u16string, matcher_child<pattern_classifier_node>>;
    using char_map_ptr          = std::unique_ptr<char_map>;
    using matcher_map_ptr       = std::unique_ptr<matcher_map>;

    enum class try_order : size_t {plain_char=0, matcher, digit, number_char, any_char, wildcard, try_end};
    friend try_order& operator++ (try_order& value) {
        if (value == try_order::try_end) {
            return value;
        }
        using int_type = typename std::underlying_type<try_order>::type;
        value = static_cast<try_order>(static_cast<int_type>(value) + 1);
        return value;
    }

    pattern_classifier_node() = default;

    pattern_classifier_node*                append_char             (char16_t c, bool is_last);
    void                                    finalize_complete_string();
    const pattern_classifier_node*          step_forward            (char16_t c, try_order& try_from_index, matcher_map::const_iterator& try_from_iterator) const;
    const pattern_classifier_node*          try_step_forward        (char16_t c, try_order try_from_index) const;
    const pattern_classifier_node*          try_step_forward        (char16_t c, matcher_map::const_iterator try_from_matcher_it) const;
    const pattern_classifier_node*          try_step_forward        (char16_t c) const;
    const pattern_classifier_node*          step_back               () const            { return parent_; }
    pattern_classifier_node*                step_back               ()                  { return parent_; }
    const pattern_classifier_node*          find_parent_wildcard    () const;

    bool                        has_parent_wildcard     () const;
    void                        ensure_plain_children   () { if (!plain_char_children_) { plain_char_children_ = std::make_unique<char_map>();}  }
    void                        create_matcher_children () { if (!matcher_children_) { matcher_children_ = std::make_unique<matcher_map>();}  }
    bool                        has_children            () const;
    bool                        equal_substring         (const std::u16string_view& str) const { return u16string() == str; }
    bool                        equal_substring_strict  (const std::u16string_view& str) const { return !is_complete_word() && equal_substring(str); }
    bool                        equal                   (const std::u16string_view& str) const { return is_complete_word() && equal_substring(str); }
    friend bool                 operator==              (const pattern_classifier_node& n, const std::u16string_view& str) { return n.equal(str); }
    friend bool                 operator!=              (const pattern_classifier_node& n, const std::u16string_view& str) { return !n.equal(str); }
    const std::u16string&       complete_string         () const { return complete_string_; }
    std::u16string              u16string               () const;
    std::string                 dbg_string              () const;

    // --- Setters and getters ---
    pattern_classifier_node*                parent                  () const                    { return parent_;               }
    void                        parent                  (pattern_classifier_node* parent_node)  { parent_ = parent_node;        }
    char16_t                    c                       () const                    { return c_;                    }
    void                        c                       (char16_t ch)               { c_ = ch;                      }
    int16_t                     level                   () const                    { return level_;                }
    void                        level                   (int16_t level)             { level_ = level;               }
    bool                        is_complete_word        () const                    { return !complete_string_.empty(); }
private:
    // --- PRIVATE: Helper functions ---
    void get_string_value(std::u16string& string_val) const;

    // --- PRIVATE: Member data ---
    std::u16string              complete_string_;
    char_map_ptr                plain_char_children_;
    matcher_map_ptr             matcher_children_;
    pattern_classifier_node*    parent_;
    char16_t                    c_;
    int16_t                     level_;
};


} //end namespace cpaf::semantic


#endif //CPAF_PATTERN_MATCHER_NODE_H
