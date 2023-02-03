#pragma once

#include <cpaf_libs/unicode/cpaf_unicode_common.h>


namespace cpaf::unicode {



std::string         to_lower_copy   (const std::string& str);



inline std::string  to_string       (const u8_const_range& range)   { return std::string(range.begin(), range.end()); }
inline std::string  to_string       (const u8_range& range)         { return std::string(range.begin(), range.end()); }

// -------------------------
// --- Parsing functions ---
// -------------------------

u8_const_range parse_substring_between  (std::string::const_iterator& from,
                                         const std::string::const_iterator& end,
                                         const std::string& begin_marker,
                                         const std::string& end_marker);

std::string     substring_between       (const std::string& search_in,
                                         const std::string& begin_marker,
                                         const std::string& end_marker,
                                         do_trim trim_result = do_trim::yes
                                         );


// -----------------------------------
// --- String conversion functions ---
// -----------------------------------



// -----------------------------------
// --- Numbers to string functions ---
// -----------------------------------

// -----------------------------------
// --- String to number functions ---
// -----------------------------------

// -----------------------------
// --- Find string functions ---
// -----------------------------

std::string::const_iterator     find_any_of (const std::string& str, const std::vector<std::string>& look_for_strings);

bool        contains_any_of (const std::string& str, const std::vector<std::string>& look_for_strings);
// --------------------------------------------------
// --- Remove, replace, simplify string functions ---
// --------------------------------------------------
void            remove_all                   (std::string& str,
                                              const std::vector<std::string>& search_for
                                             );

std::string     remove_all_copy             (const std::string& str,
                                              const std::vector<std::string>& search_for
                                             );

void            replace_all                  (std::string& str,
                                              const std::vector<std::string>& search_for,
                                              const std::vector<std::string>& replace_with
                                             );

std::string replace_all_copy                 (const std::string& str,
                                                 const std::vector<std::string>& search_for,
                                                 const std::vector<std::string>& replace_with
                                             );

std::string  simplify_white_space_copy       (const std::string& str);
std::string  simplify_white_space_copy       (const std::string& str,
                                              const std::vector<char>& remove_chars,
                                              bool is_sorted);
std::string  simplify_white_space_html_copy  (const std::string& str);
std::string  simplify_title_copy             (const std::string& str,
                                                 bool remove_all_in_brackets = true);

std::string  simplify_title_url_copy         (const std::string& str,
                                              bool remove_all_in_brackets = true);

std::string  remove_between_copy             (const std::string& str,
                                              const std::vector<char>& start_chars,
                                              const std::vector<char>& end_chars,
                                              post_op post_process = post_op::none
                                             );

std::string remove_between_brackets_copy    (const std::string& str, post_op post_process = post_op::none);
std::string clean_html_tags_copy            (const std::string& str, post_op post_process = post_op::none);

std::string remove_first_word               (const std::string& search_in);
void        remove_first_word               (std::string& search_in);

// -----------------------------
// --- Name and ID functions ---
// -----------------------------

std::string  name_to_id                      (const std::string& str, bool keep_case = false);
std::string  name_to_simplified_id           (const std::string& str, bool keep_case = false);
std::string  file_path_to_id_path            (std::string str, bool do_trim = false, bool keep_case = false);

// ---------------------------------------
// --- Special parser helper functions ---
// ---------------------------------------

bool            skip_white_space                (std::string::const_iterator& it, const std::string::const_iterator& end);
bool            skip_white_space_html           (std::string::const_iterator& it, const std::string::const_iterator& end);
bool            skip_any_of                     (std::string::const_iterator& it, const std::string::const_iterator& end,
                                                 const std::vector<char>& remove_chars, bool is_sorted);
bool            skip_any_of_ws                  (std::string::const_iterator& it, const std::string::const_iterator& end,
                                                 const std::vector<char>& remove_chars, bool is_sorted);
std::string     remove_dangling_chars_helper    (const std::string& str);

bool            empty_or_only_white_space       (std::string::const_iterator it, const std::string::const_iterator& end);

// ----------------------------------------------------------
// --- Ostream ops: Debug containers of strings to stream ---
// ----------------------------------------------------------
namespace osops {
    std::ostream& operator<< (std::ostream& os, const std::vector<std::string>& string_vec);
} // END namespace osops

} // END namespace cpaf::unicode

