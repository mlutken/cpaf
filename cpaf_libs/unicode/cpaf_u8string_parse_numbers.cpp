#include "cpaf_u8string_parse_numbers.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>

// #include <utils/cpaf_string_utils.hpp>

/// #include "cpaf_u8string_utils.h"


namespace cpaf::unicode {

void normalize_locale(std::string::iterator begin,
                             const std::string::iterator& end,
                             char decimal_point,
                             char thousands_separator)
{
    for ( ; begin != end; ++begin) {
        char& c = *begin;
        if (c == decimal_point) {
            c = chars8::dot;
        }
        else if (c == thousands_separator) {
            c = chars8::comma;
        }
    }
}


bool parse_double (
    double& value,
    std::string::const_iterator& begin,
    const std::string::const_iterator& end,
    double default_value,
    char decimal_point,
    char thousands_separator
    ) noexcept
{
    const std::string dp = std::string(1, decimal_point);
    const std::string ts = std::string(1, thousands_separator);

    std::string sStr(begin, end);
    boost::replace_all( sStr, ts, "" );
    boost::replace_all( sStr, dp, "." );

    const char* str = sStr.c_str();
    char* stopStr;
    errno = 0;
    value = strtod(str, &stopStr);
    const bool parsed_ok = (errno == 0) && (*stopStr == '\0');
    if (parsed_ok) {
        int iLen = 	stopStr - str;
        begin = begin + iLen;
    }
    else {
        value = default_value;
    }

    return value;
}


bool parse_double ( double& value,
                    const std::string::const_iterator& begin,
                    const std::string::const_iterator& end,
                    double default_value,
                    char decimal_point,
                    char thousands_separator
                   ) noexcept
{
    std::string::const_iterator begin_mutable = begin;
    return parse_double(value, begin_mutable, end, default_value, decimal_point, thousands_separator);
}

bool parse_double ( double& value,
                    const std::string& sString,
                    double default_value,
                    char decimal_point,
                    char thousands_separator) noexcept
{
    return parse_double (value, sString.begin(), sString.end(), default_value, decimal_point, thousands_separator);
}


double parse_double 	( std::string::const_iterator& begin,
                          const std::string::const_iterator& end,
                          double default_value,
                          char decimal_point,
                          char thousands_separator) noexcept
{
    double value = 0;
    if (!parse_double (value, begin, end, default_value, decimal_point, thousands_separator)) {
        value = default_value;
    }
    return value;
}

double  parse_double 	( const std::string::const_iterator& begin,
                          const std::string::const_iterator& end,
                          double default_value,
                          char decimal_point,
                          char thousands_separator) noexcept
{
    std::string::const_iterator begin_mutable = begin;
    return parse_double (begin_mutable, end, default_value, decimal_point, thousands_separator);
}

double  parse_double 	( const std::string& sString,
                          double default_value,
                          char decimal_point,
                          char thousands_separator
                    ) noexcept
{
    return parse_double (sString.begin(), sString.end(), default_value, decimal_point, thousands_separator);
}

/** Parse thru string trying to find something that can parse as a double.
itFirst is set to rest of string. */
bool parse_find_double(double& fDouble, 							///< [out] Result (double)
                       std::string::const_iterator& itFirst,
                       const std::string::const_iterator& itLast,
                       double default_value,
                       char decimal_point,
                       char thousands_separator
                       ) noexcept
{
    const std::string dp = std::string(1, decimal_point);
    const std::string ts = std::string(1, thousands_separator);

    fDouble = 0;
    if ( itFirst == itLast ) {
        return false;
    }
    struct lconv * lc;
    lc=localeconv();
    std::string sDecimalPointSystem = lc->decimal_point;

    bool bFound = true;
    std::string sStr(itFirst, itLast);
    boost::replace_all( sStr, ts, "" );
    boost::replace_all( sStr, dp, sDecimalPointSystem );

    const char* szIt 	= sStr.c_str();
    const char* szStart = sStr.c_str();
    const char* szEnd 	= sStr.c_str() + sStr.length();
    char* stopStr = NULL;

    while ( fDouble == 0 && ( szIt < szEnd ) ) {
        fDouble = strtod( szIt++, &stopStr );
    }
    if ( fDouble == 0 && (*(--szIt) != '0' ) )	bFound = false;
    int iLen = 	stopStr - szStart;
    itFirst = itFirst + iLen;
    if (!bFound) {
        fDouble = default_value;
    }
    return bFound;
}

bool parse_find_double(
    double& fDouble,
    const std::string::const_iterator& begin,
    const std::string::const_iterator& end,
    double default_value,
    char decimal_point,
    char thousands_separator
    ) noexcept
{
    std::string::const_iterator mutable_begin = begin;
    return parse_find_double(fDouble, mutable_begin, end, default_value, decimal_point, thousands_separator);
}

/** Parse thru string trying to find something that can parse as a double. */
bool parse_find_double(
    double& fDouble, 							///< [out] Result (double)
    const std::string& sInput,
    double default_value,
    char decimal_point,
    char thousands_separator
    ) noexcept
{
    return parse_find_double( fDouble, sInput.begin(), sInput.end(), default_value, decimal_point, thousands_separator );
}

double parse_find_double(
    const std::string& sInput
    , double default_value
    , char decimal_point
    , char thousands_separator
    ) noexcept
{
    double val;
    std::string::const_iterator it = sInput.begin();
    parse_find_double( val, it, sInput.end(), default_value, decimal_point, thousands_separator);
    return val;
}

bool parse_float (float& value,
                 std::string::const_iterator &itFirst,
                 const std::string::const_iterator& itLast,
                 float default_value,
                 char decimal_point,
                 char thousands_separator
                 ) noexcept
{
    const std::string dp = std::string(1, decimal_point);
    const std::string ts = std::string(1, thousands_separator);
    std::string sStr(itFirst, itLast);

    boost::replace_all( sStr, ts, "" );
    boost::replace_all( sStr, dp, "." );

    const char* str = sStr.c_str();
    char* stopStr;
    errno = 0;
    value = strtof(str, &stopStr);
    const bool parsed_ok = (errno == 0) && (*stopStr == '\0');
    if (parsed_ok) {
        int iLen = 	stopStr - str;
        itFirst = itFirst + iLen;
    }
    else {
        value = default_value;
    }

    return value;
}

bool parse_float (float& value,
                  const std::string::const_iterator& begin,
                  const std::string::const_iterator& end,
                  float default_value,
                  char decimal_point,
                  char thousands_separator) noexcept
{
    std::string::const_iterator begin_mutable = begin;
    return parse_float(value, begin_mutable, end, default_value, decimal_point, thousands_separator);
}

bool parse_float (float& value,
                  const std::string& sString,
                  float default_value,
                  char decimal_point,
                  char thousands_separator) noexcept
{
    return parse_float (value, sString.begin(), sString.end(), default_value, decimal_point, thousands_separator);
}


float parse_float 	( std::string::const_iterator& begin,
                      const std::string::const_iterator& end,
                      float default_value,
                      char decimal_point,
                      char thousands_separator) noexcept
{
    float value = 0;
    if (!parse_float (value, begin, end, default_value, decimal_point, thousands_separator)) {
        value = default_value;
    }
    return value;
}

float  parse_float 	( const std::string::const_iterator& begin,
                          const std::string::const_iterator& end,
                          float default_value,
                          char decimal_point,
                          char thousands_separator) noexcept
{
    std::string::const_iterator begin_mutable = begin;
    return parse_float (begin_mutable, end, default_value, decimal_point, thousands_separator);
}

float  parse_float 	( const std::string& sString,
                          float default_value,
                          char decimal_point,
                          char thousands_separator
                  ) noexcept
{
    return parse_float (sString.begin(), sString.end(), default_value, decimal_point, thousands_separator);
}

/** Parse thru string trying to find something that can parse as an float.
\return True if parsing succeeded. */
bool parse_find_float (float& fVal
                      , std::string::const_iterator& itFirst
                      , std::string::const_iterator itLast
                      , float default_value
                      , char decimal_point
                      , char thousands_separator
                      )
{
    double dVal;
    bool parsedOk = parse_find_double (dVal, itFirst, itLast, default_value, decimal_point, thousands_separator);
    fVal = static_cast<float>(dVal);
    return parsedOk;
}

bool parse_find_float(
    float& value,
    const std::string::const_iterator& begin,
    const std::string::const_iterator& end,
    float default_value,
    char decimal_point,
    char thousands_separator) noexcept
{
    std::string::const_iterator mutable_begin = begin;
    return parse_find_float(value, mutable_begin, end, default_value, decimal_point, thousands_separator);
}

/** Parse thru string trying to find something that can parse as an float.
\return True if parsing succeeded. */
bool parse_find_float (float& fVal
                      , const std::string sInput
                      , float default_value
                      , char decimal_point
                      , char thousands_separator
                      )
{
    return parse_find_float(fVal, sInput.begin(), sInput.end(), default_value, decimal_point, thousands_separator);
}


/** Parse thru string trying to find something that can parse as an float.
\return Value parsed or default value if parsing fails. */
float parse_find_float(
    const std::string sInput
    , float default_value
    , char decimal_point
    , char thousands_separator
    )
{
    float val;
    parse_find_float(val, sInput, default_value, decimal_point, thousands_separator);
    return val;
}

bool parse_long(long& value,
                std::string::const_iterator& begin,
                const std::string::const_iterator& end,
                long default_value,
                char thousands_separator,
                int iBase) noexcept
{
    const std::string ts = std::string(1, thousands_separator);

    std::string sStr(begin, end);
    boost::replace_all( sStr, ts, "" );

    const char* str = sStr.c_str();
    char* stopStr;
    errno = 0;
    value = strtol(str, &stopStr, iBase);
    const bool parsed_ok = (errno == 0) && (*stopStr == '\0');
    if (parsed_ok) {
        int iLen = 	stopStr - str;
        begin = begin + iLen;
    }
    else {
        value = default_value;
    }

    return value;
}


bool parse_long (long& value,
                  const std::string::const_iterator& begin,
                  const std::string::const_iterator& end,
                  long default_value,
                  char thousands_separator,
                  int iBase
               ) noexcept
{
    std::string::const_iterator begin_mutable = begin;
    return parse_long(value, begin_mutable, end, default_value, thousands_separator, iBase);
}

bool parse_long (long& value,
                  const std::string& sString,
                  long default_value,
                  char thousands_separator,
                  int iBase
              ) noexcept
{
    return parse_long (value, sString.begin(), sString.end(), default_value, thousands_separator, iBase);
}


long parse_long 	( std::string::const_iterator &begin,
                      const std::string::const_iterator& end,
                      long default_value,
                      char thousands_separator,
                      int iBase
                    ) noexcept
{
    long value;
    parse_long(value, begin, end, default_value, thousands_separator, iBase);
    return value;
}

long 	parse_long 	( const std::string::const_iterator& begin,
                      const std::string::const_iterator& end,
                      long default_value,
                      char thousands_separator,
                      int iBase
                    ) noexcept
{
    std::string::const_iterator mutable_begin = begin;
    return parse_long(mutable_begin, end, default_value, thousands_separator, iBase);
}


long	parse_long 	( const std::string& sString,
                      long default_value,
                      char thousands_separator,
                      int iBase
                    ) noexcept
{
    return parse_long(sString.begin(), sString.end(), default_value, thousands_separator, iBase);
}

bool parse_find_long(long& value,
                     std::string::const_iterator& begin,
                     const std::string::const_iterator& end,
                     long default_value,
                     char thousands_separator,
                     int iBase) noexcept
{
    const std::string ts = std::string(1, thousands_separator);

    value = 0;
    if (begin == end) {
        return false;
    }

    bool bFound = true;
    std::string sStr(begin, end);
    boost::replace_all( sStr, ts, "" );

    const char* szIt    = sStr.c_str();
    const char* szStart = sStr.c_str();
    const char* szEnd   = sStr.c_str() + sStr.length();
    char* stopStr = NULL;

    while ( value == 0 && ( szIt < szEnd ) ) {
        value = strtol( szIt++, &stopStr, iBase );
    }

    if ( value == 0 && (*(--szIt) != '0' ) )   bFound = false;
    int iLen = stopStr - szStart;
    begin = begin + iLen;
    if (!bFound) {
        value = default_value;
    }
    return bFound;

}

bool parse_find_long(long& value,
                     const std::string::const_iterator& begin,
                     const std::string::const_iterator& end,
                     long default_value,
                     char thousands_separator,
                     int iBase) noexcept
{
    std::string::const_iterator mutable_begin = begin;
    return parse_find_long(value, mutable_begin, end, default_value, thousands_separator, iBase);
}

bool parse_find_long(long& value,
                     const std::string sInput,
                     long default_value,
                     char thousands_separator,
                     int iBase) noexcept
{
    std::string::const_iterator it = sInput.begin();
    return parse_find_long(value, it, sInput.end(), default_value, thousands_separator, iBase);
}

long parse_find_long(const std::string sInput,
                     long default_value,
                     char thousands_separator,
                     int iBase) noexcept
{
    long val;
    parse_find_long(val, sInput, default_value, thousands_separator, iBase);
    return val;
}


bool parse_int(int& value,
               std::string::const_iterator& begin,
               const std::string::const_iterator& end,
               int default_value,
               char thousands_separator,
               int iBase) noexcept
{
    const std::string ts = std::string(1, thousands_separator);

    std::string sStr(begin, end);
    boost::replace_all( sStr, ts, "" );

    const char* str = sStr.c_str();
    char* stopStr;
    errno = 0;
    value = strtol(str, &stopStr, iBase);
    const bool parsed_ok = (errno == 0) && (*stopStr == '\0');
    if (parsed_ok) {
        int iLen = 	stopStr - str;
        begin = begin + iLen;
    }
    else {
        value = default_value;
    }

    return value;
}



bool parse_int (int& value,
                  const std::string::const_iterator& begin,
                  const std::string::const_iterator& end,
                  int default_value,
                  char thousands_separator,
                  int iBase
               ) noexcept
{
    std::string::const_iterator begin_mutable = begin;
    return parse_int (value, begin_mutable, end, default_value, thousands_separator, iBase);
}

bool parse_int (int& value,
                  const std::string& sString,
                  int default_value,
                  char thousands_separator,
                  int iBase
              ) noexcept
{
    return parse_int (value, sString.begin(), sString.end(), default_value, thousands_separator, iBase);
}


int parse_int 	( std::string::const_iterator& begin,
                  const std::string::const_iterator& end,
                  int default_value,
                  char thousands_separator,
                  int iBase
                    ) noexcept
{
    int value;
    parse_int(value, begin, end, default_value, thousands_separator, iBase);
    return value;
}

int 	parse_int 	( const std::string::const_iterator& begin,
                      const std::string::const_iterator& end,
                      int default_value,
                      char thousands_separator,
                      int iBase
                    ) noexcept
{
    std::string::const_iterator mutable_begin = begin;
    return parse_int(mutable_begin, end, default_value, thousands_separator, iBase);
}


int	parse_int 	( const std::string& sString,
                  int default_value,
                  char thousands_separator,
                  int iBase
                    ) noexcept
{
    return parse_int(sString.begin(), sString.end(), default_value, thousands_separator, iBase);
}

bool parse_find_int(int& value,
                    std::string::const_iterator& begin,
                    const std::string::const_iterator& end,
                    int default_value,
                    char thousands_separator,
                    int iBase) noexcept
{
    long value_long;
    bool parsed_ok = parse_find_long (value_long, begin, end, default_value, thousands_separator, iBase);
    value = static_cast<int>(value_long);
    return parsed_ok;
}

bool parse_find_int(int& value,
                    const std::string::const_iterator& begin,
                    const std::string::const_iterator& end,
                    int default_value,
                    char thousands_separator,
                    int iBase) noexcept
{
    std::string::const_iterator mutable_begin = begin;
    return parse_find_int(value, mutable_begin, end, default_value, thousands_separator, iBase);
}

bool parse_find_int(int& value,
                    const std::string& sInput,
                    int default_value,
                    char thousands_separator,
                    int iBase) noexcept
{
    return parse_find_int(value, sInput.begin(), sInput.end(), default_value, thousands_separator, iBase);
}

int parse_find_int(const std::string& sInput,
                   int default_value,
                   char thousands_separator,
                   int iBase) noexcept
{
    int val;
    parse_find_int(val, sInput, default_value, thousands_separator, iBase);
    return val;
}



} // END namespace cpaf::unicode

