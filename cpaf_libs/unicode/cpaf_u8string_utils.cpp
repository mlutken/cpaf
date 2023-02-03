#include <string>
#include <locale>
#include <codecvt>
#include <type_traits>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>

#include <unicode/uchar.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <cpaf_libs/unicode/cpaf_unicode_chars.hpp>

#include "cpaf_u16string_utils.h"
#include "cpaf_u8string_utils.h"

// https://en.cppreference.com/w/cpp/string/wide/towupper
// http://unicode.org/Public/UNIDATA/UnicodeData.txt
// http://unicode.org/Public/UNIDATA/SpecialCasing.txt
// https://raymii.org/s/snippets/std_string_to_lowercase_or_uppercase_in_cpp.html
// https://stackoverflow.com/questions/39560894/how-to-convert-unicode-characters-to-uppercase-in-c/55489526
// http://userguide.icu-project.org/howtouseicu

using namespace std;
namespace ba = boost::algorithm;

namespace cpaf::unicode {

using convec = std::vector<std::string>;
using char8_vec = std::vector<char>;


// --------------------------------
// --- Initialize static tables ---
// --------------------------------

// --- Conversion tables ---
#define BASE_CONVERT_CHARS     "@"   ,  ";"    ,  ":"   , "="   , "&"    , "?"      , "%"        , "+"     , "'"  	  ,	"\""	  , "(" 	  ,	")"	, "[" 		 , "]" 	  , "{" 	  , "}"
#define BASE_CONVERT_IDS       "~at~", "~semi~", "~col~", "~eq~", "~and~", "~qmark~", "~percent~", "~plus~", "~apos~", "~quote~", "~lpar~", "~rpar~", "~lbrack~", "~rbrack~", "~lcurl~", "~rcurl~"
static const convec g_base_convert_chars        = convec { BASE_CONVERT_CHARS };
static const convec g_base_convert_ids          = convec { BASE_CONVERT_IDS };

static const convec g_common_convert_ids        = convec { BASE_CONVERT_CHARS, "/"     };
static const convec g_common_convert_chars      = convec { BASE_CONVERT_IDS,   "~sl~"  };

static const convec g_url_convert_ids           = convec { BASE_CONVERT_CHARS, "/"     };
static const convec g_url_convert_chars         = convec { BASE_CONVERT_IDS,   "~sl~"  };

static const convec g_id_convert_chars          = convec { BASE_CONVERT_CHARS, "/"   , " " };
static const convec g_id_convert_ids            = convec { BASE_CONVERT_IDS,   "~sl~", "-" };

static const convec g_file_path_convert_chars   = convec { BASE_CONVERT_CHARS, " " };
static const convec g_file_path_convert_ids     = convec { BASE_CONVERT_IDS,   "_" };

static const convec g_simple_id_convert_chars   = convec { " " };
static const convec g_simple_id_convert_ids     = convec { "-" };

// --- tables ---
static const char8_vec g_simplify_title_remove_chars = [] ()
{
    char8_vec v{ ';', ',', ':', '^', '~', '"', '!', '@', '#', '$',
                  '/', '\\', '<', '>', '|', '=', '+', '?', '*',
                  '_' /*, '—', '–'*/ , '`' /*, '´' */, '(', '{', '[', ')', '}', ']'
                  /*,'°', '™', '®' */
                };
    std::sort(v.begin(), v.end());
    return v;
}()
;

static const char8_vec g_simplify_title_url_remove_chars = [] ()
{
    char8_vec v{ ';', ',', ':', '\'', '^', '~', '"', '!', '@', '#', '$',
                  '%', '&', '/', '\\', '<', '>', '|', '=', '+', '?', '*',
                  '_' /*, '—', '–'*/ , '`' /*, '´'*/ , '(', '{', '[', ')', '}', ']'
                  /*,'°', '™', '®'*/
                };
    std::sort(v.begin(), v.end());
    return v;
}()
;


// -----------------------------------
// --- String conversion functions ---
// -----------------------------------

/**
Convert utf-8 string to lowercase.
@todo This function is currrently implemented using u16string to lower, so
we have unnesscary convertions.
*/
std::string to_lower_copy(const std::string& str)
{
    return to_string(to_lower_copy(to_u16string(str)));
}

/** Find substring between the @a begin_marker and @a end_marker substrings.
  The @a from iterator is advanced to the end of the found substring if we succeed in finding
  both markers.
  Quiuck example:
  @code
    namespace cu = cpaf::unicode;

    const std::string search_in = "find the string between here and the one here in the end";
    std::string::const_iterator from = search_in.begin();
    auto substr = cu::parse_substring_between(from, search_in.end(), "between here", "here");

    cerr << "substr: '" << substr << "'\n";                           // Outputs: ' and the one '
    cerr << "rest: '" << std::string(from, search_in.end()) << "'\n"; // Outputs: ' in the end'
  @endcode

  In case the @a begin_marker is empty the substring starts from the @a from iterator.
  In case the @a end_marker is empty the substring ends at the @a end iterator position and
  we then also set the @a from iterator to be the same as the @a end iterator.

  In case both @a begin_marker and @a end_marker are empty string we return and empty range
  and the @a from iterator is not updated.


  @param from Iterator marking the start of the "string" to search in. Will be updated in case of succesfull match
              to allow you to continue searching onwards after if needed.
  @param end Iterator marking the end of the "string" to search in.
  @param begin_marker This marker marks the beginning of the substring to find. The matched substring begins
                      from the end of this marker
  @param end_marker This marker marks the end of the substring to find. The matched substring ends
                      at the beginning of this marker
  @return An iterator range representing the substring or an empty range. You can use the
          cpaf::unicode::to_string() to convert the retuerned range to a string.

  @sa substring_between()
 */
u8_const_range parse_substring_between(
        std::string::const_iterator& from,
        const std::string::const_iterator& end,
        const std::string& begin_marker,
        const std::string& end_marker)
{
    using namespace std;
    using namespace boost::algorithm;
    std::string::const_iterator substring_begin = end;
    std::string::const_iterator substring_end = end;
    const auto str_len_signed = std::distance(from, end);
    if (str_len_signed <= 0 ||
        (begin_marker.empty() && end_marker.empty())
        ) {
        return u8_const_range(end, end);
    }

    // Find begin marker
    if (!begin_marker.empty()) {
        const auto search_in_begin = boost::make_iterator_range(from, end);
        auto begin_range = boost::algorithm::find_first(search_in_begin, begin_marker);
        if (!begin_range) {
            return u8_const_range(end, end);
        }
        substring_begin = begin_range.end();
    }
    else {
        substring_begin = from;
    }

    // Find end marker
    if (!end_marker.empty()) {
        const auto search_in_end = boost::make_iterator_range(substring_begin, end);
        auto end_range = boost::algorithm::find_first(search_in_end, end_marker);
        if (!end_range) {
            return u8_const_range(end, end);
        }
        from = end_range.end();
        substring_end = end_range.begin();
    }
    else {
        from = end;
        substring_end = end;
    }

    return boost::make_iterator_range(substring_begin, substring_end);
}


/** Get substring between the @a begin_marker and @a end_marker substrings.
  Quiuck example:
  @code
    namespace cu = cpaf::unicode;

    const std::string search_in = "find the string between here and the one here in the end";
    auto substr = cu::substring_between(search_in, "between here", "here");
    cerr << "substr: '" << substr << "'\n";                           // Outputs: ' and the one '
  @endcode

  In case the @a begin_marker is empty the substring starts from the @a from iterator.
  In case the @a end_marker is empty the substring ends at the @a end iterator position and
  we then also set the @a from iterator to be the same as the @a end iterator.

  In case both @a begin_marker and @a end_marker are empty string we return and empty range
  and the @a from iterator is not updated.


  @param search_in The string to search for the substring in.
  @param begin_marker This marker marks the beginning of the substring to find. The matched substring begins
                      from the end of this marker.
  @param end_marker This marker marks the end of the substring to find. The matched substring ends
                     at the beginning of this marker.l
  @param trim_result Set to do_trim::yes if you want the resulting substring to have spaces trimmed from both ends.
  @return The substring between the to markers.

  @sa parse_substring_between()
 */
std::string substring_between(const std::string& search_in,
        const std::string& begin_marker,
        const std::string& end_marker,
        do_trim trim_result)
{
    auto from = search_in.begin();
    const auto subrange = parse_substring_between(from, search_in.end(), begin_marker, end_marker);
    if (trim_result == do_trim::yes) {
        return boost::algorithm::trim_copy(to_string(subrange));
    }
    else {
        return to_string(subrange);
    }
}
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
std::string::const_iterator find_any_of(const std::string& str, const std::vector<std::string>& look_for_strings)
{
    const std::string::const_iterator it_begin = str.begin();
    const std::string::const_iterator it_end = str.end();
    for (const string& look_for: look_for_strings) {
        const auto pos = str.find(look_for);
        if (pos != std::string::npos) {
            return it_begin + static_cast<std::make_signed<std::string::size_type>::type>(pos);
        }
    }
    return it_end;
}

bool contains_any_of(const string& str, const std::vector<string>& look_for_strings)
{
    return find_any_of(str, look_for_strings) != str.end();
}

// --------------------------------------------------
// --- Remove, replace, simplify string functions ---
// --------------------------------------------------
//////////////

void remove_all(std::string& str,
                const std::vector<std::string>& search_for
                )
{
    const auto max = search_for.size();
    for (size_t n = 0; n < max; ++n) {
        ba::replace_all( str, search_for[n], "" );
    }
}

std::string remove_all_copy(const std::string& str,
                               const std::vector<std::string>& search_for
                               )
{
    std::string result_str = str;
    remove_all(result_str, search_for);
    return result_str;
}


void replace_all(std::string& str,
                 const std::vector<std::string>& search_for,
                 const std::vector<std::string>& replace_with)
{
    const auto max = std::min(search_for.size(), replace_with.size());
    for (size_t n = 0; n < max; ++n) {
        ba::replace_all( str, search_for[n], replace_with[n] );
    }
}

std::string replace_all_copy(const std::string& str,
                                const std::vector<std::string>& search_for,
                                const std::vector<std::string>& replace_with)
{
    std::string result_str = str;
    replace_all(result_str, search_for, replace_with);
    return result_str;
}


/** Simplifies all whitespace to single spaces.
Whitespace is SPACE, TAB, CR, NEWLINE
The resulting string is trimmed in both ends an all consequtive
instances of any the the chars considered whitepace is replaced by a
single normal SPACE.
@code

@endcode

*/
std::string simplify_white_space_copy(const std::string& str)
{
    std::string s;
    s.reserve(str.size());
    auto it_end = str.end();
    const auto it_begin = str.begin();
    // Remove any spaces from the end
    while ( (it_end -1) >= it_begin && chars8::is_white_space(*(it_end -1))) {
        --it_end;
    }

    if (it_end <= it_begin) {
        return "";
    }

    auto it = it_begin;
    skip_white_space(it, it_end);
    while (it < it_end) {
        if (skip_white_space(it, it_end)) {
            if (it < it_end) {
                s.push_back(chars8::space);
            }
            else {
                break;
            }
        }
        s.push_back(*it);
        ++it;
    }

    return s;
}

std::string simplify_white_space_copy(const std::string& str,
                                         const std::vector<char>& remove_chars,
                                         bool is_sorted)
{
    std::string s;
    s.reserve(str.size());
    auto it_end = str.end();
    const auto it_begin = str.begin();
    // Remove any spaces from the end
    while ( (it_end -1) >= it_begin && chars8::is_white_space(*(it_end -1))) {
        --it_end;
    }

    if (it_end <= it_begin) {
        return "";
    }

    auto it = it_begin;
    skip_any_of_ws(it, it_end, remove_chars, is_sorted);
    while (it < it_end) {
        if (skip_any_of_ws(it, it_end, remove_chars, is_sorted)) {
            if (it < it_end) {
                s.push_back(chars8::space);
            }
            else {
                break;
            }
        }
        s.push_back(*it);
        ++it;
    }

    return s;
}

std::string simplify_white_space_html_copy(const std::string& str)
{
    std::string s;
    s.reserve(str.size());
    auto it_end = str.end();
    const auto it_begin = str.begin();
    // Remove any spaces from the end
    while ( (it_end -1) >= it_begin && chars8::is_white_space_html(*(it_end -1))) {
        --it_end;
    }

    if (it_end <= it_begin) {
        return "";
    }

    auto it = it_begin;
    skip_white_space_html(it, it_end);
    while (it < it_end) {
        if (chars8::is_white_space_html(*it)) {
            skip_white_space_html(it, it_end);
            if (it < it_end) {
                s.push_back(chars8::space);
            }
            else {
                break;
            }
        }
        s.push_back(*it);
        ++it;
    }

    return s;
}


/** Default title simplify. For example item_title.
The following is performed:
 - Removes all between pairs of brackets: [], {}, () ()  'removeAllInBrackets'
 - Replace all combinations of DASH & SPACE with a single space. So :
   - '- ' => ' '
   - ' -' => ' '
   - ' - ' => ' ' (Implicitly from first two)
 - Remove chars: ;,.:'^~"!@#$%&/\<>|=+?*_
 - Finally we simplify all whitespace.

 @note We do NOT remove apostrophe, but see simplify_title_url_copy() which does
 @sa https://www.thepunctuationguide.com/em-dash.html
 \return String with all above mentioned things removed.

    static $removeChars = [ ';', ',', ':', '^', '~', '"', '!', '@', '#', "\$",
                            '%', '&', '/', '\\', '<', '>', '|', '=', '+', '?', '*',
                            '_', '—', '–', '`', '´', '(', '{', '[', ')', '}', ']', '°',
                            '™', '®' ];
    if ($doRemoveAllInBrackets) {
        $string = removeAllInBrackets($string);
    }
    $string = removeDanglingDashes($string);
    $string = removeDanglingDots($string);
    return stringReplaceAllWithSpaces($removeChars, $string);

 */
std::string simplify_title_copy(const std::string& str, bool remove_all_in_brackets)
{
    std::string s;
    if (remove_all_in_brackets) {
        s = remove_between_brackets_copy(str);
    }
    else {
        s = str;
    }

    s = simplify_white_space_copy(s, g_simplify_title_remove_chars, true);

    return remove_dangling_chars_helper(s);
}

/** Default title simplify for URL use. For example item_title.
The following is performed:
 - Removes all between pairs of brackets: [], {}, () ()  'removeAllInBrackets'
 - Replace all combinations of DASH & SPACE with a single space. So :
   - '- ' => ' '
   - ' -' => ' '
   - ' - ' => ' ' (Implicitly from first two)
 - Remove chars: ;,.:'^~"!@#$%&/\<>|=+?*_
 - Finally we simplify all whitespace.

 @note We DO remove apostrophe, but see simplify_title_copy() which does NOT.
 @sa https://www.thepunctuationguide.com/em-dash.html
 \return String with all above mentioned things removed.

    static $removeChars = [ ';', ',', ':', '^', '~', '"', '!', '@', '#', "\$",
                            '%', '&', '/', '\\', '<', '>', '|', '=', '+', '?', '*',
                            '_', '—', '–', '`', '´', '(', '{', '[', ')', '}', ']', '°',
                            '™', '®' ];
    if ($doRemoveAllInBrackets) {
        $string = removeAllInBrackets($string);
    }
    $string = removeDanglingDashes($string);
    $string = removeDanglingDots($string);
    return stringReplaceAllWithSpaces($removeChars, $string);

 */
std::string simplify_title_url_copy(const std::string& str, bool remove_all_in_brackets)
{
    std::string s;
    if (remove_all_in_brackets) {
        s = remove_between_brackets_copy(str);
    }
    else {
        s = str;
    }

    s = simplify_white_space_copy(s, g_simplify_title_url_remove_chars, true);

    return remove_dangling_chars_helper(s);
}


/** Removes all chars between pairs of chars given as arrays.
  \example
  // Remove all in brackes {}, [], ().
  \auto str = remove_between_copy(str, { u'(', u'{', u'[' }, { u')', u'}', u']' });
  \endexample
  @sa remove_between_brackets_copy()
 */
std::string remove_between_copy  (const std::string& str,
                                     const std::vector<char>& start_chars,
                                     const std::vector<char>& end_chars,
                                     post_op post_process)
{
    std::string s = "";
    size_t n = 0;
    bool skip = false;
    char end_skip_char = 0;
    const auto len = str.size();
    while (n < len) {
        auto c = str[n];
        if (!skip) {
            auto skip_char_it = std::find(start_chars.begin(), start_chars.end(), c);
            if (skip_char_it != start_chars.end()) {
                skip = true;
                const auto index = static_cast<size_t>(std::distance(start_chars.begin(), skip_char_it));
                end_skip_char = end_chars[index];
            }
        }

        if (!skip) {
            s.push_back(c);
        }

        if (skip) {
            if (c == end_skip_char)
                skip = false;
        }
        n++;
    }
    switch (post_process) {
    case post_op::trim:
        ba::trim(s);
        break;
    case post_op::simplify_ws:
        return simplify_white_space_copy(s);
    case post_op::simplify_title:
        return simplify_title_copy(s, false);
    case post_op::simplify_title_full:
        return simplify_title_copy(s, true);    // Remove all in brackets
    default:
        break;
    }

    return s;
}

std::string remove_between_brackets_copy(const std::string& str, post_op post_process)
{
    return to_string( remove_between_brackets_copy(to_u16string(str), post_process) );
}

std::string clean_html_tags_copy(const std::string& str, post_op post_process)
{
    return to_string( clean_html_tags_copy(to_u16string(str), post_process) );
}


/** Return string with first word removed.
 *
 * The first word is removed including any whitespace before and after.
 * @code
 * cout << remove_first_word("one two three"); // outputs: 'two three'
 * cout << remove_first_word(" one two three  "); // outputs: 'two three'
 * @endcode
 * @param search_in The string from which to remove the first word.
 * @return The input string with first word removed. */
std::string remove_first_word(const std::string& search_in)
{
    auto str = search_in;
    remove_first_word(str);
    return str;
}

/** Remove first word from string.
 *
 * The first word is removed including any whitespace before and after.
 * @code
 * cout << remove_first_word("one two three"); // outputs: 'two three'
 * cout << remove_first_word(" one two three "); // outputs: 'two three '
 * @endcode
 * @param search_in The string from which to remove the first word. */
void remove_first_word(std::string& search_in)
{
    if (search_in.empty()) return;
    std::string::const_iterator it = search_in.begin();
    const std::string::const_iterator it_end = search_in.end();
    skip_white_space(it, it_end);
    while (it < it_end && !chars8::is_white_space(*it)) {
        ++it;
    }
    skip_white_space(it, it_end);
    if (it <= it_end) {
        search_in.erase(search_in.begin(), it);
    }
}

// -----------------------------
// --- Name and ID functions ---
// -----------------------------
std::string name_to_id(const std::string& str, bool keep_case)
{
    return to_string( name_to_id(to_u16string(str), keep_case));
}

std::string name_to_simplified_id(const std::string& str, bool keep_case)
{
    return to_string( name_to_simplified_id(to_u16string(str), keep_case));
}

std::string file_path_to_id_path(std::string str, bool do_trim, bool keep_case)
{
    return to_string( file_path_to_id_path(to_u16string(str), do_trim, keep_case));
}

// ---------------------------------------
// --- Special parser helper functions ---
// ---------------------------------------

/** Skips any whitespace chars (html).
Skips any whitespace chars (SPACE, TAB, CR, LF, NEWLINE);
@return true if skipped any whitespace.
*/
bool skip_white_space(std::string::const_iterator& it, const std::string::const_iterator& end)
{
    bool skipped_any = false;
    while (it != end && chars8::is_white_space(*it)) {
        ++it;
        skipped_any = true;
    }
    return skipped_any;
}

/** Skips as long as the chars are one of the remove chars.
The array of chars to be removed mus be sorted.
@return true if skipped any of the chars in question.
*/
bool skip_any_of(std::string::const_iterator& it,
                 const std::string::const_iterator& end,
                 const std::vector<char>& remove_chars,
                 bool is_sorted)
{
    const auto chars_begin  = remove_chars.begin();
    const auto chars_end    = remove_chars.end();

    bool skipped_any = false;
    if (is_sorted) {
        while (it != end && std::binary_search(chars_begin, chars_end, *it)) {
            ++it;
            skipped_any = true;
        }
    }
    else {
        while (it != end && (std::find(chars_begin, chars_end, *it) != chars_end)) {
            ++it;
            skipped_any = true;
        }
    }
    return skipped_any;
}


/** Skips as long as the chars are one of the remove chars or whitespace.
The array of chars to be removed mus be sorted.
Whitespace chars: SPACE, TAB, CR, LF, NEWLINE.
@return true if skipped any of the chars in question.
*/
bool skip_any_of_ws(std::string::const_iterator& it,
                    const std::string::const_iterator& end,
                    const std::vector<char>& remove_chars,
                    bool is_sorted)
{
    const auto chars_begin  = remove_chars.begin();
    const auto chars_end    = remove_chars.end();

    bool skipped_any = false;
    if (is_sorted) {
        while ( it != end &&
                (
                    chars8::is_white_space(*it) ||
                    std::binary_search(chars_begin, chars_end, *it)
                    )
                )
        {
            ++it;
            skipped_any = true;
        }
    }
    else {
        while (it != end &&
               (
                   chars8::is_white_space(*it) ||
                   (std::find(chars_begin, chars_end, *it) != chars_end)
                   )
               )
        {
            ++it;
            skipped_any = true;
        }
    }
    return skipped_any;
}


/** Skips any whitespace chars (html).
Skips any whitespace chars including &nbsp;
@return true if we have not reached the end of the string yet.
*/
bool skip_white_space_html(std::string::const_iterator& it,
                           const std::string::const_iterator& end)
{
    while (it != end && chars8::is_white_space_html(*it)) {
        ++it;
    }
    return it != end;
}


/** Removes dangling dashes '-'s and dots '.'s.
Replace all combinations of DASH & SPACE with a single space. So :
    - '- ' => ' '
    - ' -' => ' '
    - ' - ' => ' ' (Implicitly from first two)

    Same for dots '.'s. */
std::string remove_dangling_chars_helper(const std::string& str)
{
    std::string s;
    s.reserve(str.size());
    const auto it_begin = str.begin();
    const auto it_end = str.end();
    const auto it_last = it_end - 1;
    auto it = it_begin;
    while (it < it_end) {
        bool append = true;
        const auto c = *it;
        if (chars8::is_dot(c) || chars8::is_dash(c)) {
            const bool space_before = (it == it_begin)  || (it_begin < it       && chars8::is_space(*(it-1) ));
            const bool space_after  = (it == it_last)   || (it       < it_last  && chars8::is_space(*(it+1) ));
            if (space_before || space_after) append = false;
            if (space_before && space_after) ++it;
        }
        if (append) {
            s.push_back(*it);
        }
        ++it;
    }

    return s;
}

bool empty_or_only_white_space(std::string::const_iterator it, const std::string::const_iterator& end)
{
    while (it != end) {
        if (!chars8::is_white_space(*it)) {
            return false;
        }
        ++it;
    }
    return true;
}

// ----------------------------------------------------------
// --- Ostream ops: Debug containers of strings to stream ---
// ----------------------------------------------------------
namespace osops {

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& string_vec)
{
    const size_t count = string_vec.size();
    if (count == 0) return os;
    os << string_vec[0];
    for (size_t i = 1; i < count; ++i) {
        os << ", " << string_vec[i];
    }
    return os;
}
}


// END namespace osops

} // END namespace cpaf::unicode

