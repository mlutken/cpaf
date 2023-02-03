#include "phpdoc_utils.h"

// #include <boost/algorithm/string.hpp>
// #include <cpaf_string_utils.hpp>

using namespace std;

namespace phpdoc {


ParseString toParseString(const std::string& str)
{
    return ParseString(str);
}

std::string toStdString(const ParseString& str)
{
    return str;
}

std::string toStdString(const Iterator& first, const Iterator& last)
{
    return string(first, last);
}


// -----------------------
// --- Debug functions ---
// -----------------------



} // END namespace phpdoc

