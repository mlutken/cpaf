#include <string>
#include <locale>
#include <codecvt>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/locale.hpp>
#include <boost/assign.hpp>

#include <unicode/uchar.h>
#include <unicode/ustdio.h>
#include <unicode/ustring.h>
#include <unicode/ucnv.h>

#include <unicode/cpaf_unicode_chars.hpp>
//find_package(ICU 52.0 REQUIRED )



#include "cpaf_u16string_utils.h"

// https://en.cppreference.com/w/cpp/string/wide/towupper
// http://unicode.org/Public/UNIDATA/UnicodeData.txt
// http://unicode.org/Public/UNIDATA/SpecialCasing.txt
// https://raymii.org/s/snippets/std_string_to_lowercase_or_uppercase_in_cpp.html
// https://stackoverflow.com/questions/39560894/how-to-convert-unicode-characters-to-uppercase-in-c/55489526
// http://userguide.icu-project.org/howtouseicu

namespace ba = boost::algorithm;

namespace cpaf::unicode {

using convec = std::vector<std::u16string>;
using char16_vec = std::vector<char16_t>;


// --------------------------------
// --- Initialize static tables ---
// --------------------------------

// --- Conversion tables ---
#define BASE_CONVERT_CHARS     u"@"   ,  u";"    ,  u":"   , u"="   , u"&"    , u"?"      , u"%"        , u"+"     , u"'"  	  ,	u"\""	  , u"(" 	  ,	u")"	, u"[" 		 , u"]" 	  , u"{" 	  , u"}"
#define BASE_CONVERT_IDS       u"~at~", u"~semi~", u"~col~", u"~eq~", u"~and~", u"~qmark~", u"~percent~", u"~plus~", u"~apos~", u"~quote~", u"~lpar~", u"~rpar~", u"~lbrack~", u"~rbrack~", u"~lcurl~", u"~rcurl~"
static const convec g_base_convert_chars        = convec { BASE_CONVERT_CHARS };
static const convec g_base_convert_ids          = convec { BASE_CONVERT_IDS };

static const convec g_common_convert_ids        = convec { BASE_CONVERT_CHARS, u"/"     };
static const convec g_common_convert_chars      = convec { BASE_CONVERT_IDS,   u"~sl~"  };

static const convec g_url_convert_ids           = convec { BASE_CONVERT_CHARS, u"/"     };
static const convec g_url_convert_chars         = convec { BASE_CONVERT_IDS,   u"~sl~"  };

static const convec g_id_convert_chars          = convec { BASE_CONVERT_CHARS, u"/"   , u" " };
static const convec g_id_convert_ids            = convec { BASE_CONVERT_IDS,   u"~sl~", u"-" };

static const convec g_file_path_convert_chars   = convec { BASE_CONVERT_CHARS, u" " };
static const convec g_file_path_convert_ids     = convec { BASE_CONVERT_IDS,   u"_" };

static const convec g_simple_id_convert_chars   = convec { u" " };
static const convec g_simple_id_convert_ids     = convec { u"-" };

// --- tables ---
static const char16_vec g_simplify_title_remove_chars = [] ()
{
    char16_vec v{ u';', u',', u':', u'^', u'~', u'"', u'!', u'@', u'#', '$',
                  u'/', u'\\', u'<', u'>', u'|', u'=', u'+', u'?', u'*',
                  u'_', u'—', u'–', u'`', u'´', u'(', u'{', u'[', u')', u'}', u']',
                  u'°', u'™', u'®'
                };
    std::sort(v.begin(), v.end());
    return v;
}()
;

static const char16_vec g_simplify_title_url_remove_chars = [] ()
{
    char16_vec v{ u';', u',', u':', u'\'', u'^', u'~', u'"', u'!', u'@', u'#', '$',
                  u'%', u'&', u'/', u'\\', u'<', u'>', u'|', u'=', u'+', u'?', u'*',
                  u'_', u'—', u'–', u'`', u'´', u'(', u'{', u'[', u')', u'}', u']',
                  u'°', u'™', u'®'
                };
    std::sort(v.begin(), v.end());
    return v;
}()
;


// -----------------------------------
// --- String conversion functions ---
// -----------------------------------

static std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::codecvt_mode::little_endian>, char16_t> cvt_u16;


std::string to_string(const std::u16string& str)
{
    return cvt_u16.to_bytes(str);
}

std::string to_string(const std::u16string_view& str)
{
    return to_string(std::u16string(str));
}

std::string to_string(char16_t c)
{
    return to_string(std::u16string(1, c));
}

std::string to_string(const std::u16string::const_iterator& begin, const std::u16string::const_iterator& end)
{
    return to_string(std::u16string(begin, end));
}

std::vector<std::string> to_string(const std::vector<std::u16string> string_vec)
{
    std::vector<std::string> v;
    v.reserve(string_vec.size());
    for (const auto& u16s : string_vec) {
        v.push_back(to_string(u16s));
    }
    return v;
}


///**
//  @todo Perhaps improve by implmenting ourselves: https://stackoverflow.com/questions/5242524/converting-int-to-string-in-c
//*/
//std::u16string to_u16string(int value)
//{
//    return to_u16string(std::to_string(value));
//}

std::u16string to_u16string(const std::string& str)
{

    return cvt_u16.from_bytes(str);
}

std::u16string to_u16string(const std::vector<char16_t>& v)
{
    return std::u16string(v.begin(), v.end());
}

std::vector<std::u16string> to_u16string(const std::vector<std::string> string_vec)
{
    std::vector<std::u16string> v;
    v.reserve(string_vec.size());
    for (const auto& s : string_vec) {
        v.push_back(to_u16string(s));
    }
    return v;
}

std::u16string to_u16string_revert(const std::vector<char16_t>& v)
{
    return std::u16string(v.rbegin(), v.rend());
}

void to_lower(std::u16string& str)
{
    const auto end = str.end();
    for (auto it = str.begin(); it != end; ++it) {
        *it = u_tolower(*it);
    }
}

std::u16string to_lower_copy(const std::u16string& str)
{
    std::u16string s;
    s.reserve(str.size());
    for(const auto c : str) {
        s.push_back(u_tolower(c));
    }
    return s;
}

std::u16string_view mk_u16_view(const std::u16string::const_iterator& begin,
                                const std::u16string::const_iterator& end)
{
    return std::u16string_view(&(*begin), static_cast<std::size_t>(std::distance(begin, end)));
}

// --------------------------------------------------
// --- Remove, replace, simplify string functions ---
// --------------------------------------------------
void remove_all(std::u16string& str,
                const std::vector<std::u16string>& search_for
                )
{
    const auto max = search_for.size();
    for (size_t n = 0; n < max; ++n) {
        ba::replace_all( str, search_for[n], u"" );
    }
}

std::u16string remove_all_copy(const std::u16string& str,
                               const std::vector<std::u16string>& search_for
                               )
{
    std::u16string result_str = str;
    remove_all(result_str, search_for);
    return result_str;
}


void replace_all(std::u16string& str,
                 const std::vector<std::u16string>& search_for,
                 const std::vector<std::u16string>& replace_with)
{
    const auto max = std::min(search_for.size(), replace_with.size());
    for (size_t n = 0; n < max; ++n) {
        ba::replace_all( str, search_for[n], replace_with[n] );
    }
}

std::u16string replace_all_copy(const std::u16string& str,
                                const std::vector<std::u16string>& search_for,
                                const std::vector<std::u16string>& replace_with)
{
    std::u16string result_str = str;
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
std::u16string simplify_white_space_copy(const std::u16string& str)
{
    std::u16string s;
    s.reserve(str.size());
    auto it_end = str.end();
    const auto it_begin = str.begin();
    // Remove any spaces from the end
    while ( (it_end -1) >= it_begin && chars16::is_white_space(*(it_end -1))) {
        --it_end;
    }

    if (it_end <= it_begin) {
        return u"";
    }

    auto it = it_begin;
    skip_white_space(it, it_end);
    while (it < it_end) {
        if (skip_white_space(it, it_end)) {
            if (it < it_end) {
                s.push_back(chars16::space);
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

std::u16string simplify_white_space_copy(const std::u16string& str,
                                         const std::vector<char16_t>& remove_chars,
                                         bool is_sorted)
{
    std::u16string s;
    s.reserve(str.size());
    auto it_end = str.end();
    const auto it_begin = str.begin();
    // Remove any spaces from the end
    while ( (it_end -1) >= it_begin && chars16::is_white_space(*(it_end -1))) {
        --it_end;
    }

    if (it_end <= it_begin) {
        return u"";
    }

    auto it = it_begin;
    skip_any_of_ws(it, it_end, remove_chars, is_sorted);
    while (it < it_end) {
        if (skip_any_of_ws(it, it_end, remove_chars, is_sorted)) {
            if (it < it_end) {
                s.push_back(chars16::space);
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

std::u16string simplify_white_space_html_copy(const std::u16string& str)
{
    std::u16string s;
    s.reserve(str.size());
    auto it_end = str.end();
    const auto it_begin = str.begin();
    // Remove any spaces from the end
    while ( (it_end -1) >= it_begin && chars16::is_white_space_html(*(it_end -1))) {
        --it_end;
    }

    if (it_end <= it_begin) {
        return u"";
    }

    auto it = it_begin;
    skip_white_space_html(it, it_end);
    while (it < it_end) {
        if (chars16::is_white_space_html(*it)) {
            skip_white_space_html(it, it_end);
            if (it < it_end) {
                s.push_back(chars16::space);
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
std::u16string simplify_title_copy(const std::u16string& str, bool remove_all_in_brackets)
{
    std::u16string s;
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
std::u16string simplify_title_url_copy(const std::u16string& str, bool remove_all_in_brackets)
{
    std::u16string s;
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
std::u16string remove_between_copy  (const std::u16string& str,
                                     const std::vector<char16_t>& start_chars,
                                     const std::vector<char16_t>& end_chars,
                                     post_op post_process)
{
    std::u16string s = u"";
    size_t n = 0;
    bool skip = false;
    char16_t end_skip_char = 0;
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


/** Remove all in brackets {}, [], ().
@return String with all inside brackes: {}, [], () including the brackets removed. */
std::u16string remove_between_brackets_copy(const std::u16string& str, post_op post_process)
{
    static const std::vector<char16_t> start_chars { u'(', u'{', u'[' };
    static const std::vector<char16_t> end_chars   { u')', u'}', u']' };

    return remove_between_copy(str, start_chars, end_chars, post_process);
}

/** Remove all html tags from string.
 * Note this is a very simple implementation, that simply removes all between
 * pairs of '<' '>' including the arrows.
 *
@return . */
std::u16string clean_html_tags_copy(const std::u16string& str, post_op post_process)
{
    static const std::vector<char16_t> start_chars { u'<' };
    static const std::vector<char16_t> end_chars   { u'>' };
    return remove_between_copy(str, start_chars, end_chars, post_process);
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
std::u16string remove_first_word(const std::u16string& search_in)
{
    auto str = search_in;
    remove_first_word(str);
    return str;
}

/** Remove first word from string.
 *
 * The first word is removed including any whitespace before and after.
 * @code
 * cout << remove_first_word(u"one two three"); // outputs: 'two three'
 * cout << remove_first_word(u" one two three "); // outputs: 'two three '
 * @endcode
 * @param search_in The string from which to remove the first word. */
void remove_first_word(std::u16string& search_in)
{
    if (search_in.empty()) return;
    std::u16string::const_iterator it = search_in.begin();
    const std::u16string::const_iterator it_end = search_in.end();
    skip_white_space(it, it_end);
    while (it < it_end && !chars16::is_white_space(*it)) {
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



std::u16string name_to_id(const std::u16string& str, bool keep_case)
{
    std::u16string s = simplify_white_space_copy(str);
    replace_all(s, g_id_convert_chars, g_id_convert_ids);
    if (!keep_case) {
        to_lower(s);
    }
    return s;
}

/** Lowercase, removes special characters, trims whitespace and replace spaces with dash(-).
 * @param $name the input name
 * @return The name converted to a string suiable for both URL and disk.
 * Remove all special chars before creating the ID, where we simply
    replace spaces with '-' */
std::u16string name_to_simplified_id(const std::u16string& str, bool keep_case)
{
    std::u16string s = simplify_title_url_copy(str);
    replace_all(s, g_simple_id_convert_chars, g_simple_id_convert_ids);
    if (!keep_case) {
        to_lower(s);
    }
    return s;
}

/** Convert filepath to 'ID file path' suitable for using as disk name
 * So it's still a filepath with '/' separators but spaces has been replaced
 * with '-' (dashes). Also other special chars are changed similar to
 * the name_to_id() function.
 * @see idPathToFilePath()
 *  */
std::u16string file_path_to_id_path(std::u16string str, bool do_trim, bool keep_case)
{
    if (do_trim) {
        ba::trim(str);
    }
    if (!keep_case) {
        to_lower(str);
    }
    if (str.empty()) return u"";
    auto id = replace_all_copy(str, g_file_path_convert_chars, g_file_path_convert_ids);

    return id;
}

// ---------------------------------------
// --- Special parser helper functions ---
// ---------------------------------------

/** Skips any whitespace chars (html).
Skips any whitespace chars (SPACE, TAB, CR, LF, NEWLINE);
@return true if skipped any whitespace.
*/
bool skip_white_space(std::u16string::const_iterator& it,
                      const std::u16string::const_iterator& end)
{
    bool skipped_any = false;
    while (it != end && chars16::is_white_space(*it)) {
        ++it;
        skipped_any = true;
    }
    return skipped_any;
}

/** Skips as long as the chars are one of the remove chars.
The array of chars to be removed mus be sorted.
@return true if skipped any of the chars in question.
*/
bool skip_any_of(std::u16string::const_iterator& it,
                 const std::u16string::const_iterator& end,
                 const std::vector<char16_t>& remove_chars,
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
bool skip_any_of_ws(std::u16string::const_iterator& it,
                    const std::u16string::const_iterator& end,
                    const std::vector<char16_t>& remove_chars,
                    bool is_sorted)
{
    const auto chars_begin  = remove_chars.begin();
    const auto chars_end    = remove_chars.end();

    bool skipped_any = false;
    if (is_sorted) {
        while ( it != end &&
                (
                    chars16::is_white_space(*it) ||
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
                   chars16::is_white_space(*it) ||
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
bool skip_white_space_html(std::u16string::const_iterator& it,
                           const std::u16string::const_iterator& end)
{
    while (it != end && chars16::is_white_space_html(*it)) {
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
std::u16string remove_dangling_chars_helper(const std::u16string& str)
{
    std::u16string s;
    s.reserve(str.size());
    const auto it_begin = str.begin();
    const auto it_end = str.end();
    const auto it_last = it_end - 1;
    auto it = it_begin;
    while (it < it_end) {
        bool append = true;
        const auto c = *it;
        if (chars16::is_dot(c) || chars16::is_dash(c)) {
            const bool space_before = (it == it_begin)  || (it_begin < it       && chars16::is_space(*(it-1) ));
            const bool space_after  = (it == it_last)   || (it       < it_last  && chars16::is_space(*(it+1) ));
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

// ----------------------------------------------
// --- Ostream ops: Debug u16string to stream ---
// ----------------------------------------------
/** \namespace cpaf::osops Debug u16string to stream
Mainly for debugging purposes.
Use for example like this:
\example
using namespace cpaf::u16string::osops;
\endexample
*/
namespace osops {
std::ostream& operator<<(std::ostream& os, char16_t c)
{
    os << to_string(c);
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::u16string& str)
{
    os << to_string(str);
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::u16string_view& view)
{
    os << to_string(view);
    return os;
}

}// END namespace osops




#if 0


/**
 * Reverse function of filePathToIdPath() to covert back.
 * @see filePathToIdPath()
 *  */
function idPathToFilePath(string $file_path_id)
{
    global $g_file_path_convert_ids;
    global $g_file_path_convert_chars;
    return str_replace  ( $g_file_path_convert_ids , $g_file_path_convert_chars, $file_path_id);
}

/** Replaces special characters with dash(-) and trims whitespace.
 * @global type $g_id_convert_chars Holds the special characters that will be replaces with -
 * @param type $string the input string
 * @param type $doTrim if set to false, whitespace is not removed
 * @return string the modified string.
 */
function nameToImageFileName(string $string) : string
{
                                                   $string = simplifyTitle($string); // Removes all the special chars like &, /, (, # etc..
$string = stringReplaceAll([' ' => '-'], $string);
return  mb_strtolower( $string, 'UTF-8' );
}

function idToName($id)
{
    global $g_id_convert_ids;
    global $g_id_convert_chars;
    return str_replace  ( $g_id_convert_ids , $g_id_convert_chars, $id);
}

function urlToID(string $url) : string
{
                                    $url_lc = mb_strtolower($url, 'UTF-8');
return sha1($url_lc);
}

//function urlToID($string, $doTrim = true)
//{
//    global $g_url_convert_ids;
//    global $g_url_convert_chars;
//
//    if ($doTrim) $string = trim($string);
//    if ( '' == $string ) return '';
//	$string = str_replace(' ', '%20', $string);	//
//    $n = mb_strtolower( $string, 'UTF-8' );
//    $id = str_replace  ( $g_url_convert_chars, $g_url_convert_ids, $n ); // Originally called nameToPath()
//    return $id;
//}

//function idToUrl($id)
//{
//    global $g_url_convert_ids;
//    global $g_url_convert_chars;
//    return str_replace  ( $g_url_convert_ids , $g_url_convert_chars, $id);
//}


#endif


} // END namespace cpaf::unicode

