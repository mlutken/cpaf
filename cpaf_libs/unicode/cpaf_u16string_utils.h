#pragma once

#include <string>
#include <vector>
#include <boost/range/iterator.hpp>
#include <cpaf_libs/unicode/cpaf_unicode_common.h>


namespace cpaf::unicode {


// -----------------------------------
// --- String conversion functions ---
// -----------------------------------

std::string     to_string                       (const std::u16string& str);
std::string     to_string                       (const std::u16string_view& str);
std::string     to_string                       (char16_t c);
std::string     to_string                       (const std::u16string::const_iterator& begin, const std::u16string::const_iterator& end);

std::vector<std::string>
                to_string                       (const std::vector<std::u16string> string_vec);

std::u16string  to_u16string                    (const std::string& str);
std::u16string  to_u16string                    (const std::vector<char16_t>& v);

std::vector<std::u16string>
                to_u16string                    (const std::vector<std::string> string_vec);

std::u16string  to_u16string_revert             (const std::vector<char16_t>& v);
void            to_lower                        (std::u16string& str);
std::u16string  to_lower_copy                   (const std::u16string& str);

std::u16string_view  mk_u16_view                (const std::u16string::const_iterator& begin, const std::u16string::const_iterator& end);


// -----------------------------------
// --- Numbers to string functions ---
// -----------------------------------

template<typename T>
std::u16string to_u16string (T value)
{
    return to_u16string(std::to_string(value));
}

// -----------------------------------
// --- String to number functions ---
// -----------------------------------

// -----------------------------
// --- Find string functions ---
// -----------------------------



// --------------------------------------------------
// --- Remove, replace, simplify string functions ---
// --------------------------------------------------

void            remove_all                      (std::u16string& str,
                                                 const std::vector<std::u16string>& search_for
                                                );

std::u16string  remove_all_copy                 (const std::u16string& str,
                                                 const std::vector<std::u16string>& search_for
                                                );

void            replace_all                     (std::u16string& str,
                                                 const std::vector<std::u16string>& search_for,
                                                 const std::vector<std::u16string>& replace_with
                                                );

std::u16string replace_all_copy                 (const std::u16string& str,
                                                 const std::vector<std::u16string>& search_for,
                                                 const std::vector<std::u16string>& replace_with
                                                );

std::u16string  simplify_white_space_copy       (const std::u16string& str);
std::u16string  simplify_white_space_copy       (const std::u16string& str,
                                                 const std::vector<char16_t>& remove_chars,
                                                 bool is_sorted);
std::u16string  simplify_white_space_html_copy  (const std::u16string& str);
std::u16string  simplify_title_copy             (const std::u16string& str,
                                                 bool remove_all_in_brackets = true);

std::u16string  simplify_title_url_copy         (const std::u16string& str,
                                                 bool remove_all_in_brackets = true);

std::u16string  remove_between_copy             (const std::u16string& str,
                                                 const std::vector<char16_t>& start_chars,
                                                 const std::vector<char16_t>& end_chars,
                                                 post_op post_process = post_op::none
                                                );


std::u16string remove_between_brackets_copy     (const std::u16string& str, post_op post_process = post_op::none);
std::u16string clean_html_tags_copy             (const std::u16string& str, post_op post_process = post_op::none);


std::u16string remove_first_word                (const std::u16string& search_in);
void           remove_first_word                (std::u16string& search_in);

// -----------------------------
// --- Name and ID functions ---
// -----------------------------

std::u16string  name_to_id                      (const std::u16string& str, bool keep_case = false);
std::u16string  name_to_simplified_id           (const std::u16string& str, bool keep_case = false);
std::u16string  file_path_to_id_path            (std::u16string str, bool do_trim = false, bool keep_case = false);

// ---------------------------------------
// --- Special parser helper functions ---
// ---------------------------------------
bool            skip_white_space                (std::u16string::const_iterator& it, const std::u16string::const_iterator& end);
bool            skip_white_space_html           (std::u16string::const_iterator& it, const std::u16string::const_iterator& end);
bool            skip_any_of                     (std::u16string::const_iterator& it, const std::u16string::const_iterator& end,
                                                 const std::vector<char16_t>& remove_chars, bool is_sorted);
bool            skip_any_of_ws                  (std::u16string::const_iterator& it, const std::u16string::const_iterator& end,
                                                 const std::vector<char16_t>& remove_chars, bool is_sorted);
std::u16string  remove_dangling_chars_helper    (const std::u16string& str);

// ----------------------------------------------
// --- Ostream ops: Debug u16string to stream ---
// ----------------------------------------------
namespace osops {
    std::ostream& operator<< (std::ostream& os, char16_t c);
    std::ostream& operator<< (std::ostream& os, const std::u16string& str);
    std::ostream& operator<< (std::ostream& os, const std::u16string_view& view);
} // END namespace osops




} // END namespace cpaf::unicode
