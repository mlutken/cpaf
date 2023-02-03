#ifndef CPAF_CHAR_MATCH_H
#define CPAF_CHAR_MATCH_H

#include <set>
#include <memory>
#include <unicode/cpaf_unicode_chars.hpp>

namespace cpaf::semantic
{

class char_match_base
{
public:
    virtual ~char_match_base();
    bool match(char16_t c) const { return do_match(c); }
private:
    virtual bool do_match(char16_t c) const = 0;
};

/** Matcher for any character any number of times (except space). */
class match_wildcard : public char_match_base
{
public:
    static std::unique_ptr<match_wildcard> create () { return std::make_unique<match_wildcard>(); }

    match_wildcard() = default;
private:
    virtual bool do_match(char16_t c) const { return !unicode::chars16::is_space(c); }
};

/** Matcher for any character (except space). */
class match_any_char : public char_match_base
{
public:
    static std::unique_ptr<match_any_char> create () { return std::make_unique<match_any_char>(); }

    match_any_char() = default;
private:
    virtual bool do_match(char16_t c) const { return !unicode::chars16::is_space(c); }
};

/** Matcher for a digit. */
class match_digit : public char_match_base
{
public:
    static std::unique_ptr<match_digit> create () { return std::make_unique<match_digit>(); }

    match_digit() = default;
private:
    virtual bool do_match(char16_t c) const { return unicode::chars16::is_digit(c); }
};

/** Matcher for a single char. */
class match_char : public char_match_base
{
public:
    static std::unique_ptr<match_char> create (char16_t c) { return std::make_unique<match_char>(c); }

    match_char(char16_t c) : c_(c) {}
private:
    virtual bool do_match(char16_t c) const { return c == c_; }
    char16_t c_;
};

/** Matcher for range of char. Both ends are inluded in range. */
class match_range : public char_match_base
{
public:
    static std::unique_ptr<match_range> create (char16_t c_first, char16_t c_last) {
        return std::make_unique<match_range>(c_first, c_last);
    }

    match_range(char16_t c_first, char16_t c_last) : c_first_(c_first), c_last_(c_last) {}
private:
    virtual bool do_match(char16_t c) const { return c_first_ <= c && c <= c_last_; }
    char16_t c_first_;  // Inclusive
    char16_t c_last_;   // Inclusive
};

/** Matcher for a set of chars. */
class match_any_of : public char_match_base
{
public:
    template <class ITERATOR>
    static std::unique_ptr<match_any_of> create (const ITERATOR& begin, const ITERATOR& end) {
        return std::make_unique<match_any_of>(begin, end);
    }

    template<class ITERATOR>
    match_any_of(const ITERATOR& begin, const ITERATOR& end) : chars_(begin, end) {}

private:
    virtual bool do_match(char16_t c) const { return chars_.find(c) != chars_.end(); }
    std::set<char16_t> chars_;
};


/** Matching logical AND between the two matchers. */
class match_and : public char_match_base
{
public:
    static std::unique_ptr<match_range> create (char16_t c_first, char16_t c_last) {
        return std::make_unique<match_range>(c_first, c_last);
    }

    match_and(std::unique_ptr<char_match_base>&& lhs, std::unique_ptr<char_match_base>&& rhs)
        : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
private:
    virtual bool do_match(char16_t c) const { return lhs_->match(c) && c <= lhs_->match(c); }
    std::unique_ptr<char_match_base>    lhs_;
    std::unique_ptr<char_match_base>    rhs_;
};

/** Matching logical OR between the two matchers. */
class match_or : public char_match_base
{
public:
    static std::unique_ptr<match_range> create (char16_t c_first, char16_t c_last) {
        return std::make_unique<match_range>(c_first, c_last);
    }

    match_or(std::unique_ptr<char_match_base> lhs, std::unique_ptr<char_match_base> rhs)
        : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
private:
    virtual bool do_match(char16_t c) const { return lhs_->match(c) || c <= lhs_->match(c); }
    std::unique_ptr<char_match_base>    lhs_;
    std::unique_ptr<char_match_base>    rhs_;
};

class char_match_factory
{
public:
    static char_match_factory& i();

//    std::unique_ptr<char_match_base> create(const std::u16string& match) const;
private:
//    std::unique_ptr<char_match_base> create_simple(const std::u16string_view& sub_match) const;
};

} //end namespace cpaf::semantic


#endif //CPAF_CHAR_MATCH_H

