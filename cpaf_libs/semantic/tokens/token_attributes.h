#ifndef CPAF_TOKEN_ATTRIBUTES_H
#define CPAF_TOKEN_ATTRIBUTES_H

#include <bitset>
#include <map>
#include <vector>
#include <unordered_map>
#include <string>


namespace cpaf::semantic
{

class token_attributes {
public:
    using bitset_type = uint32_t;
    using bitset = std::bitset<32>;
    enum values: bitset_type {
        space_before_pos= 0,
        space_after_pos,
        is_special_pos,
        is_whitespace_pos,
        is_separator_pos,
        is_blank_pos,
        is_space_pos,
        is_dash_pos,
        is_plus_pos,
        is_slash_pos,
        is_backslash_pos,
        is_asterrisk_pos,
        is_leftpar_pos,
        is_rightpar_pos,
        is_leftquote_pos,
        is_rightquote_pos,
        is_string_pos,
        is_number_pos,
        has_comma_pos,
        has_dot_pos,
        has_digits_pos
    };

    static const bitset_type space_before_set   = (1 << space_before_pos);
    static const bitset_type space_after_set    = (1 << space_after_pos);
    static const bitset_type is_special_set     = (1 << is_special_pos);
    static const bitset_type is_whitespace_set  = (1 << is_whitespace_pos);
    static const bitset_type is_separator_set   = (1 << is_separator_pos);
    static const bitset_type is_blank_set       = (1 << is_blank_pos);
    static const bitset_type is_space_set       = (1 << is_space_pos);
    static const bitset_type is_dash_set        = (1 << is_dash_pos);
    static const bitset_type is_plus_set        = (1 << is_plus_pos);
    static const bitset_type is_slash_set       = (1 << is_slash_pos);
    static const bitset_type is_backslash_set   = (1 << is_backslash_pos);
    static const bitset_type is_asterrisk_set   = (1 << is_asterrisk_pos);
    static const bitset_type is_leftpar_set     = (1 << is_leftpar_pos);
    static const bitset_type is_rightpar_set    = (1 << is_rightpar_pos);
    static const bitset_type is_leftquote_set   = (1 << is_leftquote_pos);
    static const bitset_type is_rightquote_set  = (1 << is_rightquote_pos);
    static const bitset_type is_string_set      = (1 << is_string_pos);
    static const bitset_type is_number_set      = (1 << is_number_pos);
    static const bitset_type has_comma_set      = (1 << has_comma_pos);
    static const bitset_type has_dot_set        = (1 << has_dot_pos);
    static const bitset_type has_digits_set     = (1 << has_digits_pos);
    static const bitset_type pure_digits_set    = is_number_set;

    token_attributes() = default;
    explicit token_attributes(const token_attributes& attribs) = default;
    explicit token_attributes(const bitset& attribs) : attributes_(attribs) {}

    token_attributes& operator= (const token_attributes& attribs) = default;
    token_attributes& operator= (const token_attributes::bitset& attribs) { attributes_ = attribs; return *this; }

    token_attributes& operator|= (const token_attributes& attribs) noexcept         { attributes_ |= attribs.attributes(); return *this; }
    token_attributes& operator&= (const token_attributes& attribs) noexcept         { attributes_ &= attribs.attributes(); return *this; }
    token_attributes& operator^= (const token_attributes& attribs) noexcept         { attributes_ ^= attribs.attributes(); return *this; }

    token_attributes& operator|= (const token_attributes::bitset& attribs) noexcept { attributes_ |= attribs; return *this; }
    token_attributes& operator&= (const token_attributes::bitset& attribs) noexcept { attributes_ &= attribs; return *this; }
    token_attributes& operator^= (const token_attributes::bitset& attribs) noexcept { attributes_ ^= attribs; return *this; }

    bool operator== (const token_attributes& attribs) noexcept          { return attributes_ == attribs.attributes(); }
    bool operator!= (const token_attributes& attribs) noexcept          { return !(attributes_ == attribs.attributes()); }
    bool operator== (const token_attributes::bitset& attribs) noexcept  { return attributes_ == attribs; }
    bool operator!= (const token_attributes::bitset& attribs) noexcept  { return !(attributes_ == attribs); }

    bitset::reference   operator[] (std::size_t pos)        { return attributes_[pos]; }
    constexpr bool      operator[] (std::size_t pos) const  { return attributes_[pos]; }

    bool space_before   () const   { return attributes_.test(space_before_pos); }
    bool space_after    () const   { return attributes_.test(space_after_pos);  }
    bool is_special     () const   { return attributes_.test(is_special_pos);}
    bool is_whitespace  () const   { return attributes_.test(is_whitespace_pos);}
    bool is_separator   () const   { return attributes_.test(is_separator_pos); }
    bool is_blank       () const   { return attributes_.test(is_blank_pos);     }
    bool is_space       () const   { return attributes_.test(is_space_pos);     }
    bool is_dash        () const   { return attributes_.test(is_dash_pos);      }
    bool is_plus        () const   { return attributes_.test(is_plus_pos);      }
    bool is_slash       () const   { return attributes_.test(is_slash_pos);     }
    bool is_backslash   () const   { return attributes_.test(is_backslash_pos); }
    bool is_asterrisk   () const   { return attributes_.test(is_asterrisk_pos); }
    bool is_leftpar     () const   { return attributes_.test(is_leftpar_pos);   }
    bool is_rightpar    () const   { return attributes_.test(is_rightpar_pos);  }
    bool is_leftquote   () const   { return attributes_.test(is_leftquote_pos); }
    bool is_rightquote  () const   { return attributes_.test(is_rightquote_pos);}
    bool is_string      () const   { return attributes_.test(is_string_pos);    }
    bool is_number      () const   { return attributes_.test(is_number_pos);    }
    bool has_comma      () const   { return attributes_.test(has_comma_pos);    }
    bool has_dot        () const   { return attributes_.test(has_dot_pos);      }
    bool has_digits     () const   { return attributes_.test(has_digits_pos);   }
    bool is_pure_digits () const   { return (attributes_ & bitset(pure_digits_set)) == pure_digits_set;    }

    void space_before   (bool yes) { attributes_.set(space_before_pos, yes);    }
    void space_after    (bool yes) { attributes_.set(space_after_pos, yes);     }
    void is_special     (bool yes) { attributes_.set(is_special_pos, yes);      }
    void is_whitespace  (bool yes) { attributes_.set(is_whitespace_pos, yes);   }
    void is_separator   (bool yes) { attributes_.set(is_separator_pos, yes);    }
    void is_blank       (bool yes) { attributes_.set(is_blank_pos, yes);        }
    void is_space       (bool yes) { attributes_.set(is_space_pos, yes);     if (yes) is_separator(true); }
    void is_dash        (bool yes) { attributes_.set(is_dash_pos, yes);      if (yes) is_separator(true); }
    void is_plus        (bool yes) { attributes_.set(is_plus_pos, yes);      if (yes) is_separator(true); }
    void is_slash       (bool yes) { attributes_.set(is_slash_pos, yes);     if (yes) is_separator(true); }
    void is_backslash   (bool yes) { attributes_.set(is_backslash_pos, yes); if (yes) is_separator(true); }
    void is_asterrisk   (bool yes) { attributes_.set(is_asterrisk_pos, yes); if (yes) is_separator(true); }
    void is_leftpar     (bool yes) { attributes_.set(is_leftpar_pos, yes);   if (yes) is_separator(true); }
    void is_rightpar    (bool yes) { attributes_.set(is_rightpar_pos, yes);  if (yes) is_separator(true); }
    void is_leftquote   (bool yes) { attributes_.set(is_leftquote_pos, yes); if (yes) is_separator(true); }
    void is_rightquote  (bool yes) { attributes_.set(is_rightquote_pos, yes);if (yes) is_separator(true); }
    void is_string      (bool yes) { attributes_.set(is_string_pos, yes);       }
    void is_number      (bool yes) { attributes_.set(is_number_pos, yes);       }
    void has_comma      (bool yes) { attributes_.set(has_comma_pos, yes);       }
    void has_dot        (bool yes) { attributes_.set(has_dot_pos, yes);         }
    void has_digits     (bool yes) { attributes_.set(has_dot_pos, yes);         }

    const bitset&   attributes   () const   { return attributes_;}
    void            attributes   (const bitset& attributes) { attributes_ = attributes; }

    void reset() noexcept { attributes_.reset(); }

    std::string dbg_string() const;

private:
    bitset attributes_;
};


} //end namespace cpaf::semantic


#endif //CPAF_TOKEN_ATTRIBUTES_H

